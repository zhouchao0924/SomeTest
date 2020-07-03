#pragma once

#include "CoreMinimal.h"
#include "StructVariable.generated.h"

USTRUCT(BlueprintType)
struct TESTDETAILS_API FStructVariable
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = Hell)
	FText MyText;
};