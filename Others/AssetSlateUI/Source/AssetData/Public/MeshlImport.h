
#pragma once

#include "CoreMinimal.h"
#include "MeshlImport.generated.h"

USTRUCT(BlueprintType)
struct FMeshlImport
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "MeshSavePaths")
	FText MeshSavePaths;
};