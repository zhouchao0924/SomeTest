#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditor/Core/Architect/K3Node.h"
#include "EventNode.generated.h"

UCLASS()
class UEventNode : public UK3Node
{
	GENERATED_BODY()

public:

	virtual void AllocateDefaultPins() override;

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent);
#endif
};