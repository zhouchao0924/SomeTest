#include "BlueprintEditor/GraphNode/TextNode.h"
#include "Type/BTEType.h"

VariableIndex(FText)

UTextNode::UTextNode()
{
	VariableName = *FString::Printf(TEXT("%s_%i"), *GetClass()->GetName(), IncreaseProgressively(FText));
}

void UTextNode::AllocateDefaultPins()
{
	CreatePin(EEdGraphPinDirection::EGPD_Output, FPC_Public::PC_Text, FName(), VariableName);
}

#if WITH_EDITOR
void UTextNode::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName() == "VariableName")
	{
		Pins[0]->PinName = VariableName;
	}
	else if (PropertyChangedEvent.GetPropertyName() == "Value")
	{
		Pins[0]->DefaultTextValue = Value;
	}

	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}
#endif
