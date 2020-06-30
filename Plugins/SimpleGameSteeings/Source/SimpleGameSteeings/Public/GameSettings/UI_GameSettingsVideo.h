// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UI_SimpleGameSettingsCore.h"
#include "UI_GameSettingsVideo.generated.h"

class UCheckBox;
class UComboBoxString;
class USlider;
class UTextBlock;
class USpinBox;
/**
 * 
 */
UCLASS()
class SIMPLEGAMESETTINGS_API UUI_GameSettingsVideo : public UUI_SimpleGameSettingsCore
{
	GENERATED_BODY()

		UPROPERTY(meta = (BindWidget))
		USpinBox *DisplayNitsSpinBox;

	UPROPERTY(meta = (BindWidget))
		USpinBox *FrameRateLimitSpinBox;

	UPROPERTY(meta = (BindWidget))
		UCheckBox* FullScreenCheckBox;

	UPROPERTY(meta = (BindWidget))
		UCheckBox* WindowScreenCheckBox;

	UPROPERTY(meta = (BindWidget))
		UCheckBox* SupportPhysXCheckBox;

	UPROPERTY(meta = (BindWidget))
		UCheckBox* VSyncCheckBox;

	UPROPERTY(meta = (BindWidget))
		UCheckBox* HDRDisplayCheckBox;

	UPROPERTY(meta = (BindWidget))
		UComboBoxString*  ResolutionBoxString;

	UPROPERTY(meta = (BindWidget))
		UComboBoxString*  LanguageString;

	UPROPERTY(meta = (BindWidget))
		USlider* AntiAliasingSlider;

	UPROPERTY(meta = (BindWidget))
		USlider* ShadowQualitySlider;

	UPROPERTY(meta = (BindWidget))
		USlider* TextureQualitySlider;

	UPROPERTY(meta = (BindWidget))
		USlider* PostProcessingSlider;

	UPROPERTY(meta = (BindWidget))
		USlider* EffectsSlider;

	UPROPERTY(meta = (BindWidget))
		USlider* FoliageSlider;

	UPROPERTY(meta = (BindWidget))
		USlider* ViewDistanceSlider;

	UPROPERTY(meta = (BindWidget))
		USlider* ShadingSlider;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* AntiAliasingText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ShadowQualityText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* TextureQualityText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* PostProcessingText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* EffectsText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* FoliageText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ViewDistanceText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ShadingText;

public:
	virtual void NativeConstruct()override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual	void SaveSettings();

	virtual	void LoadSettings();

	UFUNCTION()
		void FullScreenCheckClickedBox(bool ClickedWidget);

	UFUNCTION()
		void WindowScreenCheckClickedBox(bool ClickedWidget);

	UFUNCTION()
		void SupportPhysXCheckClickedBox(bool ClickedWidget);

	UFUNCTION()
		void VSyncCheckClickedBox(bool ClickedWidget);

	UFUNCTION()
		void EnableHDRDisplayOutput(bool ClickedWidget);

	UFUNCTION()
		void SetFrameRateLimit(float NewValue);
protected:

	virtual void ChangedValue(float InValue);

	virtual void SelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
};
