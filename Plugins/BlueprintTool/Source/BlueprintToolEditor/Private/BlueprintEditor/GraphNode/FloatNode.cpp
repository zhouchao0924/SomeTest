#include "BlueprintEditor/GraphNode/FloatNode.h"
#include "Type/BTEType.h"

VariableIndex(float)

UFloatNode::UFloatNode()
{
	VariableName = *FString::Printf(TEXT("%s_%i"), *GetClass()->GetName(), IncreaseProgressively(float));
}

void UFloatNode::AllocateDefaultPins()
{
	CreatePin(EEdGraphPinDirection::EGPD_Output, FPC_Public::PC_Float, FName(), VariableName);
}

#if WITH_EDITOR
void UFloatNode::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName() == "VariableName")
	{
		Pins[0]->PinName = VariableName;
	}
	else if (PropertyChangedEvent.GetPropertyName() == "Value")
	{
		Pins[0]->DefaultValue = FString::SanitizeFloat(Value);
	}

	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}
#endif
