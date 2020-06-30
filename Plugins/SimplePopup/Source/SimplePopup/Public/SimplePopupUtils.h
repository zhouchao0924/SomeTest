// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimplePopupType.h"

class UUI_SimplePopup;

//仅仅提供C++使用
namespace SimplePopupUtils
{
	SIMPLEPOPUP_API FPopupHandle CreatePopup(
		UWorld *InWorld,
		TSubclassOf<UUI_SimplePopup> PopupClass,
		const FText &Content,
		ESimplePopupType SimplePopupType,
		float Life = 10.f,
		FSimpleDelegate SureDelegate = FSimpleDelegate(),
		FSimpleDelegate CancelDelegate = FSimpleDelegate());
}