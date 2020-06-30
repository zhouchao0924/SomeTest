// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_TutoriaSystem.h"
#include "Tutoria/UI_TutoriaSlot.h"
#include "Tutoria/UI_TutoriaList.h"
#include "MediaPlayer.h"
#include "Components/Image.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "HAL/FileManager.h"
#include "MediaSource.h"
#include "GlobalTutoriaProxy.h"
#include "Components/ScrollBox.h"
#include "MediaPlaylist.h"
#include "MediaSoundComponent.h"
#include "Components/ComboBoxString.h"

UUI_TutoriaSystem::UUI_TutoriaSystem(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	, MouseMoveCount(0.f)
	, bMouseMoveFinsh(false)
{

}

void UUI_TutoriaSystem::SetTutoriaType(ETutoriaType InTutoriaType)
{
	TutoriaType = InTutoriaType;

	switch (InTutoriaType)
	{
	case ETutoriaType::FULL_SCREEN:
		ScreenButton->SetCheckedState(ECheckBoxState::Checked);
		break;
	case ETutoriaType::WINDOW:
		ScreenButton->SetCheckedState(ECheckBoxState::Unchecked);
		break;
	}
}

void UUI_TutoriaSystem::PlayMedia(const FString &URL, const FTimespan &InTime)
{
	MediaPlayer->OpenFile(URL);
	MediaPlayer->Seek(InTime);
}

void UUI_TutoriaSystem::NativeConstruct()
{
	Super::NativeConstruct();
	InitMadia();
	SetTutoriaType(ETutoriaType::WINDOW);
	MediaPlayer->NativeAudioOut = true;

	MediaPlayer->OnEndReached.AddDynamic(this, &UUI_TutoriaSystem::FinshPlayMovie);

	ReplayButton->OnClicked.AddDynamic(this, &UUI_TutoriaSystem::Replay);
	PauseButton->OnClicked.AddDynamic(this, &UUI_TutoriaSystem::Pause);
	SuspendButton->OnCheckStateChanged.AddDynamic(this, &UUI_TutoriaSystem::ClickedCheckBox);
	ScreenButton->OnCheckStateChanged.AddDynamic(this, &UUI_TutoriaSystem::ClickedCheckBoxScreen);
	PreviousButton->OnClicked.AddDynamic(this, &UUI_TutoriaSystem::Previous);
	NextButton->OnClicked.AddDynamic(this, &UUI_TutoriaSystem::Next);
	OpenBarAnimation = GetNameWidgetAnimation("OpenBar");
	CloseBarAnimation = GetNameWidgetAnimation("CloseBar");
	VolumeButton->OnCheckStateChanged.AddDynamic(this, &UUI_TutoriaSystem::ClickedCheckBoxVolume);

	ComboResolution->OnSelectionChanged.AddDynamic(this, &UUI_TutoriaSystem::Resolution);
	ComboPlaySpeed->OnSelectionChanged.AddDynamic(this, &UUI_TutoriaSystem::PlaySpeed);

	MovieProgress->OnMouseCaptureBegin.AddDynamic(this, &UUI_TutoriaSystem::MouseCaptureBegin);
	MovieProgress->OnMouseCaptureEnd.AddDynamic(this, &UUI_TutoriaSystem::MouseCaptureEnd);
	MovieProgress->OnValueChanged.AddDynamic(this, &UUI_TutoriaSystem::ChangedValue);
	VolumeProgress->OnValueChanged.AddDynamic(this, &UUI_TutoriaSystem::ChangedVolumeValue);

	SimpleTutoriaMulticastDelegate.BindUObject(this, &UUI_TutoriaSystem::Play);
}

void UUI_TutoriaSystem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (MediaPlayer)
	{
		float NewValue = MediaPlayer->GetTime().GetTotalSeconds() / MediaPlayer->GetDuration().GetTotalSeconds();
		MovieProgress->SetValue(NewValue);

		PlayTimeText->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d:%02d/%02d:%02d:%02d"), MediaPlayer->GetTime().GetHours(), MediaPlayer->GetTime().GetMinutes(), MediaPlayer->GetTime().GetSeconds(), MediaPlayer->GetDuration().GetHours(), MediaPlayer->GetDuration().GetMinutes(), MediaPlayer->GetDuration().GetSeconds())));
	}

	if (MouseMoveCount > 0.0f)
	{
		MouseMoveCount -= InDeltaTime;
		bMouseMoveFinsh = true;
	}
	else if (bMouseMoveFinsh)
	{
		PlayAnimation(CloseBarAnimation);
		bMouseMoveFinsh = false;
	}
}

FReply UUI_TutoriaSystem::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (MouseMoveCount <= 0.f && InMouseEvent.GetCursorDelta() != FVector2D::ZeroVector)
	{
		MouseMoveCount = 5.f;
		PlayAnimation(OpenBarAnimation);
	}
	else
	{
		if (InMouseEvent.GetCursorDelta() != FVector2D::ZeroVector)
		{
			MouseMoveCount = 5.f;
		}
	}

	return FReply::Handled();
}

