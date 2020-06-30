// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UI_InformationWidget.h"
#include "Components/TextBlock.h"


void UUI_InformationWidget::SetTextBlockStyle(const FString &InText, const FLinearColor &Color, float Percentage)
{
	int32 MaxFontSize = 50;
	int32 MinFontSize = 16;
	int32 CurrentFontSize = MinFontSize + (MaxFontSize - MinFontSize) *Percentage;
	TextWidget->SetText(FText::FromString(InText));
	TextWidget->SetColorAndOpacity(FSlateColor(Color));
}
