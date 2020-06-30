// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UI_ArchivesWidget.h"
#include "SimpleArchivesType.h"
#include "UI_ArchivesBar.generated.h"


class UImage;
class UTextBlock;
class UCheckBox;
class UButton;
struct FSaveSlot;
/**
 * 
 */
UCLASS()
class SIMPLEARCHIVES_API UUI_ArchivesBar : public UUI_ArchivesWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UImage *GameThumbnail;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* SaveGameDateBlock;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* ChapterNameBlock;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* SaveNumberBlock;

	UPROPERTY(meta = (BindWidget))
		UCheckBox* CheckBoxButton;

	UPROPERTY(meta = (BindWidget))
		UButton* DeleteMyDataButton;

public:
	FSimpleArchivesSlotDelegate CallNewWindowsDelegate;

public:
	UUI_ArchivesBar(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	//Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void Update();

	UFUNCTION()
		void ClickedCheckBox(bool ClickedWidget);

	UFUNCTION()
		void OnClickedWidgetDelete();

	void SetGameThumbnail(UTexture2D *InImage);

	void SetSaveGameDate(const FText &InText);

	void SetChapterName(const FText &InText);

	void SetCheckBoxState(ECheckBoxState InState);

	UFUNCTION()
		void ClearSlotData();

	FSaveSlot *GetSaveSlot();

public:
	FSimpleDelegate ReverseProxy;

public:
	int32 SlotIndex;
	
};
