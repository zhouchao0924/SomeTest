#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditor/Core/Architect/VariableNode.h"
#include "IntNode.generated.h"

UCLASS()
class UIntNode :public UVariableNode
{
	GENERATED_BODY()

public:
	UIntNode();

	virtual void AllocateDefaultPins() override;
#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent);
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variable)
	FName VariableName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Instance)
	int32 Value;
};