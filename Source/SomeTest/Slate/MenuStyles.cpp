// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuStyles.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"

TSharedPtr<FSlateStyleSet> FMenuStyles::MenuStyleInstance = NULL;

void FMenuStyles::Initialize()
{
	if (!MenuStyleInstance.IsValid())
	{
		MenuStyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*MenuStyleInstance);
	}
}

void FMenuStyles::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*MenuStyleInstance);
	ensure(MenuStyleInstance.IsUnique());
	MenuStyleInstance.Reset();
}

FName FMenuStyles::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MenuStyles"));
	return StyleSetName;
}

TSharedRef<FSlateStyleSet> FMenuStyles::Create()
{
	TSharedRef<FSlateStyleSet> StyleRef = FSlateGameResources::New(FMenuStyles::GetStyleSetName(), "/Game/Slate/UI/Styles", "/Game/Slate/UI/Styles");
	return StyleRef;
}

const ISlateStyle& FMenuStyles::Get()
{
	return *MenuStyleInstance;
}