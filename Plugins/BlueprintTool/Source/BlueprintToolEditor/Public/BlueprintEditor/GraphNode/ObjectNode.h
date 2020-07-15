#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditor/Core/Architect/VariableNode.h"
#include "ObjectNode.generated.h"

UCLASS()
class UObjectNode :public UVariableNode
{
	GENERATED_BODY()

public:
	UObjectNode();

	virtual void AllocateDefaultPins() override;
#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent);
#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variable)
	FName VariableName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Instance)
	UObject *Value;

	void SetValue(UObject *InValue);

	FSimpleDelegate Update;
};