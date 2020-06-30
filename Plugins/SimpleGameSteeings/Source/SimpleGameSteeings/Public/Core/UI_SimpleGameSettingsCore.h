// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_SimpleGameSettingsCore.generated.h"

class USlider;
class UTextBlock;
class UComboBoxString;
/**
 * 
 */
UCLASS()
class SIMPLEGAMESETTINGS_API UUI_SimpleGameSettingsCore : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
		virtual	void SaveSettings() {}

	UFUNCTION()
		virtual	void LoadSettings() {}
protected:
	void UpdateAttibeLevel(USlider *NewSlider, UTextBlock *NewBlock);
	void UpdateAttibe(USlider *NewSlider, UTextBlock *NewBlock);

	UFUNCTION()
		virtual void ChangedValue(float InValue) {}

	UFUNCTION()
		virtual void SelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType) {}

	virtual void BindChangedValue(USlider *NewSlider);
	virtual void BindSelectionChanged(UComboBoxString *BoxString);

	void SetSettingsVlaue(USlider* InSlider, UTextBlock *NewBlock, TFunction<void(float InValue)> InFunc);
	void LoadSettingsVlaue(USlider* InSlider, UTextBlock *NewBlock, TFunction<float()> InFunc);
	void SetSettingsLevel(USlider* InSlider, UTextBlock *NewBlock, TFunction<void(float InLevel)> InFunc);
	void LoadSettingsLevel(USlider* InSlider, UTextBlock *NewBlock, TFunction<float()> InFunc);
	void LoadBoxString(UComboBoxString* InBoxString, TFunction<FString()> InFunc);
};
