// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleMoveScreenFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESCREENMOVE_API USimpleMoveScreenFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	* You can control the movement of the screen in eight directions with the mouse .
	* @param   ScreenMoveSpeed		Screen movement speed .
	* @return  Can move back to ture .
	*/
	UFUNCTION(BlueprintCallable, Category = SimpleMoveScreen, meta = (WorldContext = WorldContextObject))
	static bool ListenScreenMove(UObject *WorldContextObject, float ScreenMoveSpeed = 100.f);
	
};
