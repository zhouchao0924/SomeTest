#pragma once

#include "CoreMinimal.h"
#include "PakInfo.h"

namespace SimpleZip
{
	struct FDecompressedByte
	{
		static bool DecompressedData(const FString & ReadPath, const FString & SavaPath, EEncryptionMode EncryptionMode = EEncryptionMode::NONE, const FString &Keys = "");

		static bool AESDecrypData(TArray<uint8> &RawData, const FString &Keys);
	};
}