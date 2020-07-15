#include "BTCompilerVMBackend.h"
#include "BlueprintEditor/Core/Architect/K3Node.h"
#include "BlueprintToolScript/Public/BTScript.h"
#include "Misc/DefaultValueHelper.h"
#include "Internationalization/TextNamespaceUtil.h"
#include "UObject/TextProperty.h"
#include "UObject/PropertyPortFlags.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

#define VM_DefaultValueByString \
FString DefaultValue; \
if ((ValueAddr.IsBPVariable || (ValueAddr.RHS && Cast<UK3Node>(ValueAddr.RHS->GetOwningNode())->IsPure())))\
{\
	DefaultValue = ValueAddr.RHS->DefaultValue;\
}\
else\
{\
	DefaultValue = ValueAddr.DefaultValue;\
}

#define VM_DefaultValueByText \
FText DefaultTextValue; \
if ((ValueAddr.IsBPVariable || (ValueAddr.RHS && Cast<UK3Node>(ValueAddr.RHS->GetOwningNode())->IsPure())))\
{\
	DefaultTextValue = ValueAddr.RHS->DefaultTextValue;\
}\
else\
{\
	DefaultTextValue = ValueAddr.DefaultTextValue;\
}

//获取连接pin的Property
UProperty *GetPropertyByPin(UEdGraphPin *Pin)
{
	UProperty * NewProp = nullptr;
	UK3Node *K3 = Cast<UK3Node>(Pin->GetOwningNode());
	if (K3)
	{
		if (K3->Function)
		{
			for (TFieldIterator<UProperty> i(K3->Function); i; ++i)
			{
				UProperty *Prop = *i;
				if (Prop->GetFName() == Pin->PinName)
				{
					NewProp = Prop;
					break;
				}
			}
		}
		else if (K3->Property)
		{
			NewProp = K3->Property;
		}
	}

	return NewProp;
}

namespace Scripttecode
{
	class FScripttecodeWriter : public FArchiveUObject
	{
	public:
		TArray<uint8>& ScriptBuffer;
	public:
		FScripttecodeWriter(TArray<uint8>& InScriptBuffer)
			: ScriptBuffer(InScriptBuffer)
		{
		}

		virtual void Serialize(void* V, int64 Length) override
		{
			int32 iStart = ScriptBuffer.AddUninitialized(Length);
			FMemory::Memcpy(&(ScriptBuffer[iStart]), V, Length);
		}

		using FArchiveUObject::operator<<;

		FArchive& operator<<(FName& Name) override
		{
			FArchive& Ar = *this;

			FScriptName ScriptName = NameToScriptName(Name);
			Ar << ScriptName.ComparisonIndex;
			Ar << ScriptName.DisplayIndex;
			Ar << ScriptName.Number;

			return Ar;
		}

		FArchive& operator<<(UObject*& Res) override
		{
			ScriptPointerType D = (ScriptPointerType)Res;
			FArchive& Ar = *this;

			Ar << D;
			return Ar;
		}

		FArchive& operator<<(TCHAR* S)
		{
			Serialize(S, FCString::Strlen(S) + 1);
			return *this;
		}

		FArchive& operator<<(EVMCommand E)
		{
			checkSlow(E < 0xFF);

			uint8 B = E;
			Serialize(&B, 1);
			return *this;
		}

		FArchive& operator<<(EBlueprintTextLiteralType E)
		{
			static_assert(sizeof(__underlying_type(EBlueprintTextLiteralType)) == sizeof(uint8), "EBlueprintTextLiteralType is expected to be a uint8");

			uint8 B = (uint8)E;
			Serialize(&B, 1);
			return *this;
		}

		FArchive& operator<<(FLazyObjectPtr& LazyObjectPtr) override
		{
			return FArchive::operator<<(LazyObjectPtr);
		}
	};
}

struct FScriptBuilder
{
public:
	Scripttecode::FScripttecodeWriter Writer;
public:
	virtual ~FScriptBuilder(){}

	FScriptBuilder(TArray<uint8>& InScriptBuffer)
		: Writer(Scripttecode::FScripttecodeWriter(InScriptBuffer))
	{
		VectorStruct = TBaseStructure<FVector>::Get();
		RotatorStruct = TBaseStructure<FRotator>::Get();
		TransformStruct = TBaseStructure<FTransform>::Get();
	}
private:
	UScriptStruct* VectorStruct;
	UScriptStruct* RotatorStruct;
	UScriptStruct* TransformStruct;
public:

