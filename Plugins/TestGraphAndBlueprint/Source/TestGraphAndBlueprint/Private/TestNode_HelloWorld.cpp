#include "TestNode_HelloWorld.h"

UTestNode_HelloWorld::UTestNode_HelloWorld()
{

}

void UTestNode_HelloWorld::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "SGraphPinBool", FName(), TEXT("SGraphPinBool"));
	CreatePin(EGPD_Input, "SGraphPinColor", FName(), TEXT("SGraphPinColor"));
	CreatePin(EGPD_Input, "SGraphPinEnum", FName(), TEXT("SGraphPinEnum"));
	CreatePin(EGPD_Input, "SGraphPinExec", FName(), TEXT("SGraphPinExec"));
	CreatePin(EGPD_Input, "SGraphPinNameList", FName(), TEXT("SGraphPinNameList"));
	CreatePin(EGPD_Input, "SGraphPinObject", FName(), TEXT("SGraphPinObject"));
	CreatePin(EGPD_Input, "SGraphPinString", FName(), TEXT("SGraphPinString"));
	CreatePin(EGPD_Input, "SGraphPinVector2D", FName(), TEXT("SGraphPinVector2D"));
	CreatePin(EGPD_Input, "SGraphPinVector4", FName(), TEXT("SGraphPinVector4"));
	CreatePin(EGPD_Input, "SGraphPinInteger", FName(), TEXT("SGraphPinInteger"));

	CreatePin(EGPD_Output, "SMyGraphPin", FName(), TEXT("SMyGraphPin"));
	CreatePin(EGPD_Output, "SMyGraphPin1", FName(), TEXT("SMyGraphPin"));
	CreatePin(EGPD_Output, "SMyGraphPin2", FName(), TEXT("SMyGraphPin"));
	CreatePin(EGPD_Output, "SMyGraphPin3", FName(), TEXT("SMyGraphPin"));
	CreatePin(EGPD_Output, "SMyGraphPin4", FName(), TEXT("SMyGraphPin"));
}

FText UTestNode_HelloWorld::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("HelloWorld");
}
