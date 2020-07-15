#pragma once

#include "CoreMinimal.h"
#include "GraphEditor.h"
#include "K3Node.generated.h"

UCLASS()
class UK3Node :public UEdGraphNode
{
	GENERATED_BODY()

public:
	virtual void AllocateDefaultPins() override;

	FText GetNodeTitle();
	void CreatePinType(UProperty *Type, EEdGraphPinDirection EdGraphPinDirection);
	UPROPERTY()
		UFunction *Function;

	UPROPERTY()
		UProperty *Property;

	UPROPERTY()
		FText NodeTitle;

	virtual bool IsPure();
};