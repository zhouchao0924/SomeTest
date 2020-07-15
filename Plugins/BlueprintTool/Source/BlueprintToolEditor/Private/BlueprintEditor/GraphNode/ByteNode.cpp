#include "BlueprintEditor/GraphNode/ByteNode.h"
#include "Type/BTEType.h"

VariableIndex(Byte)

UByteNode::UByteNode()
{
	VariableName = *FString::Printf(TEXT("%s_%i"), *GetClass()->GetName(), IncreaseProgressively(Byte));
}

void UByteNode::AllocateDefaultPins()
{
	CreatePin(EEdGraphPinDirection::EGPD_Output, FPC_Public::PC_Byte, FName(), VariableName);
}

#if WITH_EDITOR
void UByteNode::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
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
