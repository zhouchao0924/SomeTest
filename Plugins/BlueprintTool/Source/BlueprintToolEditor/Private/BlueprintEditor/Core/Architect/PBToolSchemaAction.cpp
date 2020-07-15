#include "BlueprintEditor/Core/Architect/PBToolSchemaAction.h"
#include "BlueprintEditor/Core/Architect/K3Node.h"

#define LOCTEXT_NAMESPACE "PBToolSchemaAction"

UEdGraphNode* FPBToolSchemaAction::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode /*= true*/)
{
	UEdGraphNode *UEdResultNode = nullptr;

	if (K3Node != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("FF", "Hell:K3Node"));

		ParentGraph->Modify();

		if (FromPin != nullptr)
		{
			FromPin->Modify();
		}

		K3Node->Rename(nullptr, ParentGraph);
		ParentGraph->AddNode(K3Node, true, bSelectNewNode);

		K3Node->CreateNewGuid();
		K3Node->PostPlacedNewNode();
		K3Node->AllocateDefaultPins();
		K3Node->AutowireNewNode(FromPin);

		K3Node->NodePosX = Location.X;
		K3Node->NodePosY = Location.Y;

		K3Node->SetFlags(RF_Transactional);

		UEdResultNode = K3Node;
	}

	return UEdResultNode;
}

#undef LOCTEXT_NAMESPACE