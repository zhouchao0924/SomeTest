// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SimpleGameExtraSettings.generated.h"

class USimpleGameUserSettings;
/**
 * 
 */
UCLASS(config = SimpleGameExtraSettings)
class SIMPLEGAMESETTINGSEDITOR_API USimpleGameExtraSettings : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	TSubclassOf<USimpleGameUserSettings> GameUserSettingsClass;
};
