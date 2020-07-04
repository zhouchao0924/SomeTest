#include "TestNode_HelloWorld.h"

UTestNode_HelloWorld::UTestNode_HelloWorld()
{

}

void UTestNode_HelloWorld::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "HelloNodeInput", FName(), TEXT("In"));
	CreatePin(EGPD_Input, "HelloNodeInput", FName(), TEXT("In"));
	CreatePin(EGPD_Input, "HelloNodeInput", FName(), TEXT("In"));
	CreatePin(EGPD_Input, "HelloNodeInput", FName(), TEXT("In"));
	CreatePin(EGPD_Input, "HelloNodeInput", FName(), TEXT("In"));
	CreatePin(EGPD_Input, "HelloNodeInput", FName(), TEXT("In"));
	CreatePin(EGPD_Output, "HelloNodeInput", FName(), TEXT("Out"));
	CreatePin(EGPD_Output, "HelloNodeInput", FName(), TEXT("Out"));
	CreatePin(EGPD_Output, "HelloNodeInput", FName(), TEXT("Out"));
	CreatePin(EGPD_Output, "HelloNodeInput", FName(), TEXT("Out"));
	CreatePin(EGPD_Output, "HelloNodeInput", FName(), TEXT("Out"));
	CreatePin(EGPD_Output, "HelloNodeInput", FName(), TEXT("Out"));
}

FText UTestNode_HelloWorld::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("HelloWorld");
}
