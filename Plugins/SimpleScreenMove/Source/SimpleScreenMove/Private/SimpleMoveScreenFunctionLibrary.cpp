// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleMoveScreenFunctionLibrary.h"
#include "Tool/ScreenMoveUnits.h"

bool USimpleMoveScreenFunctionLibrary::ListenScreenMove(UObject *WorldContextObject, float ScreenMoveSpeed /*= 100.f*/)
{
	if(UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{	
		return FScreenMoveUnits().ListenScreenMove(World->GetFirstPlayerController(), ScreenMoveSpeed);
	}

	return false;
}
