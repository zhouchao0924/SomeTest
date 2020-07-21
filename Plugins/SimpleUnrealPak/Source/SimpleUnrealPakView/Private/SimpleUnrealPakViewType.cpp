#include "SimpleUnrealPakViewType.h"

FSimpleOneParamDelegate SimpleOneParamDelegate;
UStaticMeshComponent *GMeshComponent = nullptr;
UProceduralMeshComponent *GProceduralMeshComponent;
UWorld *ViewProtWorld = nullptr;
TArray<AActor*> Actors;

UWorld *GetWorld()
{
	return ViewProtWorld;
}