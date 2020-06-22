// Fill out your copyright notice in the Description page of Project Settings.


#include "TestMacroCharacter.h"

// Sets default values
ATestMacroCharacter::ATestMacroCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestMacroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestMacroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	{
		if (IsShow)
		{
			FString  TempText = "What is this, WTF";
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, TempText);
			}
			UE_LOG(LogTemp, Log, TEXT("This is ExecFunction------"));
		}
	}
}

// Called to bind functionality to input
void ATestMacroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATestMacroCharacter::ExecFunction()
{
	IsShow = !IsShow;
}

