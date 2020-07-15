#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditor/Core/Architect/VariableNode.h"
#include "BoolNode.generated.h"

UCLASS()
class UBoolNode :public UVariableNode
{
	GENERATED_BODY()

public:
	UBoolNode();

	virtual void AllocateDefaultPins() override;

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent);
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variable)
	FName VariableName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Instance)
	bool Value;
};