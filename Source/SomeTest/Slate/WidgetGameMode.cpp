// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetGameMode.h"
#include "MainMenuHUD.h"

AWidgetGameMode::AWidgetGameMode()
{
	HUDClass = AMainMenuHUD::StaticClass();
}
