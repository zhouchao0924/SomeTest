// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMapGameMode.h"
#include "GameCharacter.h"
#include "GameHUD.h"
#include <algorithm>
#include "GameMapPlayerController.h"

AGameMapGameMode::AGameMapGameMode()
	: Super(FObjectInitializer::Get())
	, CurrentScore(0)
{
	HUDClass = AGameHUD::StaticClass();
	DefaultPawnClass = AGameCharacter::StaticClass();
	PlayerControllerClass = AGameMapPlayerController::StaticClass();
}

int32 AGameMapGameMode::GetScore()
{
	return CurrentScore;
}

void AGameMapGameMode::AddPoints(int32 value)
{
	if (value > 0)
		CurrentScore += value;
}

void AGameMapGameMode::DeductPoints(int32 value)
{
	if (value > 0)
		CurrentScore = std::max(CurrentScore - value, 0);
}
