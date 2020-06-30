// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimplePopupType.h"
#include "SimplePopupBlueprintLibrary.generated.h"

class UUI_SimplePopup;
/**
 * We can create a pop-up window and return a handle, according to which we can perform the behavior in the pop-up window. 
 * Of course, it can not be implemented.@See CreatePopup,PopupSure,PopupCancel
 */
UCLASS()
class SIMPLEPOPUP_API USimplePopupBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	 * You can create a pop-up window based on Customization .
	 * @param	PopupClass				Need a UMG Popup class .
	 * @param	Content					What to display in the pop-up window .
	 * @param	SimplePopupType			Types of pop ups @See SimplePopupType.h.
	 * @param	SureDelegate			Definite agent of pop-up window .
	 * @param	CancelDelegate			Cancel agent for pop ups .
	 * @param	Life					When the pop-up window type is ESimplePopupType::DELAY, you need a custom time to destroy it.
	 * @return  FPopupHandle			Returns a pop-up handle. You can customize many pop-up behaviors .@See PopupSure(...),PopupCancel(..).
	 */
	UFUNCTION(BlueprintCallable, Category = SimplePopup, meta = (WorldContext = WorldContextObject))
		static FPopupHandle CreatePopup(UObject* WorldContextObject, TSubclassOf<UUI_SimplePopup> PopupClass,const FText &Content,ESimplePopupType SimplePopupType,FSimpleBlueprintDelegate SureDelegate,FSimpleBlueprintDelegate CancelDelegate,float Life = 10.f);

	/**
	 * Determine pop-up behavior ,Pop up window will disappear after execution .
	 * @return  bool				   When the behavior is executed correctly, return to ture .
	 */
	UFUNCTION(BlueprintCallable, Category = SimplePopup)
		static bool PopupSure(FPopupHandle PopupHandle);
	
	/**
	 * Cancel pop-up behavior ,Pop up window will disappear after execution .
	 * @return  bool				   When the behavior is executed correctly, return to ture .
	 */
	UFUNCTION(BlueprintCallable, Category = SimplePopup)
		static bool PopupCancel(FPopupHandle PopupHandle);
};
