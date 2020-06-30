// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UI_SimpleGameSettingsCore.h"
#include "UI_GameSettingsGameSetting.generated.h"

class UInputKeySelectorMapping;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class SIMPLEGAMESETTINGS_API UUI_GameSettingsGameSetting : public UUI_SimpleGameSettingsCore
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UVerticalBox *ActionList;

public:
	virtual void NativeConstruct()override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual	void SaveSettings();
	virtual	void LoadSettings();

	UFUNCTION()
		void ChangeInputKey(FInputChord SelectedKey);

	UFUNCTION()
		void OnSelectingKeyChanged(UInputKeySelectorMapping *InInputKey);
protected:
	void UpdateInputKey();

protected:
	UInputKeySelectorMapping *KeySelectorTmp;
};
