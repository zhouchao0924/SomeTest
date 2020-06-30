// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_TutoriaList.generated.h"

class UScrollBox;
/**
 * 
 */
UCLASS()
class SIMPLETUTORIA_API UUI_TutoriaList : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
		UScrollBox *ScrollMediaList;
public:
	UWidgetAnimation* GetNameWidgetAnimation(const FString& WidgetAnimationName) const;
public:

	void Add(UWidget *InWidget);
protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
		UWidgetAnimation* OpenPlayListAnimation;

	UPROPERTY()
		UWidgetAnimation* ClosePlayListAnimation;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
};
