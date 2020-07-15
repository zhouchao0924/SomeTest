#pragma once
#include "BlueprintData.h"

struct FBTCompilationUtilities
{
	static void FlushCompilationQueueImpl(UBlueprintData *Data);

	static void ClearByte(UBlueprintData *Data);
};