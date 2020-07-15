#include "BTScript.h"
#include "ScriptObject.h"
#include "FunctionManage.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

template<class T>
void ConversionBaseProp(FBTFrame& Stack, void const *RefData)
{
	FGuid ID = Stack.ReadPinID();
	void const *Addr = FindTemporaryStack(ID);
	*(T*)RefData = *(T*)Addr;
}

template<class T>
void ConversionBaseObject(FBTFrame& Stack, void const *RefData)
{
	FGuid ID = Stack.ReadPinID();
	void const *Addr = FindTemporaryStack(ID);
	*(T**)RefData = (T*)Addr;
}

class FTemporaryStack : public TSharedFromThis<FTemporaryStack>
{
public:
	FTemporaryStack(void const* Inddr, FGuid Prop)
		:Addr(Inddr)
		, PropID(Prop)
	{}

	FGuid PropID;
	void const *Addr;
	TSharedPtr<FTemporaryStack> Nest;
};

//作为临时存储，一个函数逻辑一个该实例
TSharedPtr<FTemporaryStack> TemporaryStack;

void Add(FGuid PropID, void const *Addr)
{
	TSharedPtr<FTemporaryStack> ListParm = TemporaryStack;
	if (!ListParm.IsValid())
	{
		TemporaryStack = MakeShareable(new FTemporaryStack(Addr, PropID));
	}
	else
	{
		while (ListParm->Nest.IsValid())
		{
			ListParm = ListParm->Nest;
		}

		ListParm->Nest = MakeShareable(new FTemporaryStack(Addr, PropID));
	}
}

void const *FindTemporaryStack(FGuid PropID)
{
	TSharedPtr<FTemporaryStack> FindTarget = TemporaryStack;
	bool bFound = false;
	for (; FindTarget.IsValid() && !bFound;)
	{
		if (FindTarget->PropID == PropID)
		{
			bFound = true;
			break;
		}

		FindTarget = FindTarget->Nest;
	}

	return FindTarget->Addr;
}

TSet<UFunction* > UBTScriptObject::FunctionList;

FNativeBTFuncPtr GBTNatives[VMC_Max];

uint8 GBTRegisterNative(int32 NativeBytecodeIndex, const FNativeBTFuncPtr& Func)
{
	static bool bInitialized = false;
	if (!bInitialized)
	{
		bInitialized = true;
		for (uint32 i = 0; i < ARRAY_COUNT(GBTNatives); i++)
		{
			GBTNatives[i] = &UBTScriptObject::VMC_Undefined;
		}
	}
	if (NativeBytecodeIndex != INDEX_NONE)
	{
		if (!(NativeBytecodeIndex<0 || 
			(uint32)NativeBytecodeIndex>ARRAY_COUNT(GBTNatives) ||
			GBTNatives[NativeBytecodeIndex] != &UBTScriptObject::VMC_Undefined))
		{
			CA_SUPPRESS(6386)
			GBTNatives[NativeBytecodeIndex] = Func;
		}
	}
	
	return 0;
}

void FBTFrame::AddOutParm(void const *Addr)
{
	TSharedPtr<FBTOutParm> ListParm = BTOutParm;
	if (!ListParm.IsValid())
	{
		BTOutParm = MakeShareable(new FBTOutParm);
		BTOutParm->PropAddr = Addr;
	}
	else
	{
		while (ListParm->Nest.IsValid())
		{
			ListParm = ListParm->Nest;
		}

		ListParm->Nest = MakeShareable(new FBTOutParm);
		ListParm->Nest->PropAddr = Addr;
	}
}

float FBTFrame::ReadFloat()
{
	float Result;
#if PLATFORM_SUPPORTS_UNALIGNED_LOADS
	Result = *(float*)Code;
#else
	FMemory::Memcpy(&Result, Code, sizeof(float));
#endif
	Code += sizeof(float);
	return Result;
}

UProperty * FBTFrame::ReadProperty()
{
	UProperty* Result = (UProperty*)ReadObject();

	// Callers don't check for NULL; this method is expected to succeed.
	check(Result);
	return Result;
}

FName FBTFrame::ReadName()
{
	FScriptName Result;
#if PLATFORM_SUPPORTS_UNALIGNED_LOADS
	Result = *(FScriptName*)Code;
#else
	FMemory::Memcpy(&Result, Code, sizeof(FScriptName));
#endif
	Code += sizeof(FScriptName);
	return ScriptNameToName(Result);
}

