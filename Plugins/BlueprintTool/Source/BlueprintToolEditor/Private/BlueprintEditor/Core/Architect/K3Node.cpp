#include "BlueprintEditor/Core/Architect/K3Node.h"
#include "Type/BTEType.h"
#include "UObject/TextProperty.h"

#define ResetDescribe \
UField *Prop = Function->Children;\
while (Prop != NULL)\
{\
	UProperty *MyProperty = Cast<UProperty>(Prop);\
	if (MyProperty->PropertyFlags & CPF_OutParm || MyProperty->PropertyFlags & CPF_ReturnParm)\
	{\
		CreatePinType(MyProperty, EGPD_Output);\
	}\
	else if (MyProperty->PropertyFlags & CPF_Parm)\
	{\
		CreatePinType(MyProperty, EGPD_Input);\
	}\
	Prop = Prop->Next;\
}

void UK3Node::AllocateDefaultPins()
{
	if (Function)
	{
		if (Function->GetMetaData("CodeType") == "Describe")
		{
			CreatePin(EGPD_Output, FPC_Public::PC_Exec, FName(), FPC_Public::PC_Exec);
			CreatePin(EGPD_Input, FPC_Public::PC_Exec, FName(), FPC_Public::PC_Exec);

			ResetDescribe
		}
		else if (Function->GetMetaData("CodeType") == "PureDescribe")
		{
			ResetDescribe
		}
	}
	else if (Property)
	{
		CreatePinType(Property, EGPD_Output);
	}
}

FText UK3Node::GetNodeTitle()
{
	if (!NodeTitle.IsEmpty())
	{
		return NodeTitle;
	}

	if (Function)
	{
		NodeTitle = FText::FromString(Function->GetName());
	}
	else if (Property)
	{
		NodeTitle = FText::FromString(Property->GetName());
	}

	return NodeTitle;
}

void UK3Node::CreatePinType(UProperty *MyProperty, EEdGraphPinDirection EdGraphPinDirection)
{
	if (MyProperty->IsA(UBoolProperty::StaticClass()))
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Boolean, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA(UIntProperty::StaticClass()))
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Int, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA(UByteProperty::StaticClass()))
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Byte, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA(UFloatProperty::StaticClass()))
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Float, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA(UClassProperty::StaticClass()))
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Class, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA(UNameProperty::StaticClass()))
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Name, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA(UDelegateProperty::StaticClass()))
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Delegate, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA(UObjectProperty::StaticClass()))
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Object, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA(UStrProperty::StaticClass()))
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_String, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA(UTextProperty::StaticClass()))
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Text, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA(UStructProperty::StaticClass()))
	{
		UScriptStruct* Struct = Cast<UStructProperty>(MyProperty)->Struct;

		//CreatePin(const EEdGraphPinDirection Dir, const FName PinCategory, const FName PinSubCategory, UObject* PinSubCategoryObject, const FName PinName, const FCreatePinParams& PinParams)
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Struct, Struct->GetFName(), Struct, MyProperty->GetFName());
	}
	else if (MyProperty->IsA(UEnumProperty::StaticClass()))
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Enum, FName(), MyProperty->GetFName());
	}
	else if (MyProperty->IsA(UInterfaceProperty::StaticClass()))
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_Interface, FName(), MyProperty->GetFName());
	}
	else
	{
		CreatePin(EdGraphPinDirection, FPC_Public::PC_DefaultBT, FName(), MyProperty->GetFName());
	}
}

bool UK3Node::IsPure()
{
	for (UEdGraphPin *NewPin : Pins)
	{
		if (NewPin->PinType.PinCategory == FPC_Public::PC_Exec)
		{
			return false;
		}
	}

	return true;
}

