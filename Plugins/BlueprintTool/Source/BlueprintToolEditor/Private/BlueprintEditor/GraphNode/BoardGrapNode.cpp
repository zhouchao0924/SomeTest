#include "BlueprintEditor/GraphNode/BoardGrapNode.h"

void UBoardGrapNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "HelloNodeInput", FName(), TEXT("In"));
	CreatePin(EGPD_Input, "HelloNodeInput2", FName(), TEXT("In"));
	CreatePin(EGPD_Input, "HelloNodeInput3", FName(), TEXT("In"));
	CreatePin(EGPD_Input, "HelloNodeInput4", FName(), TEXT("In"));

	CreatePin(EGPD_Output, "HelloNodeInput5", FName(), TEXT("Out"));
	CreatePin(EGPD_Output, "HelloNodeInput6", FName(), TEXT("Out"));
	CreatePin(EGPD_Output, "HelloNodeInput7", FName(), TEXT("Out"));
	CreatePin(EGPD_Output, "HelloNodeInput8", FName(), TEXT("Out"));
}

FText UBoardGrapNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("As Test Node");
}

