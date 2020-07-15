#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditor/Core/Architect/VariableNode.h"
#include "StructVectorNode.generated.h"

UCLASS()
class UStructVectorNode :public UVariableNode
{
	GENERATED_BODY()

public:
	UStructVectorNode();

	virtual void AllocateDefaultPins() override;
#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent);
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variable)
	FName VariableName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Instance)
	FVector Value;
};