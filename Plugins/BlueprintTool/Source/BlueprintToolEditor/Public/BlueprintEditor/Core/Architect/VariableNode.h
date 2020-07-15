#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditor/Core/Architect/K3Node.h"
#include "VariableNode.generated.h"

UCLASS()
class UVariableNode :public UK3Node
{
	GENERATED_BODY()

public:

	virtual bool IsPure() override;
};