FGuid FBTFrame::ReadPinID()
{
	FGuid Result;
	Result = *(FGuid*)Code;
	Code += sizeof(FGuid);

	return Result;
}

UObject* FBTFrame::ReadObject()
{
	 int64 TempCode = *(int64 *)Code;
	 UObject* Result = (UObject*)(TempCode);

	 Code += sizeof(int64);

	 return Result;
}

void FBTFrame::Step(UObject* Context, void const * RefData)
{
	int32 B = *Code++;
	(GBTNatives[B])(Context, *this, RefData);
}

uint8 * FBTFrame::GetParmAddr()
{
	void const *Proper = BTOutParm->PropAddr;
	BTOutParm = BTOutParm->Nest;

	Step(nullptr, Proper);

	return  (uint8 *) const_cast<void*>(Proper);
}

void FBTFrame::ClearParm()
{
	BTOutParm.Reset();
}

FBTFrame::FBTFrame(UFunction *NewFunction)
	:Code(NewFunction->Script.GetData())
	, Function(NewFunction)
{

}

void UBTScriptObject::CallBTFunction(FBTFrame& Stack, void const *Data, UFunction* Function)
{
	//获取变量
	UProperty *ReturnProp = nullptr;
	for (TFieldIterator<UProperty> i(Function); i; ++i)
	{
		UProperty * Proper = *i;
		if (Proper->PropertyFlags & CPF_ReturnParm)
		{
			ReturnProp = Proper;
		}
		else
		{
			//声明局部变量
			uint8* LocalTempResult = (uint8*)FMemory_Alloca(Proper->GetSize());
			Proper->InitializeValue(LocalTempResult);
			Stack.AddOutParm(LocalTempResult);
		}
	}

	if (ReturnProp == nullptr)
	{
		//执行本地的C++函数
		FFuntionManage::GetNativeFuncPtr(Function)(nullptr, Stack, Data);
	}
	else
	{
		int32 N = ReturnProp->GetSize();
//		Data = new uint8[(ReturnProp->GetSize())]; 
		Data = (uint8*)FMemory_Alloca(ReturnProp->GetSize());//alloca
		ReturnProp->InitializeValue(const_cast<void *>(Data));//对我们的内存进行初始化
		FFuntionManage::GetNativeFuncPtr(Function)(nullptr, Stack, Data);
		Stack.Step(NULL, Data);
	}

	//清除局部变量
	Stack.ClearParm();

	//执行字节码对应的命令
	Stack.Step(NULL, Data);
}

UFunction * UBTScriptObject::FindScriptFuntion(FName FunName)
{
	UFunction *NewFuntion = nullptr;
	for (UFunction *Fun : FunctionList)
	{
		if (Fun->GetFName() == FunName)
		{
			NewFuntion = Fun;
		}
	}

	return NewFuntion;
}

BT_VM_FUNCTION(UBTScriptObject::VMC_LetBool)
{
	*(int32*)RefData = Stack.ReadInt<int32>();
}
Add_VM_FUNCTION(VMC_LetBool)

BT_VM_FUNCTION(UBTScriptObject::VMC_IntConst)
{
	*(int32*)RefData = Stack.ReadInt<int32>();
}
Add_VM_FUNCTION(VMC_IntConst)

BT_VM_FUNCTION(UBTScriptObject::VMC_Funtion)
{
	UBTScriptObject::CallBTFunction(Stack, RefData, (UFunction*)Stack.ReadObject());
}
Add_VM_FUNCTION(VMC_Funtion)

BT_VM_FUNCTION(UBTScriptObject::VMC_StringConst)
{
	*(FString*)RefData = (ANSICHAR*)Stack.Code;

	while (*Stack.Code)
		Stack.Code++;
	Stack.Code++;
}
Add_VM_FUNCTION(VMC_StringConst)

BT_VM_FUNCTION(UBTScriptObject::VMC_EndOfScript)
{
	TemporaryStack.Reset();
}
Add_VM_FUNCTION(VMC_EndOfScript)

BT_VM_FUNCTION(UBTScriptObject::VMC_Undefined)
{

}
Add_VM_FUNCTION(VMC_Undefined)


void UBTScriptObject::ExecutionScript(UFunction * Function, void const *Data)
{
	check(Function);
	if (Function->Script.Num() == 0)
	{
		return;
	}

	FBTFrame Stack(Function);

	Stack.Step(nullptr, Data);
}
//
//void Main()
//{
//	int32 i = 0;
//	int32 j = 0;
//	PPPP(i, j);
//
//	PP(j, i);
//}
//void PP(int32 i, int32 &j)
//{
//	i = j;
//}
//void PPPP(int32 i, int32 &j)
//{
//	j = 10;
//}

