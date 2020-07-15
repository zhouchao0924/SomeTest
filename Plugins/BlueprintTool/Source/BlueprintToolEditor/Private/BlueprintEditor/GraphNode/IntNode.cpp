#include "BlueprintEditor/GraphNode/IntNode.h"
#include "Type/BTEType.h"

VariableIndex(int32)

UIntNode::UIntNode()
{
	VariableName = *FString::Printf(TEXT("%s_%i"), *GetClass()->GetName(), IncreaseProgressively(int32));
}

void UIntNode::AllocateDefaultPins()
{
	CreatePin(EEdGraphPinDirection::EGPD_Output, FPC_Public::PC_Int, FName(), VariableName);
}

#if WITH_EDITOR
void UIntNode::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
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
