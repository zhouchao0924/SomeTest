#pragma once

#include "CoreMinimal.h"
#include "DescriptionBPTool.generated.h"

USTRUCT(BlueprintType)
struct FDescriptionBPTool
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Transform")
	FTransform MeshTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor")
	AActor *MeshActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hello")
	float A;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hello", Meta = (HelloWorld = "Hide"))
	float B;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hello")
	float C;
};