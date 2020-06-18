// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"
#include "Engine/GameViewportClient.h"
#include "GameHUDUI.h"
#include "Widgets/SWeakWidget.h"

AGameHUD::AGameHUD()
	: Super(FObjectInitializer::Get())
{
}

void AGameHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GEngine && GEngine->GameViewport)
	{
		UGameViewportClient* Viewport = GEngine->GameViewport;

		SAssignNew(GameHUD, SGameHUDUI).OwnerHUD(TWeakObjectPtr<AGameHUD>(this));

		Viewport->AddViewportWidgetContent(
			SNew(SWeakWidget).PossiblyNullContent(GameHUD.ToSharedRef())
		);
	}
}