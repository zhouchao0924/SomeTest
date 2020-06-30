// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SimpleGameUserSettings.h"
#include "../Private/Internationalization/LegacyInternationalization.h"
#include "Internationalization/Culture.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/AmbientSound.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

USimpleGameUserSettings::USimpleGameUserSettings()
{

}

void USimpleGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);

	//´æ´¢µ½ÅäÖÃ
	SeveTDIni();
}

void USimpleGameUserSettings::LoadSettings(bool bForceReload /*= false*/)
{
	Super::LoadSettings(bForceReload);

	LoadTDIni();
}

void USimpleGameUserSettings::SaveSettings()
{
	Super::SaveSettings();
}

void USimpleGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();

	BackgroundSound = 1.0f;

	GameSound = 1.0f;

	UISound = 1.0f;
}

void USimpleGameUserSettings::SetCurrentLanguage(const FString &NewLanguage)
{
	if (NewLanguage == "English" || NewLanguage == "en")
	{
		FInternationalization::Get().SetCurrentCulture("en-US");
	}
	else if (NewLanguage == "Chinese" || NewLanguage == "zh" || NewLanguage == "zh-CN")
	{
		FInternationalization::Get().SetCurrentCulture("zh-Hans-CN");
	}
}

void USimpleGameUserSettings::SeveTDIni()
{
	const TCHAR* Section = TEXT("SimpleGameUserSettingsSound");
	GConfig->SetFloat(Section, TEXT("sg.BackgroundSound"), BackgroundSound, GGameUserSettingsIni);
	GConfig->SetFloat(Section, TEXT("sg.GameSound"), GameSound, GGameUserSettingsIni);
	GConfig->SetFloat(Section, TEXT("sg.UISound"), UISound, GGameUserSettingsIni);
	GConfig->SetBool(Section, TEXT("sg.bSupportPhysX"), bSupportPhysX, GGameUserSettingsIni);
	GConfig->SetString(Section, TEXT("sg.Language"), *GetCurrentLanguageType(), GGameUserSettingsIni);
}

bool USimpleGameUserSettings::LoadTDIni()
{
	const TCHAR* Section = TEXT("SimpleGameUserSettingsSound");
	FString LanguageString;
	bool bSueecss =
		GConfig->GetFloat(Section, TEXT("sg.BackgroundSound"), BackgroundSound, GGameUserSettingsIni) &&
		GConfig->GetFloat(Section, TEXT("sg.GameSound"), GameSound, GGameUserSettingsIni) &&
		GConfig->GetFloat(Section, TEXT("sg.UISound"), UISound, GGameUserSettingsIni) &&
		GConfig->GetBool(Section, TEXT("sg.bSupportPhysX"), bSupportPhysX, GGameUserSettingsIni) &&
		GConfig->GetString(Section, TEXT("sg.Language"), LanguageString, GGameUserSettingsIni);

	SetCurrentLanguage(LanguageString);

	return bSueecss;
}

USimpleGameUserSettings* USimpleGameUserSettings::GetSimpleGameUserSettings()
{
	return GEngine != NULL ? (Cast<USimpleGameUserSettings>(GEngine->GetGameUserSettings())) : NULL;
}

FString USimpleGameUserSettings::GetCurrentLanguageType()
{
	if (FInternationalization::Get().GetCurrentCulture().Get().GetName() == "en" ||
		FInternationalization::Get().GetCurrentCulture().Get().GetName() == "en-US")
	{
		return "English";
	}
	else if (FInternationalization::Get().GetCurrentCulture().Get().GetName() == "zh" ||
		FInternationalization::Get().GetCurrentCulture().Get().GetName() == "zh-CN" ||
		FInternationalization::Get().GetCurrentCulture().Get().GetName() == "zh-Hans-CN")
	{
		return "Chinese";
	}

	return "English";
}

class UWorld* USimpleGameUserSettings::GetWorld() const
{
	return GWorld;
}

void USimpleGameUserSettings::SetBackgroundSound(float VolumeMultiplier)
{
	BackgroundSound = VolumeMultiplier;

	TArray<USoundClass*> TmpSound;
	GetAllSoundClass(TmpSound);

	for (USoundClass* NewSoundClass : TmpSound)
	{
		NewSoundClass->Properties.Volume = BackgroundSound;
	}
}

void USimpleGameUserSettings::SetGameSound(float VolumeMultiplier)
{
	GameSound = VolumeMultiplier;
}

void USimpleGameUserSettings::SetUISound(float VolumeMultiplier)
{
	UISound = VolumeMultiplier;
}

float USimpleGameUserSettings::GetBackgroundSound() const
{
	return BackgroundSound;
}

float USimpleGameUserSettings::GetGameSound() const
{
	return GameSound;
}

float USimpleGameUserSettings::GetUISound() const
{
	return UISound;
}

bool USimpleGameUserSettings::GetAllAmbientSound(TArray<class AAmbientSound*> &Sounds)
{
	TArray<AActor*> ClassArrayTmp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAmbientSound::StaticClass(), ClassArrayTmp);

	for (AActor* Tmp : ClassArrayTmp)
	{
		if (AAmbientSound *NewSound = Cast<AAmbientSound>(Tmp))
		{
			Sounds.Add(NewSound);
		}
	}

	return Sounds.Num() > 0;
}

bool USimpleGameUserSettings::SpawnSoundAtLocation(USoundBase* CharacterSound, const FVector &Location)
{
	if (CharacterSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CharacterSound, Location, GameSound);
	}

	return CharacterSound != nullptr;
}

bool USimpleGameUserSettings::GetAllSoundClass(TArray<class USoundClass*> &Sounds)
{
	TArray<AActor*> ClassArrayTmp;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAmbientSound::StaticClass(), ClassArrayTmp);

	for (AActor* Tmp : ClassArrayTmp)
	{
		AAmbientSound* AmbientSound = Cast<AAmbientSound>(Tmp);
		if (AmbientSound &&
			AmbientSound->GetAudioComponent() &&
			AmbientSound->GetAudioComponent()->Sound &&
			AmbientSound->GetAudioComponent()->Sound->DefaultSoundClassObject
			)
		{
			Sounds.Add(AmbientSound->GetAudioComponent()->Sound->DefaultSoundClassObject);
		}
	}

	return Sounds.Num() > 0;
}
