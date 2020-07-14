#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditor/Core/Architect/K3Node.h"
#include "BoardGrapNode.generated.h"

UCLASS()
class UBoardGrapNode : public UK3Node
{
	GENERATED_BODY()

public:
	virtual void AllocateDefaultPins() override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = K3Node)
		FTransform BoardGrapNode;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Description)
//		FDescriptionBPTool DescriptionBPTool;
};