BT_VM_FUNCTION(UBTScriptObject::VMC_LetString)
{
	ConversionBaseProp<FString>(Stack, RefData);
}
Add_VM_FUNCTION(VMC_LetString)

BT_VM_FUNCTION(UBTScriptObject::VMC_LetInt)
{
	ConversionBaseProp<int32>(Stack, RefData);
}
Add_VM_FUNCTION(VMC_LetInt)

BT_VM_FUNCTION(UBTScriptObject::VMC_LetObject)
{
	ConversionBaseObject<UObject>(Stack, RefData);
}
Add_VM_FUNCTION(VMC_LetObject)

BT_VM_FUNCTION(UBTScriptObject::VMC_LetName)
{
	ConversionBaseProp<FName>(Stack, RefData);
}
Add_VM_FUNCTION(VMC_LetName)

BT_VM_FUNCTION(UBTScriptObject::VMC_LetFloat)
{
	ConversionBaseProp<float>(Stack, RefData);
}
Add_VM_FUNCTION(VMC_LetFloat)

BT_VM_FUNCTION(UBTScriptObject::VMC_LetInt64)
{
	ConversionBaseProp<int64>(Stack, RefData);
}
Add_VM_FUNCTION(VMC_LetInt64)

BT_VM_FUNCTION(UBTScriptObject::VMC_LetUInt64)
{
	ConversionBaseProp<uint64>(Stack, RefData);
}
Add_VM_FUNCTION(VMC_LetUInt64)

BT_VM_FUNCTION(UBTScriptObject::VMC_LetByte)
{
	ConversionBaseProp<uint8>(Stack, RefData);
}
Add_VM_FUNCTION(VMC_LetByte)

BT_VM_FUNCTION(UBTScriptObject::VMC_LetText)
{
	ConversionBaseProp<FText>(Stack, RefData);
}
Add_VM_FUNCTION(VMC_LetText)

BT_VM_FUNCTION(UBTScriptObject::VMC_LetVector)
{
	ConversionBaseProp<FVector>(Stack, RefData);
}
Add_VM_FUNCTION(VMC_LetVector)

BT_VM_FUNCTION(UBTScriptObject::VMC_LetRotator)
{
	ConversionBaseProp<FRotator>(Stack, RefData);
}
Add_VM_FUNCTION(VMC_LetRotator)

BT_VM_FUNCTION(UBTScriptObject::VMC_LetTransform)
{
	ConversionBaseProp<FTransform>(Stack, RefData);
}
Add_VM_FUNCTION(VMC_LetTransform)

BT_VM_FUNCTION(UBTScriptObject::VMC_SaveValue)
{
	FGuid ID = Stack.ReadPinID();
	Add(ID, RefData);
}
Add_VM_FUNCTION(VMC_SaveValue)

BT_VM_FUNCTION(UBTScriptObject::VMC_FloatConst)
{
	*(float*)RefData = Stack.ReadFloat();
}
Add_VM_FUNCTION(VMC_FloatConst)

BT_VM_FUNCTION(UBTScriptObject::VMC_Int64Const)
{
	*(int64*)RefData = Stack.ReadInt<int64>();
}
Add_VM_FUNCTION(VMC_Int64Const)

BT_VM_FUNCTION(UBTScriptObject::VMC_UInt64Const)
{
	*(uint64*)RefData = Stack.ReadInt<uint64>();
}
Add_VM_FUNCTION(VMC_UInt64Const)

BT_VM_FUNCTION(UBTScriptObject::VMC_UnicodeStringConst)
{
	*(FString*)RefData = FString((UCS2CHAR*)Stack.Code);

	while (*(uint16*)Stack.Code)
	{
		Stack.Code += sizeof(uint16);
	}
	Stack.Code += sizeof(uint16);
}
Add_VM_FUNCTION(VMC_UnicodeStringConst)

