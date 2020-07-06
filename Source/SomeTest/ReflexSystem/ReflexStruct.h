#pragma once

#include "ReflexStruct.generated.h"

class UBoxComponent;

USTRUCT(Blueprintable)
struct FReflexStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "BoxComponent")
		TArray<UBoxComponent*> ArrayComponent;

	UPROPERTY(EditDefaultsOnly, Category = "BoxComponent")
		TSet<AActor*> SetMyComponent;

	UPROPERTY(BlueprintReadOnly, Category = "TTT")
		float VlaueC;

	UPROPERTY(BlueprintReadOnly, Category = "TTT")
		FString VlaueString;
public:
};