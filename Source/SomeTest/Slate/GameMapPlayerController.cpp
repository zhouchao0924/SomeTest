// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMapPlayerController.h"
#include "GameCharacter.h"

AGameMapPlayerController::AGameMapPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
}

void AGameMapPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Home", IE_Pressed, this, &AGameMapPlayerController::HomeButtomPressed);
	InputComponent->BindAction("End", IE_Pressed, this, &AGameMapPlayerController::EndButtomPressed);
	InputComponent->BindAction("Pgup", IE_Pressed, this, &AGameMapPlayerController::PgupButtomPressed);
	InputComponent->BindAction("Pgdn", IE_Pressed, this, &AGameMapPlayerController::PgdnButtomPressed);
}

void AGameMapPlayerController::HomeButtomPressed()
{
	Cast<AGameCharacter>(GetCharacter())->AddHealth();
}

void AGameMapPlayerController::EndButtomPressed()
{
	Cast<AGameCharacter>(GetCharacter())->SubHealth();
}

void AGameMapPlayerController::PgupButtomPressed()
{
	Cast<AGameCharacter>(GetCharacter())->AddScore();
}

void AGameMapPlayerController::PgdnButtomPressed()
{
	Cast<AGameCharacter>(GetCharacter())->SubScore();
}
