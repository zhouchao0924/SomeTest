#pragma once

#include "CoreMinimal.h"
#include "PakInfo.h"

namespace SimplePak
{
	struct FCompressedByte
	{
		bool CompressedData(const FString & ReadPath, const FString & SavaPath, int64 FragmentSize = BYTE_30M, uint8 Tmp = 1);
	};
}