BT_VM_FUNCTION(UBTScriptObject::VMC_TransformConst)
{
	// Rotation
	FQuat TmpRotation;
	TmpRotation.X = Stack.ReadFloat();
	TmpRotation.Y = Stack.ReadFloat();
	TmpRotation.Z = Stack.ReadFloat();
	TmpRotation.W = Stack.ReadFloat();

	// Translation
	FVector TmpTranslation;
	TmpTranslation.X = Stack.ReadFloat();
	TmpTranslation.Y = Stack.ReadFloat();
	TmpTranslation.Z = Stack.ReadFloat();

	// Scale
	FVector TmpScale;
	TmpScale.X = Stack.ReadFloat();
	TmpScale.Y = Stack.ReadFloat();
	TmpScale.Z = Stack.ReadFloat();

	((FTransform*)RefData)->SetComponents(TmpRotation, TmpTranslation, TmpScale);
}
Add_VM_FUNCTION(VMC_TransformConst)

BT_VM_FUNCTION(UBTScriptObject::VMC_NoObject)
{
	*(UObject**)RefData = nullptr;
}
Add_VM_FUNCTION(VMC_NoObject)

BT_VM_FUNCTION(UBTScriptObject::VMC_True)
{
	*(bool*)RefData = true;
}
Add_VM_FUNCTION(VMC_True)

BT_VM_FUNCTION(UBTScriptObject::VMC_False)
{
	*(bool*)RefData = false;
}
Add_VM_FUNCTION(VMC_False)

BT_VM_FUNCTION(UBTScriptObject::VMC_TextConst)
{
	// What kind of text are we dealing with?
	const EBlueprintTextLiteralType TextLiteralType = (EBlueprintTextLiteralType)*Stack.Code++;

	switch (TextLiteralType)
	{
	case EBlueprintTextLiteralType::Empty:
	{
		*(FText*)RefData = FText::GetEmpty();
	}
	break;

	case EBlueprintTextLiteralType::LocalizedText:
	{
		FString SourceString;
		Stack.Step(nullptr, &SourceString);

		FString KeyString;
		Stack.Step(nullptr, &KeyString);

		FString Namespace;
		Stack.Step(nullptr, &Namespace);

		*(FText*)RefData = FInternationalization::ForUseOnlyByLocMacroAndGraphNodeTextLiterals_CreateText(*SourceString, *Namespace, *KeyString);
	}
	break;

	case EBlueprintTextLiteralType::InvariantText:
	{
		FString SourceString;
		Stack.Step(nullptr, &SourceString);

		*(FText*)RefData = FText::AsCultureInvariant(MoveTemp(SourceString));
	}
	break;

	case EBlueprintTextLiteralType::LiteralString:
	{
		FString SourceString;
		Stack.Step(nullptr, &SourceString);

		*(FText*)RefData = FText::FromString(MoveTemp(SourceString));
	}
	break;

	case EBlueprintTextLiteralType::StringTableEntry:
	{
		Stack.ReadObject(); // String Table asset (if any)

		FString TableIdString;
		Stack.Step(nullptr, &TableIdString);

		FString KeyString;
		Stack.Step(nullptr, &KeyString);

		*(FText*)RefData = FText::FromStringTable(FName(*TableIdString), KeyString);
	}
	break;

	default:
		checkf(false, TEXT("Unknown EBlueprintTextLiteralType! Please update UObject::execTextConst to handle this type of text."));
		break;
	}
}
Add_VM_FUNCTION(VMC_TextConst)

BT_VM_FUNCTION(UBTScriptObject::VMC_ByteConst)
{
	*(uint8*)RefData = *Stack.Code++;
}
Add_VM_FUNCTION(VMC_ByteConst)

BT_VM_FUNCTION(UBTScriptObject::VMC_VectorConst)
{
	((FVector*)RefData)->X = Stack.ReadFloat();
	((FVector*)RefData)->Y = Stack.ReadFloat();
	((FVector*)RefData)->Z = Stack.ReadFloat();
}
Add_VM_FUNCTION(VMC_VectorConst)

BT_VM_FUNCTION(UBTScriptObject::VMC_RotationConst)
{
	((FRotator*)RefData)->Pitch = Stack.ReadFloat();
	((FRotator*)RefData)->Yaw = Stack.ReadFloat();
	((FRotator*)RefData)->Roll = Stack.ReadFloat();
}
Add_VM_FUNCTION(VMC_RotationConst)

BT_VM_FUNCTION(UBTScriptObject::VMC_NameConst)
{
	*(FName*)RefData = Stack.ReadName();
}
Add_VM_FUNCTION(VMC_NameConst)

BT_VM_FUNCTION(UBTScriptObject::VMC_ObjectConst)
{
	*(UObject**)RefData = (UObject*)Stack.ReadObject();
}
Add_VM_FUNCTION(VMC_ObjectConst)

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif