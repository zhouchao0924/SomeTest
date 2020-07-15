#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditor/Core/Architect/VariableNode.h"
#include "StringNode.generated.h"

UCLASS()
class UStringNode :public UVariableNode
{
	GENERATED_BODY()

public:
	UStringNode();

	virtual void AllocateDefaultPins() override;
#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent);
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variable)
	FName VariableName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Instance)
	FString Value;
};