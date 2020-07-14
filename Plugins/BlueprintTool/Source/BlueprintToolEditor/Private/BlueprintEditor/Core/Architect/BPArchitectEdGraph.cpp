#include "BlueprintEditor/Core/Architect/BPArchitectEdGraph.h"
#include "BlueprintEditor/Core/Architect/PBToolSchema.h"

UBPArchitectEdGraph::UBPArchitectEdGraph(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	Schema = UPBToolSchema::StaticClass();
}

void UBPArchitectEdGraph::InitializeGraph()
{

}
