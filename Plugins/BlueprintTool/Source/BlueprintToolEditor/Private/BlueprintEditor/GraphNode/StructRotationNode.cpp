#include "BlueprintEditor/GraphNode/StructRotationNode.h"
#include "Type/BTEType.h"

VariableIndex(FRotator)

UStructRotationNode::UStructRotationNode()
{
	VariableName = *FString::Printf(TEXT("%s_%i"), *GetClass()->GetName(), IncreaseProgressively(FRotator));
}

void UStructRotationNode::AllocateDefaultPins()
{
	UScriptStruct* RotatorStruct = TBaseStructure<FRotator>::Get();
	CreatePin(EEdGraphPinDirection::EGPD_Output, FPC_Public::PC_Struct, RotatorStruct->GetFName(), RotatorStruct, TEXT("Rotator"));
}

#if WITH_EDITOR
void UStructRotationNode::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
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
