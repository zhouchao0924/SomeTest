// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/UI_ArchivesWidget.h"
#include "SimpleArchivesType.h"
#include "Data/SimpleArchivesList.h"
#include "UI_ArchivesSystem.generated.h"



class UButton;
class UUI_ArchivesBar;
class UScrollBox;
class ISimpleArchivesInterface;

/**
 * 
 */
UCLASS()
class SIMPLEARCHIVES_API UUI_ArchivesSystem : public UUI_ArchivesWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
		UScrollBox *SaveSlotList;

	UPROPERTY(meta = (BindWidget))
		UButton *LoadGameButton;

	UPROPERTY(meta = (BindWidget))
		UButton *SaveGameButton;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUI_ArchivesBar> ArchivesBarClass;
public:
	void InitArchivesSystem(EArchivesState ArchivesState);

	UFUNCTION()
		void LoadGame();

	UFUNCTION()
		void SaveGame();

	virtual void NativeConstruct()override;
	virtual void NativeDestruct() override;

	//ºô½Ð´°¿Ú
	void BindWindows(TFunction<void(FSimpleDelegate)>);

	bool GetArchivesBarArray(TArray<UUI_ArchivesBar*> &InArchivesBars);
protected:

	void UpdateArchivesSlot();

	void CallAllCkeckBox(UUI_ArchivesBar* OwnerArchivesBar);

	void ResetArchivesBar(UUI_ArchivesBar* InArchivesBar, const FSaveSlot *InData);

private:
	void CallAllArchivesBar(TFunction<void(UUI_ArchivesBar* InArchivesBar)>);
	void CallAllArchivesBarBreak(TFunction<bool(UUI_ArchivesBar* InArchivesBar)>);

};
