#define IsUse 0

#if IsUse
void UUI_SimpleGameSettingsCore::SetSettingsVlaue(USlider* InSlider, UTextBlock *NewBlock, TFunction<void(float InValue)> InFunc)
{
	InFunc(InSlider->GetValue());
	//USimpleGameUserSettings::GetSimpleGameUserSettings()->SetBackgroundSound(InSlider->GetValue());

	//更新显示值
	UpdateAttibe(InSlider, NewBlock);
}

SetSettingsVlaue(BackgroundSoundSlider, BackgroundSoundText,
	[](float InValue)
{
	USimpleGameUserSettings::GetSimpleGameUserSettings()->SetBackgroundSound(InValue);
});

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUI_SimpleGameSettingsCore::LoadSettingsVlaue(USlider* InSlider, UTextBlock *NewBlock, TFunction<float()> InFunc)
{
	InSlider->SetValue(InFunc());

	//更新显示值
	UpdateAttibe(InSlider, NewBlock);
}

LoadSettingsVlaue(GameSoundSlider, GameSoundText,
	[]()
{
	return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetGameSound();
});

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUI_SimpleGameSettingsCore::SetSettingsLevel(USlider* InSlider, UTextBlock *NewBlock, TFunction<void(float InLevel)> InFunc)
{
	InFunc(InSlider->GetValue());
	UpdateAttibeLevel(InSlider, NewBlock);
}

SetSettingsLevel(AudioQualityLevelSlider, AudioQualityLevelText,
	[](float InLevel)
{
	USimpleGameUserSettings::GetSimpleGameUserSettings()->SetAudioQualityLevel(InLevel * SCAL_ABILITY_QUALITY_LEVEL);
});

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUI_SimpleGameSettingsCore::LoadSettingsLevel(USlider* InSlider, UTextBlock *NewBlock, TFunction<float()> InFunc)
{
	InSlider->SetValue(InFunc());
	UpdateAttibeLevel(InSlider, NewBlock);
}

LoadSettingsLevel(AudioQualityLevelSlider, AudioQualityLevelText,
	[]()
{
	return USimpleGameUserSettings::GetSimpleGameUserSettings()->GetAudioQualityLevel() / SCAL_ABILITY_QUALITY_LEVEL;
});

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UUI_SimpleGameSettingsCore::LoadBoxString(UComboBoxString* InBoxString, TFunction<FString()> InFunc)
{
	InBoxString->SetSelectedOption(InFunc());
}

LoadBoxString(ResolutionBoxString,
	[]()
{
	FIntPoint ResolutionPoint = USimpleGameUserSettings::GetSimpleGameUserSettings()->GetScreenResolution();
	return FString::Printf(TEXT("%ix%i"), ResolutionPoint.X, ResolutionPoint.Y);
});

#endif