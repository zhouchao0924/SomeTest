#include "BlueprintEditor/GraphNode/BoolNode.h"
#include "Type/BTEType.h"

VariableIndex(bool)

UBoolNode::UBoolNode()
{
	VariableName = *FString::Printf(TEXT("%s_%i"),*GetClass()->GetName(), IncreaseProgressively(bool));
}

void UBoolNode::AllocateDefaultPins()
{
	CreatePin(EEdGraphPinDirection::EGPD_Output, FPC_Public::PC_Boolean, FName(), VariableName);
}

#if WITH_EDITOR
void UBoolNode::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName() == "VariableName")
	{
		Pins[0]->PinName = VariableName;
	}
	else if (PropertyChangedEvent.GetPropertyName() == "Value")
	{
		Pins[0]->DefaultValue = FString::FromInt(Value);
	}

	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}
#endif
