#include "SlateSound/SimpleSlateSound.h"
#include "Sound/SoundBase.h"
#include "AudioDevice.h"
#include "ActiveSound.h"
#include "Sound/SlateSound.h"
#include "Core/SimpleGameUserSettings.h"

void FSimpleSlateSoundDevice::PlaySound(const FSlateSound& Sound, int32 UserIndex /*= 0*/) const
{
	if (GEngine && Sound.GetResourceObject() != nullptr)
	{
		if (FAudioDevice* const AudioDevice = GEngine->GetActiveAudioDevice())
		{
			if (USoundBase* const SoundResource = Cast<USoundBase>(Sound.GetResourceObject()))
			{
				FActiveSound NewActiveSound;
				NewActiveSound.SetSound(SoundResource);
				NewActiveSound.bIsUISound = true;
				NewActiveSound.UserIndex = UserIndex;
				NewActiveSound.Priority = SoundResource->Priority;
				NewActiveSound.VolumeMultiplier = USimpleGameUserSettings::GetSimpleGameUserSettings()->GetUISound();

				AudioDevice->AddNewActiveSound(NewActiveSound);
			}
		}
	}
}

float FSimpleSlateSoundDevice::GetSoundDuration(const FSlateSound& Sound) const
{
	USoundBase* const SoundResource = Cast<USoundBase>(Sound.GetResourceObject());
	return (SoundResource) ? SoundResource->Duration : 0.0f;
}

