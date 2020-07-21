// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SimpleUEViewConfigurationInfor.generated.h"


/**
 * 
 */
UCLASS(config = UEViewConfigurationInfor)
class SIMPLEUNREALPAKVIEW_API USimpleUEViewConfigurationInfor : public UObject
{
	GENERATED_BODY()
	
public:
	USimpleUEViewConfigurationInfor();

	UPROPERTY(config, EditAnywhere, Category = UEViewInfo, meta = (ToolTip = "xxx."))
	FFilePath PakPath;

	UPROPERTY(config, EditAnywhere, Category = UEViewInfo, meta = (ToolTip = "xxx."))
	FString AES;

	UPROPERTY(config, EditAnywhere, Category = UEViewInfo, meta = (ToolTip = "xxx."))
	FString MountPoint;
};
