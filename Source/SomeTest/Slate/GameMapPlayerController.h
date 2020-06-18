// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameMapPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SOMETEST_API AGameMapPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGameMapPlayerController();

	virtual void SetupInputComponent() override;

	void HomeButtomPressed();
	void EndButtomPressed();
	void PgupButtomPressed();
	void PgdnButtomPressed();
};
