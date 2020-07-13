#include "Blueprint/Architect/BPArchitectEdGraph.h"
#include "Blueprint/Architect/BPToolSchema.h"

UBPArchitectEdGraph::UBPArchitectEdGraph(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Schema = UBPToolSchema::StaticClass();
}

void UBPArchitectEdGraph::InitializeGraph()
{

}