void UUI_TutoriaSystem::InitMadia()
{
	if (MediaPlayer)
	{
		if (!MediaSoundComponent)
		{
			MediaSoundComponent = NewObject<UMediaSoundComponent>(this, UMediaSoundComponent::StaticClass());
			MediaSoundComponent->RegisterComponentWithWorld(GetWorld());
			MediaSoundComponent->SetMediaPlayer(MediaPlayer);
		}

		TArray<FString> MadiaFilenames;
		FString MadiaPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / TEXT("Media/"));
		IFileManager::Get().FindFilesRecursive(MadiaFilenames, *MadiaPath, TEXT("*.*"), true, false);

		for (int32 i = 0; i < MadiaFilenames.Num(); i++)
		{
			if (UUI_TutoriaSlot *TutoriaSlot = CreateWidget<UUI_TutoriaSlot>(GetWorld(), TutoriaSlotClass))
			{
				TutoriaSlot->TutoriaPath = MadiaFilenames[i];
				TutoriaList->Add(TutoriaSlot);
			}
		}
		if (MadiaFilenames.Num() > 0)
		{
			Play(MadiaFilenames[0]);
		}
	}
}

void UUI_TutoriaSystem::ChangedValue(float InValue)
{
	if (MediaPlayer)
	{
		MediaPlayer->Seek(FTimespan(MediaPlayer->GetDuration().GetTicks() * InValue));
	}
}

void UUI_TutoriaSystem::ChangedVolumeValue(float InValue)
{
	if (MediaPlayer)
	{
		MediaPlayer->SetNativeVolume(InValue);//不能调整声音大小

		int32 IntValue = InValue * 10;
		VolumeValue->SetText(FText::FromString(FString::Printf(TEXT("%02d"), IntValue)));
	}
}

void UUI_TutoriaSystem::PlaySpeed(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	SelectedItem.RemoveFromEnd("x");
	float NewPlaySpeed = FCString::Atof(*SelectedItem);

	if (MediaPlayer)
	{
		MediaPlayer->SetRate(NewPlaySpeed);
	}
}

void UUI_TutoriaSystem::Resolution(FString SelectedItem, ESelectInfo::Type SelectionType)
{

}

void UUI_TutoriaSystem::ActivationMovie()
{
	if (MediaPlayer->IsPlaying())
	{
		MediaPlayer->Pause();
		PauseImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SuspendButton->SetCheckedState(ECheckBoxState::Unchecked);
	}
	else
	{
		MediaPlayer->Play();
		PauseImage->SetVisibility(ESlateVisibility::Hidden);
		SuspendButton->SetCheckedState(ECheckBoxState::Checked);
	}
	
}

void UUI_TutoriaSystem::Previous()
{
	MediaPlayer->Previous();
}

void UUI_TutoriaSystem::Next()
{
	MediaPlayer->Next();
}

void UUI_TutoriaSystem::ClickedCheckBox(bool ClickedWidget)
{
	ActivationMovie();
}

void UUI_TutoriaSystem::ClickedCheckBoxVolume(bool ClickedWidget)
{
	if (MediaPlayer)
	{
		MediaPlayer->SetNativeVolume((float)ClickedWidget);
	}
}

void UUI_TutoriaSystem::ClickedCheckBoxScreen(bool ClickedWidget)
{
	if (TutoriaType == ETutoriaType::FULL_SCREEN)
	{
		RemoveFromParent();
	}
	else if (TutoriaType == ETutoriaType::WINDOW)
	{
		if (UUI_TutoriaSystem *TutoriaSystem = CreateWidget<UUI_TutoriaSystem>(GetWorld(), GetClass()))
		{
			TutoriaSystem->AddToViewport(99);
			TutoriaSystem->SetTutoriaType(ETutoriaType::FULL_SCREEN);
			TutoriaSystem->PlayMedia(MediaPlayer->GetUrl(), MediaPlayer->GetTime());
		}
	}
}

void UUI_TutoriaSystem::Replay()
{
	if (MediaPlayer->IsReady())
	{
		MediaPlayer->Rewind();
	}
}

void UUI_TutoriaSystem::Close()
{
	if (MediaPlayer->IsPlaying())
	{
		MediaPlayer->Pause();
		PauseImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SuspendButton->SetCheckedState(ECheckBoxState::Unchecked);
	}
	MediaPlayer->Close();
}

void UUI_TutoriaSystem::Pause()
{
	ActivationMovie();
}

bool UUI_TutoriaSystem::Play(const FString &InPath)
{
	return MediaPlayer->OpenFile(InPath);
}

void UUI_TutoriaSystem::FinshPlayMovie()
{
	if (MediaPlayer->IsReady())
	{
		MediaPlayer->Close();
	}
}

void UUI_TutoriaSystem::MouseCaptureBegin()
{

}

void UUI_TutoriaSystem::MouseCaptureEnd()
{

}

UWidgetAnimation* UUI_TutoriaSystem::GetNameWidgetAnimation(const FString& WidgetAnimationName) const
{
	if (UWidgetBlueprintGeneratedClass* WidgetBlueprintGenerated = Cast<UWidgetBlueprintGeneratedClass>(GetClass()))
	{
		TArray<UWidgetAnimation*> TArrayAnimations = WidgetBlueprintGenerated->Animations;
		UWidgetAnimation** MyTempAnimation = TArrayAnimations.FindByPredicate([&](const UWidgetAnimation* OurAnimation) {return OurAnimation->GetFName().ToString() == (WidgetAnimationName + FString("_INST")); });
		return *MyTempAnimation;
	}

	return nullptr;
}
