#include "BlueprintEditor/GraphNode/StringNode.h"
#include "Type/BTEType.h"

VariableIndex(FString)

UStringNode::UStringNode()
{
	VariableName = *FString::Printf(TEXT("%s_%i"), *GetClass()->GetName(), IncreaseProgressively(FString));
}

void UStringNode::AllocateDefaultPins()
{
	CreatePin(EEdGraphPinDirection::EGPD_Output, FPC_Public::PC_String, FName(), VariableName);
}

#if WITH_EDITOR
void UStringNode::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName() == "VariableName")
	{
		Pins[0]->PinName = VariableName;
	}
	else if (PropertyChangedEvent.GetPropertyName() == "Value")
	{
		Pins[0]->DefaultValue = Value;
	}

	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}
#endif
