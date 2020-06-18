// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUDUI.h"
#include "GameHUD.generated.h"


/**
 * 
 */
UCLASS()
class SOMETEST_API AGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	AGameHUD();

	/**
	 * Initializes the Slate UI and adds it as a widget to the game viewport.
	 **/
	virtual void PostInitializeComponents() override;

private:

	/**
	 * Reference to the Game HUD UI.
	 **/
	TSharedPtr<class SGameHUDUI> GameHUD;
};
