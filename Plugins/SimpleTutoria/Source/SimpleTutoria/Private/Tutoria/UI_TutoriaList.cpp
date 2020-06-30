// Fill out your copyright notice in the Description page of Project Settings.


#include "TuToria/UI_TutoriaList.h"
#include "Components/ScrollBox.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"

UWidgetAnimation* UUI_TutoriaList::GetNameWidgetAnimation(const FString& WidgetAnimationName) const
{
	if (UWidgetBlueprintGeneratedClass* WidgetBlueprintGenerated = Cast<UWidgetBlueprintGeneratedClass>(GetClass()))
	{
		TArray<UWidgetAnimation*> TArrayAnimations = WidgetBlueprintGenerated->Animations;
		UWidgetAnimation** MyTempAnimation = TArrayAnimations.FindByPredicate([&](const UWidgetAnimation* OurAnimation) {return OurAnimation->GetFName().ToString() == (WidgetAnimationName + FString("_INST")); });
		return *MyTempAnimation;
	}

	return nullptr;
}

void UUI_TutoriaList::Add(UWidget *InWidget)
{
	if (ScrollMediaList)
	{
		ScrollMediaList->AddChild(InWidget);
	}
}

void UUI_TutoriaList::NativeConstruct()
{
	Super::NativeConstruct();

	OpenPlayListAnimation = GetNameWidgetAnimation("OpenPlayList");
	ClosePlayListAnimation = GetNameWidgetAnimation("ClosePlayList");
}

void UUI_TutoriaList::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (OpenPlayListAnimation)
	{
		PlayAnimation(OpenPlayListAnimation);
	}
}

void UUI_TutoriaList::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (ClosePlayListAnimation)
	{
		PlayAnimation(ClosePlayListAnimation);
	}
}
