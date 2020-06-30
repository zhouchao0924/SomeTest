// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_DELEGATE_OneParam(FSimpleArchivesSlotDelegate,FSimpleDelegate)

enum class EArchivesState :uint8
{
	NONE,
	SAVE,
	LOAD,
};