// Fill out your copyright notice in the Description page of Project Settings.


#include "SimplePopupBlueprintLibrary.h"

FPopupHandle USimplePopupBlueprintLibrary::CreatePopup(
	UObject* WorldContextObject,
	TSubclassOf<UUI_SimplePopup> PopupClass,
	const FText &Content,
	ESimplePopupType SimplePopupType,
	FSimpleBlueprintDelegate SureDelegate,
	FSimpleBlueprintDelegate CancelDelegate,
	float Life /*= 10.f*/)
{
	FPopupHandle Handle;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World && PopupClass)
	{
		if (UUI_SimplePopup *SimplePopupPtr = CreateWidget<UUI_SimplePopup>(World, PopupClass))
		{
			SimplePopupPtr->AddToViewport(100);

			SimplePopupPtr->SimpleBlueprintCancelDelegate = CancelDelegate;
			SimplePopupPtr->SimpleBlueprintSureDelegate = SureDelegate;
			SimplePopupPtr->Delay = Life;
			SimplePopupPtr->SimplePopupType = SimplePopupType;
			SimplePopupPtr->SetText(Content);
			SimplePopupPtr->InitPopup();

			Handle.SetSimplePopup(SimplePopupPtr);
		}
	}

	return Handle;
}

bool USimplePopupBlueprintLibrary::PopupSure(FPopupHandle PopupHandle)
{
	return PopupHandle.Sure();
}

bool USimplePopupBlueprintLibrary::PopupCancel(FPopupHandle PopupHandle)
{
	return PopupHandle.Cancel();
}