	void EmitStringLiteral(const FString& String)
	{
		if (FCString::IsPureAnsi(*String))
		{
			Writer << EVMCommand::VMC_StringConst;
			uint8 OutCh;
			for (const TCHAR* Ch = *String; *Ch; ++Ch)
			{
				OutCh = CharCast<ANSICHAR>(*Ch);
				Writer << OutCh;
			}

			OutCh = 0;
			Writer << OutCh;
		}
		else
		{
			Writer << VMC_UnicodeStringConst;
			uint16 OutCh;
			for (const TCHAR* Ch = *String; *Ch; ++Ch)
			{
				OutCh = CharCast<UCS2CHAR>(*Ch);
				Writer << OutCh;
			}

			OutCh = 0;
			Writer << OutCh;
		}
	}

	virtual void EmitTermExpr(UProperty* CoerceProperty, const FFunctionTerminal &ValueAddr)
	{
		if (ValueAddr.Direction == EEdGraphPinDirection::EGPD_Input)
		{
			if (CoerceProperty->IsA(UInt64Property::StaticClass()))
			{
				if (ValueAddr.IsLink)
				{
					Writer << VMC_LetInt64;
				}
				else
				{
					VM_DefaultValueByString

					int64 Value = 0;
					LexFromString(Value, *DefaultValue);
					Writer << VMC_Int64Const;
					Writer << Value;

					return;
				}
			}
			else if (CoerceProperty->IsA(UUInt64Property::StaticClass()))
			{
				if (ValueAddr.IsLink)
				{
					Writer << VMC_LetUInt64;
				}
				else
				{
					VM_DefaultValueByString

					uint64 Value = 0;
					LexFromString(Value, *DefaultValue);
					Writer << VMC_UInt64Const;
					Writer << Value;

					return;
				}
			}
			else if (CoerceProperty->IsA(UByteProperty::StaticClass()))
			{
				if (ValueAddr.IsLink)
				{
					Writer << VMC_LetByte;
				}
				else
				{
					VM_DefaultValueByString

					uint8 Value = 0;

					UEnum* EnumPtr = nullptr;

					if (UByteProperty* ByteProp = Cast< UByteProperty >(CoerceProperty))
					{
						EnumPtr = ByteProp->Enum;
					}
					else if (UEnumProperty* EnumProp = Cast< UEnumProperty >(CoerceProperty))
					{
						EnumPtr = EnumProp->GetEnum();
					}

					//Parameter property can represent a generic byte. we need the actual type to parse the value.
					//if (!EnumPtr)
					//{
					//	EnumPtr = Cast<UEnum>(Term->Type.PinSubCategoryObject.Get());
					//}

					//Check for valid enum object reference
					if (EnumPtr)
					{
						//Get value from enum string
						Value = EnumPtr->GetValueByName(*DefaultValue);
					}
					else
					{
						Value = FCString::Atoi(*DefaultValue);
					}

					Writer << VMC_ByteConst;
					Writer << Value;

					return;
				}
			}
			else if (CoerceProperty->IsA(UStrProperty::StaticClass()))
			{
				if (ValueAddr.IsLink)
				{
					Writer << VMC_LetString;
				}
				else
				{
					VM_DefaultValueByString

					EmitStringLiteral(DefaultValue);

					return;
				}
			}
			else if (CoerceProperty->IsA(UObjectProperty::StaticClass()) || CoerceProperty->IsA(UClassProperty::StaticClass()))
			{
				if (ValueAddr.IsLink)
				{
					Writer << VMC_LetObject;
				}
				else
				{
					VM_DefaultValueByString

					if (DefaultValue.IsEmpty())
					{
						Writer << VMC_NoObject;
					}
					else
					{
						UObject * Obj = LoadObject<UObject>(NULL, *DefaultValue);
						if (Obj)
						{
							Writer << VMC_ObjectConst;
							Writer << Obj;
							return;
						}
						else
						{
							Writer << VMC_NoObject;
						}
					}

					return;
				}
			}
			else if (CoerceProperty->IsA(UIntProperty::StaticClass()))
			{
				if (ValueAddr.IsLink)
				{
					Writer << VMC_LetInt;
				}
				else
				{
					VM_DefaultValueByString

					int32 Value = FCString::Atoi(*DefaultValue);
					Writer << VMC_IntConst;
					Writer << Value;
					return;
				}
			}
			else if (CoerceProperty->IsA(UBoolProperty::StaticClass()))
			{
				if (ValueAddr.IsLink)
				{
					Writer << VMC_LetBool;
				}
				else
				{
					VM_DefaultValueByString

					bool Value = DefaultValue.ToBool();
					Writer << (Value ? VMC_True : VMC_False);
					return;
				}
			}
			else if (CoerceProperty->IsA(UFloatProperty::StaticClass()))
			{
				if (ValueAddr.IsLink)
				{
					Writer << VMC_LetFloat;
				}
				else
				{
					VM_DefaultValueByString

					float Value = FCString::Atof(*DefaultValue);

					Writer << VMC_FloatConst;
					Writer << Value;

					return;
				}
			}
			else if (CoerceProperty->IsA(UNameProperty::StaticClass()))
			{
				if (ValueAddr.IsLink)
				{
					Writer << VMC_LetName;
				}
				else
				{
					VM_DefaultValueByString

					FName LiteralName(*DefaultValue);

					Writer << VMC_NameConst;
					Writer << LiteralName;

					return;
				}
			}
			else if (CoerceProperty->IsA(UTextProperty::StaticClass()))
			{
				if (ValueAddr.IsLink)
				{
					Writer << VMC_LetText;
				}
				else
				{
					VM_DefaultValueByText

					Writer << VMC_TextConst;
					const FString& StringValue = FTextInspector::GetDisplayString(DefaultTextValue);

					if (DefaultTextValue.IsEmpty())
					{
						Writer << EBlueprintTextLiteralType::Empty;
					}
					else if (DefaultTextValue.IsCultureInvariant())
					{
						Writer << EBlueprintTextLiteralType::InvariantText;
						EmitStringLiteral(StringValue);
					}
					else
					{
						bool bIsLocalized = false;
						FString Namespace;
						FString Key;
						const FString* SourceString = FTextInspector::GetSourceString(DefaultTextValue);

						if (SourceString && DefaultTextValue.ShouldGatherForLocalization())
						{
							bIsLocalized = FTextLocalizationManager::Get().FindNamespaceAndKeyFromDisplayString(FTextInspector::GetSharedDisplayString(DefaultTextValue), Namespace, Key);
						}

						if (bIsLocalized)
						{
							// BP bytecode always removes the package localization ID to match how text works at runtime
							// If we're gathering editor-only text then we'll pick up the version with the package localization ID from the property/pin rather than the bytecode
							Namespace = TextNamespaceUtil::StripPackageNamespace(Namespace);

							Writer << EBlueprintTextLiteralType::LocalizedText;
							EmitStringLiteral(*SourceString);
							EmitStringLiteral(Key);
							EmitStringLiteral(Namespace);
						}
						else
						{
							Writer << EBlueprintTextLiteralType::LiteralString;
							EmitStringLiteral(StringValue);
						}
					}

					return;
				}
			}
			else if (CoerceProperty->IsA(UStructProperty::StaticClass()))
			{
				if (ValueAddr.IsLink)
				{
					UStructProperty* StructProperty = Cast<UStructProperty>(CoerceProperty);
					UScriptStruct* Struct = StructProperty->Struct;
					if (Struct == VectorStruct)
					{
						Writer << VMC_LetVector;
					}
					else if (Struct == RotatorStruct)
					{
						Writer << VMC_LetRotator;
					}
					else if (Struct == TransformStruct)
					{
						Writer << VMC_LetTransform;
					}
				}
				else
				{
					VM_DefaultValueByString

					UStructProperty* StructProperty = Cast<UStructProperty>(CoerceProperty);
					UScriptStruct* Struct = StructProperty->Struct;
					if (Struct == VectorStruct)
					{
						FVector V = FVector::ZeroVector;
						if (!DefaultValue.IsEmpty())
						{
							const bool bParsedUsingCustomFormat = FDefaultValueHelper::ParseVector(DefaultValue, /*out*/ V);
							if (!bParsedUsingCustomFormat)
							{
								Struct->ImportText(*DefaultValue, &V, nullptr, EPropertyPortFlags::PPF_None, nullptr, GetPathNameSafe(StructProperty));
							}
						}
						Writer << VMC_VectorConst;
						Writer << V;
					}
					else if (Struct == RotatorStruct)
					{
						FRotator R = FRotator::ZeroRotator;
						if (!DefaultValue.IsEmpty())
						{
							const bool bParsedUsingCustomFormat = FDefaultValueHelper::ParseRotator(DefaultValue, /*out*/ R);
							if (!bParsedUsingCustomFormat)
							{
								Struct->ImportText(*DefaultValue, &R, nullptr, PPF_None, nullptr, GetPathNameSafe(StructProperty));
							}
						}
						Writer << VMC_RotationConst;
						Writer << R;
					}
					else if (Struct == TransformStruct)
					{
						FTransform T = FTransform::Identity;
						if (!DefaultValue.IsEmpty())
						{
							const bool bParsedUsingCustomFormat = T.InitFromString(DefaultValue);
							if (!bParsedUsingCustomFormat)
							{
								Struct->ImportText(*DefaultValue, &T, nullptr, PPF_None, nullptr, GetPathNameSafe(StructProperty));
							}
						}
						Writer << VMC_TransformConst;
						Writer << T;
					}
					else
					{
						//有可能是结构体...
						//
					}

					return;
				}
			}

			//UProperty * NewProp = GetPropertyByPin(ValueAddr.RHS);
			FGuid ID = ValueAddr.RHS->PinId;
			Writer << ID;
		}
		else //Output
		{
			Writer << VMC_SaveValue;
			FGuid EmptyLinkID = ValueAddr.PinId;
			Writer << EmptyLinkID;
			//if (ValueAddr.IsLink)
			//{
			//	for (UEdGraphPin *pin : ValueAddr.LHS)
			//	{
			//		Writer << VMC_SaveValue;
			//		Writer << pin->PinId;
			//	}
			//}
			//else
			//{
			//	//必须存储，因为占位
			//	Writer << VMC_SaveValue;
			//	FGuid EmptyLinkID = ValueAddr.PinId;
			//	Writer << EmptyLinkID;
			//}
		}
	}
};

