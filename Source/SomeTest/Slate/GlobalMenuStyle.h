// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateWidgetStyleContainerBase.h"
#include "GlobalMenuStyle.generated.h"



/**
 * 
 */
USTRUCT()
struct FGlobalStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override;
	static const FGlobalStyle& GetDefault();

	// Style that define the appearance of all menu buttons. 
	UPROPERTY(EditAnywhere, Category = Appearance)
		FButtonStyle MenuButtonStyle;

	// Style that defines the text on all of our menu buttons. 
	UPROPERTY(EditAnywhere, Category = Appearance)
		FTextBlockStyle MenuButtonTextStyle;

	// Style that defines the text for our menu title. 
	UPROPERTY(EditAnywhere, Category = Appearance)
		FTextBlockStyle MenuTitleStyle;
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UGlobalMenuStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FGlobalStyle MenuStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &MenuStyle);
	}
};
