// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalMenuStyle.h"


const FName FGlobalStyle::TypeName = TEXT("FGlobalStyle");

const FGlobalStyle& FGlobalStyle::GetDefault()
{
	static FGlobalStyle Default;
	return Default;
}

void FGlobalStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

const FName FGlobalStyle::GetTypeName() const
{
	static const FName InTypeName = TEXT("FGlobalStyle");
	return InTypeName;
}