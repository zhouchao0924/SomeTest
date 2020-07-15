#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditor/Core/Architect/K3Node.h"
#include "Details/Blueprint/DescriptionBPTool.h"
#include "BoardGrapNode.generated.h"

UCLASS()
class UFunctionGrapNode: public UK3Node
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = K3Node)
	FTransform BoardGrapNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Description)
	FDescriptionBPTool DescriptionBPTool;
};