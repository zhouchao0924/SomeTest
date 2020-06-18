// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUDUI.h"
#include "SlateOptMacros.h"
#include "MenuStyles.h"
#include "GlobalMenuStyle.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "GameMapGameMode.h"
#include "GameCharacter.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SGameHUDUI::Construct(const FArguments& InArgs)
{
	Score.Bind(this, &SGameHUDUI::GetScore);
	Health.Bind(this, &SGameHUDUI::GetHealth);

	OwnerHUD = InArgs._OwnerHUD;

	HUDStyle = &FMenuStyles::Get().GetWidgetStyle<FGlobalStyle>("Global");

	ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Top)
			[
				SNew(STextBlock)
				.TextStyle(&HUDStyle->MenuTitleStyle)
				.Text(Score)
			]
			+ SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Top)
			[
				SNew(STextBlock)
				.TextStyle(&HUDStyle->MenuTitleStyle)
				.Text(Health)
			]
		];

}

FText SGameHUDUI::GetScore() const
{
	// NOTE: THIS IS A TERRIBLE WAY TO DO THIS. DO NOT DO IT. IT ONLY WORKS ON SERVERS. USE GAME STATES INSTEAD!
	AGameMapGameMode* gameMode = Cast<AGameMapGameMode>(OwnerHUD->GetWorldSettings()->GetWorld()->GetAuthGameMode());

	if (gameMode == nullptr)
		return FText::FromString(TEXT("SCORE: --"));

	FString score = TEXT("SCORE: ");
	score.AppendInt(gameMode->GetScore());

	return FText::FromString(score);
}

FText SGameHUDUI::GetHealth() const
{
	AGameCharacter* character = Cast<AGameCharacter>(OwnerHUD->PlayerOwner->GetCharacter());

	if (character == nullptr)
		return FText::FromString(TEXT("HEALTH: --"));

	FString health = TEXT("HEALTH: ");
	health.AppendInt(character->Health);

	return FText::FromString(health);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION