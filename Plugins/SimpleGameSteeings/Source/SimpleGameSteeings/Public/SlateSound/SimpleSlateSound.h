// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Sound/ISlateSoundDevice.h"

struct FSlateSound;

class FSimpleSlateSoundDevice : public ISlateSoundDevice
{
public:
	virtual ~FSimpleSlateSoundDevice() {}

private:
	virtual void PlaySound(const FSlateSound& Sound, int32 UserIndex = 0) const override;
	virtual float GetSoundDuration(const FSlateSound& Sound) const override;
};