// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class SOMETEST_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()
	// Initializes the Slate UI and adds it as widget content to the game viewport.
	virtual void PostInitializeComponents() override;

	// Reference to the Main Menu Slate UI.
	TSharedPtr<class SMainMenuUI> MainMenuUI;

public:
	// Called by SMainMenu whenever the Play Game! button has been clicked.
	UFUNCTION(BlueprintImplementableEvent, Category = "Menus|Main Menu")
		void PlayGameClicked();

	// Called by SMainMenu whenever the Quit Game button has been clicked.
	UFUNCTION(BlueprintImplementableEvent, Category = "Menus|Main Menu")
		void QuitGameClicked();
	
};
