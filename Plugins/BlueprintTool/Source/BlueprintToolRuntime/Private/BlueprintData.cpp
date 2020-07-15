#include "BlueprintData.h"

UBlueprintData::UBlueprintData()
	:EdGraph(nullptr)
{

}

void UBlueprintData::AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector)
{
#if WITH_EDITORONLY_DATA

	Collector.AddReferencedObject(CastChecked<UBlueprintData>(InThis)->EdGraph, CastChecked<UBlueprintData>(InThis));

#endif	// WITH_EDITORONLY_DATA

	Super::AddReferencedObjects(InThis, Collector);
}
