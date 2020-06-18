// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCharacter.h"
#include "Engine/World.h"

// Sets default values
AGameCharacter::AGameCharacter()
{
	Health = 10;
}

void AGameCharacter::AddHealth()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("AddHealth"));
	}
	Health++;
}

void AGameCharacter::SubHealth()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("SubHealth"));
	}
	
	Health = --Health <= 0 ? 0 : Health;
}

void AGameCharacter::AddScore()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("AddScore"));
	}
	
	GetGameMode()->AddPoints(10);
}

void AGameCharacter::SubScore()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("SubScore"));
	}

	GetGameMode()->DeductPoints(10);
}
