#include "BlueprintEditor/GraphNode/EventNode.h"
#include "Type/BTEType.h"

void UEventNode::AllocateDefaultPins()
{
	if (Function && Function->GetMetaData("CodeType") == "Event")
	{
		CreatePin(EGPD_Output, FPC_Public::PC_Exec, FName(), FPC_Public::PC_Exec);

		UField *Prop = Function->Children;
		while (Prop != NULL)
		{
			UProperty *MyProperty = Cast<UProperty>(Prop);
			CreatePinType(MyProperty, EGPD_Output);
			Prop = Prop->Next;
		}
	}
}

#if WITH_EDITOR
void UEventNode::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}
#endif
