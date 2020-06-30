// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UI_SimpleGameSettingsCore.h"
#include "UI_GameSetingsAudio.generated.h"

class USlider;
class UButton;
class USoundBase;
/**
 * 
 */
UCLASS()
class SIMPLEGAMESETTINGS_API UUI_GameSetingsAudio : public UUI_SimpleGameSettingsCore
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
		UButton* GameSoundButton;

	UPROPERTY(meta = (BindWidget))
		UButton* UISoundButton;

	UPROPERTY(meta = (BindWidget))
		UButton* BackgroundSoundButton;

	UPROPERTY(meta = (BindWidget))
		USlider* AudioQualityLevelSlider;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* AudioQualityLevelText;

	UPROPERTY(meta = (BindWidget))
		USlider* BackgroundSoundSlider;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* BackgroundSoundText;

	UPROPERTY(meta = (BindWidget))
		USlider* GameSoundSlider;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* GameSoundText;

	UPROPERTY(meta = (BindWidget))
		USlider* UISoundSlider;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* UISoundText;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		USoundBase *BackgroundSoundTestObject;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		USoundBase *GameSoundTestObject;
public:

	virtual	void SaveSettings();
	virtual	void LoadSettings();

	virtual void NativeConstruct()override;

	UFUNCTION()
		void SpawnGameSoundForTest();

	UFUNCTION()
		void SpawnUISoundForTest();

	UFUNCTION()
		void SpawnBackgroundSoundForTest();

	void DestroyBackgroundSound();
protected:

	virtual void ChangedValue(float InValue);
};
