// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuHUD.h"
#include "MainMenuUI.h"
#include "Widgets/SWeakWidget.h"

void AMainMenuHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SAssignNew(MainMenuUI, SMainMenuUI).MainMenuHUD(this);

	if (GEngine->IsValidLowLevel())
	{
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(MainMenuUI.ToSharedRef()));
	}
}
