#include "BlueprintEditor/Core/Architect/BPArchitectEdGraph.h"
#include "BlueprintEditor/Core/Architect/PBToolSchema.h"
#include "BlueprintEditor/GraphNode/ObjectNode.h"

UBPArchitectEdGraph::UBPArchitectEdGraph(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	Schema = UPBToolSchema::StaticClass();
}

void UBPArchitectEdGraph::InitializeGraph()
{

}

UEdGraphNode* UBPArchitectEdGraph::CreateNewNode(UObject* AssetObject, const FVector2D& Location)
{
	UClass* AssetClass = AssetObject->GetClass();
	UObjectNode* MeshNode = CreateNewNode<UObjectNode>(Location);
	MeshNode->SetValue(AssetObject);

	return MeshNode;
}