void FVM::GenerateBytecode()
{
	if (FunctionList)
	{
		UK3Node *EventNode = Cast<UK3Node>(FunctionList->LinearExecutionList[0]);
		FScriptBuilder Writer(EventNode->Function->Script);
		for (auto &Tmp : FunctionList->ListVMStatement)
		{
			UK3Node *Node = Cast<UK3Node>(Tmp.Key);
			if (Node->Function)
			{			
				for (EVMStatementType &Statement : Tmp.Value)
				{
					switch (Statement)
					{
					case VMS_CallFunc:
					{
						Writer.Writer << EVMCommand::VMC_Funtion;
						Writer.Writer << Node->Function;

						break;
					}
					case VMS_Nest:
					{
						//序列化我们的参数
						for (TFieldIterator<UProperty> i(Node->Function); i; ++i)
						{
							UProperty *UProp = *i;
							FFunctionTerminal NewTerminal = FunctionList->FindTerm(Node, UProp);
						
							Writer.EmitTermExpr(UProp, NewTerminal);
						}

						//找到当前的node
						UEdGraphNode *CurrentNode = Node;
						//for (UEdGraphNode *NodeTmp : FunctionList->LinearExecutionList)
						//{
						//	UK3Node *NewNode = Cast<UK3Node>(NodeTmp);
						//	if (NewNode->Function == Node->Function && NewNode != Node)
						//	{
						//		CurrentNode = Node;
						//		break;
						//	}
						//}
						//寻找当前node对应的序列
						int32 index = FunctionList->LinearExecutionList.Find(CurrentNode);
						index++;

						if (FunctionList->LinearExecutionList.IsValidIndex(index))
						{
							UEdGraphNode *NestNode = FunctionList->LinearExecutionList[index];
							UK3Node *NestNode3 = Cast<UK3Node>(NestNode);

							//写入字节码
							Writer.Writer << EVMCommand::VMC_Funtion;
							Writer.Writer << NestNode3->Function;
						}

						break;
					}
					case VMS_Return:
						Writer.Writer << VMC_EndOfScript;
						break;
					default:
						break;
					}
				}
			}
		}
	}
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif