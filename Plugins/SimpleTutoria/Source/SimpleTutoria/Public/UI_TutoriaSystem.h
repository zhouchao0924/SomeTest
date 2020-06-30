// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_TutoriaSystem.generated.h"

class UScrollBox;
class UCheckBox;
class UButton;
class USlider;
class UImage;
class UTextBlock;
class UMediaPlayer;
class UUI_TutoriaSlot;
class UWidgetAnimation;
class UMediaSoundComponent;
class UComboBoxString;
class UUI_TutoriaList;


enum class ETutoriaType
{
	FULL_SCREEN,
	WINDOW,
	MAX
};

UCLASS()
class SIMPLETUTORIA_API UUI_TutoriaSystem : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UUI_TutoriaList* TutoriaList;

	UPROPERTY(meta = (BindWidget))
		UCheckBox *ScreenButton;

	UPROPERTY(meta = (BindWidget))
		UButton* ReplayButton;

	UPROPERTY(meta = (BindWidget))
		UButton* NextButton;

	UPROPERTY(meta = (BindWidget))
		UButton* PreviousButton;

	UPROPERTY(meta = (BindWidget))
		UCheckBox *VolumeButton;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* VolumeValue;

	UPROPERTY(meta = (BindWidget))
		USlider* VolumeProgress;

	UPROPERTY(meta = (BindWidget))
		UCheckBox* SuspendButton;

	UPROPERTY(meta = (BindWidget))
		UButton* PauseButton;

	UPROPERTY(meta = (BindWidget))
		USlider* MovieProgress;

	UPROPERTY(meta = (BindWidget))
		UImage*  PauseImage;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* PlayTimeText;

	UPROPERTY()
		UMediaSoundComponent *MediaSoundComponent;

	UPROPERTY(meta = (BindWidget))
		UComboBoxString *ComboPlaySpeed;

	UPROPERTY(meta = (BindWidget))
		UComboBoxString *ComboResolution;

public:
	//载入视频播放
	UPROPERTY(EditAnywhere, Category = UI)
		UMediaPlayer* MediaPlayer;

	UPROPERTY(EditAnywhere, Category = UI)
		TSubclassOf<UUI_TutoriaSlot> TutoriaSlotClass;
	UUI_TutoriaSystem(const FObjectInitializer& ObjectInitializer);

	ETutoriaType TutoriaType;
	void SetTutoriaType(ETutoriaType InTutoriaType);

	void PlayMedia(const FString &URL, const FTimespan &InTime);
protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	//初始化视频资源
	UFUNCTION()
		void InitMadia();

	UFUNCTION()
		void ChangedValue(float InValue);

	UFUNCTION()
		void ChangedVolumeValue(float InValue);

	UFUNCTION()
		void PlaySpeed(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
		void Resolution(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
		void ClickedCheckBox(bool ClickedWidget);

	UFUNCTION()
		void ClickedCheckBoxVolume(bool ClickedWidget);

	UFUNCTION()
		void ClickedCheckBoxScreen(bool ClickedWidget);

	UFUNCTION()
		void ActivationMovie();

	UFUNCTION()
		void Previous();

	UFUNCTION()
		void Next();

	UFUNCTION()
		void Replay();

	UFUNCTION()
		void Close();

	UFUNCTION()
		void Pause();

	UFUNCTION()
		bool Play(const FString &InPath);

	UFUNCTION()
		void FinshPlayMovie();

	UFUNCTION()
		void MouseCaptureBegin();

	UFUNCTION()
		void MouseCaptureEnd();

protected:
	UPROPERTY()
		UWidgetAnimation* OpenBarAnimation;

	UPROPERTY()
		UWidgetAnimation* CloseBarAnimation;

	float MouseMoveCount;
	bool bMouseMoveFinsh;

private:
	UPROPERTY()
		UUI_TutoriaSystem* UI_TutoriaSystem;

protected:
		UWidgetAnimation* GetNameWidgetAnimation(const FString& WidgetAnimationName) const;
};
