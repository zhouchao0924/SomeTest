// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameMapGameMode.h"
#include "GameCharacter.generated.h"


UCLASS()
class SOMETEST_API AGameCharacter : public ACharacter
{
	GENERATED_BODY()

	AGameCharacter();
public:
	/**
 * Stores the character's current health.
 **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		int32 Health;

	FORCEINLINE AGameMapGameMode* GetGameMode() { return GetWorld() ? GetWorld()->GetAuthGameMode<AGameMapGameMode>() : nullptr; }

	void AddHealth();
	void SubHealth();
	void AddScore();
	void SubScore();
};
