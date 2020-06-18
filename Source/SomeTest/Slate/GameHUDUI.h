// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "GlobalMenuStyle.h"
#include "GameHUD.h"

/**
 * 
 */
class SOMETEST_API SGameHUDUI : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGameHUDUI)
		: _OwnerHUD()
	{}
	SLATE_ARGUMENT(TWeakObjectPtr<class AGameHUD>, OwnerHUD);
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	/**
	 * Stores a weak reference to the HUD owning this widget.
	 **/
	TWeakObjectPtr<class AGameHUD> OwnerHUD;

	/**
	 * A reference to the Slate Style used for this HUD's widgets.
	 **/
	const struct FGlobalStyle* HUDStyle;

private:
	/**
	 * Attribute storing the binding for the player's score.
	 **/
	TAttribute<FText> Score;

	/**
	 * Attribute storing the binding for the player's health.
	 **/
	TAttribute<FText> Health;

	/**
	 * Our Score will be bound to this function, which will retrieve the appropriate data and convert it into an FText.
	 **/
	FText GetScore() const;

	/**
	 * Our Health will be bound to this function, which will retrieve the appropriate data and convert it into an FText.
	 **/
	FText GetHealth() const;
};
