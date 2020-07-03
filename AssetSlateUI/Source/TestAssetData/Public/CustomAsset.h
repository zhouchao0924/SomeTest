// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CustomAsset.generated.h"


/**
 * 
 */
UCLASS()
class TESTASSETDATA_API UCustomAsset : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MyDataAsset")
		FText MyText;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MyDataAsset")
		float FloatValue;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MyDataAsset")
		int32 Int32Value;

	
};
