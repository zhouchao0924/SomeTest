#include "BlueprintEditor/GraphNode/NameNode.h"
#include "Type/BTEType.h"

VariableIndex(FName)

UNameNode::UNameNode()
{
	VariableName = *FString::Printf(TEXT("%s_%i"), *GetClass()->GetName(), IncreaseProgressively(FName));
}

void UNameNode::AllocateDefaultPins()
{
	CreatePin(EEdGraphPinDirection::EGPD_Output, FPC_Public::PC_Name, FName(), VariableName);
}

#if WITH_EDITOR
void UNameNode::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.GetPropertyName() == "VariableName")
	{
		Pins[0]->PinName = VariableName;
	}
	else if (PropertyChangedEvent.GetPropertyName() == "Value")
	{
		Pins[0]->DefaultValue = Value.ToString();
	}

	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}
#endif
