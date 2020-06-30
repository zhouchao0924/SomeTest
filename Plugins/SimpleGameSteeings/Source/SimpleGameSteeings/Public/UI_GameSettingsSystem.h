// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleGameSettingsType.h"
#include "Core/UI_SimpleGameSettingsCore.h"
#include "UI_GameSettingsSystem.generated.h"


class UCheckBox;
class UButton;
class UWidgetSwitcher;
class UUI_GameSettingsVideo;
class UUI_GameSetingsAudio;
class UUI_GameSettingsGameSetting;
/**
 * 
 */
UCLASS()
class SIMPLEGAMESETTINGS_API UUI_GameSettingsSystem : public UUI_SimpleGameSettingsCore
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
		UCheckBox* AudioSettingBox;

	UPROPERTY(meta = (BindWidget))
		UCheckBox* VideoSettingsBox;

	UPROPERTY(meta = (BindWidget))
		UCheckBox* GameSettingsBox;

	UPROPERTY(meta = (BindWidget))
		UButton* SaveButton;

	UPROPERTY(meta = (BindWidget))
		UButton* RestoreDefaultButton;

	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* SettingsListWitcher;

	UPROPERTY(meta = (BindWidget))
		UUI_GameSettingsVideo* GameSettingsVideo;

	UPROPERTY(meta = (BindWidget))
		UUI_GameSetingsAudio* GameSettingsAudio;

	UPROPERTY(meta = (BindWidget))
		UUI_GameSettingsGameSetting* GameSettingsGameSetting;
public:
	virtual void NativeConstruct()override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
		void AudioSettingCheckBox(bool ClickedWidget);

	UFUNCTION()
		void VideoSettingsCheckBox(bool ClickedWidget);

	UFUNCTION()
		void GameSettingsCheckBox(bool ClickedWidget);

	virtual	void SaveSettings();
	virtual	void LoadSettings();

	UFUNCTION()
		void RestoreDefaultSettings();

private:

	void SetSettingState(EGameSettingsType Type);
};
