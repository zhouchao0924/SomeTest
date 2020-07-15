#include "BlueprintEditor/GraphNode/StructVectorNode.h"
#include "Type/BTEType.h"

VariableIndex(FVector)

UStructVectorNode::UStructVectorNode()
{
	VariableName = *FString::Printf(TEXT("%s_%i"), *GetClass()->GetName(), IncreaseProgressively(FVector));
}

void UStructVectorNode::AllocateDefaultPins()
{
	UScriptStruct* VectorStruct = TBaseStructure<FVector>::Get();
	CreatePin(EEdGraphPinDirection::EGPD_Output, FPC_Public::PC_Struct, VectorStruct->GetFName(), VectorStruct, TEXT("Vector"));
}

#if WITH_EDITOR
void UStructVectorNode::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
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
