// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

namespace SimpleUnrealPakEditor
{
	struct FPak
	{
		TArray<FProcHandle> PakFile(const TArray<FString> &Paths);
		TArray<FProcHandle> PakMapFile(const TArray<FString> &Paths);
		FProcHandle Cook(const TArray<FString> &MapPaths,bool bMap);
	};
}