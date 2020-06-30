// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UI_SimplePopup.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBoxSlot.h"

#define LOCTEXT_NAMESPACE "UUI_SimplePopup"
UUI_SimplePopup::UUI_SimplePopup(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	, SimplePopupType(ESimplePopupType::ONE)
	, Delay(10.f)
{

}

void UUI_SimplePopup::SetText(const FText &InNewText)
{
	IntroductionBox->SetText(InNewText);
}

void UUI_SimplePopup::InitPopup()
{
	auto SpawnButton = [&](
		const FText &ButtonName,
		FOnButtonClickedEvent ClickedEvent,
		EHorizontalAlignment HorizontalAlignment,
		EVerticalAlignment VerticalAlignment,
		FSlateChildSize SlateChildSize,
		FMargin InPadding = FMargin())
	{
		if (UButton *NewButton = NewObject<UButton>(this, UButton::StaticClass()))
		{
			NewButton->OnClicked = ClickedEvent;
			if (UHorizontalBoxSlot* HorizontalBoxSlot = ButtonListBox->AddChildToHorizontalBox(NewButton))
			{
				HorizontalBoxSlot->SetHorizontalAlignment(HorizontalAlignment);
				HorizontalBoxSlot->SetVerticalAlignment(VerticalAlignment);
				HorizontalBoxSlot->SetPadding(InPadding);
				HorizontalBoxSlot->SetSize(SlateChildSize);
			}

			if (UTextBlock *TextBlock = NewObject<UTextBlock>(this, UTextBlock::StaticClass()))
			{
				TextBlock->SetText(ButtonName);
				NewButton->AddChild(TextBlock);
			}
		}
	};

	switch (SimplePopupType)
	{
		case ESimplePopupType::NONE:
			RemoveFromParent();
			break;
		case ESimplePopupType::DELAY:
			break;
		case ESimplePopupType::WAIT:
			break;
		case ESimplePopupType::ONE:
		{
			FOnButtonClickedEvent OnClicked;
			OnClicked.AddDynamic(this, &UUI_SimplePopup::Sure);

			SpawnButton(
				LOCTEXT("SureButton", "Sure"),
				OnClicked,
				EHorizontalAlignment::HAlign_Center,
				EVerticalAlignment::VAlign_Bottom,
				ESlateSizeRule::Fill);
			break;
		}
		case ESimplePopupType::TWO:
		{
			FOnButtonClickedEvent OnClickedSure;
			OnClickedSure.AddDynamic(this, &UUI_SimplePopup::Sure);
			FOnButtonClickedEvent OnClickedCancel;
			OnClickedCancel.AddDynamic(this, &UUI_SimplePopup::Cancel);

			SpawnButton(
				LOCTEXT("CancelButton", "Cancel"),
				OnClickedCancel,
				EHorizontalAlignment::HAlign_Left,
				EVerticalAlignment::VAlign_Bottom,
				ESlateSizeRule::Automatic);

			SpawnButton(
				LOCTEXT("SureButton", "Sure"),
				OnClickedSure,
				EHorizontalAlignment::HAlign_Right,
				EVerticalAlignment::VAlign_Bottom,
				ESlateSizeRule::Fill);
			break;
		}
	}
}

void UUI_SimplePopup::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (SimplePopupType == ESimplePopupType::DELAY)
	{
		Delay -= InDeltaTime;
		if (Delay <=0)
		{
			Cancel();
			Delay = 100.f;
		}
	}
}

void UUI_SimplePopup::Cancel()
{
	SimpleBlueprintCancelDelegate.ExecuteIfBound();
	SimpleCancelDelegate.ExecuteIfBound();
	RemoveFromParent();
}

void UUI_SimplePopup::Sure()
{
	SimpleBlueprintSureDelegate.ExecuteIfBound();
	SimpleSureDelegate.ExecuteIfBound();
	RemoveFromParent();
}
#undef LOCTEXT_NAMESPACE