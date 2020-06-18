// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameMapGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SOMETEST_API AGameMapGameMode : public AGameMode
{
	GENERATED_BODY()
public:

	AGameMapGameMode();

	/**
	 * Retrieves the current Score from the game mode.
	 **/
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Score")
		int32 GetScore();

	/**
	 * Adds to the game score.
	 **/
	UFUNCTION(BlueprintCallable, Category = "Score")
		void AddPoints(int32 value);

	/**
	 * Removes from the game score.
	 **/
	UFUNCTION(BlueprintCallable, Category = "Score")
		void DeductPoints(int32 value);

private:

	/**
	 * Stores the current score.
	 **/
	int32 CurrentScore;
};
