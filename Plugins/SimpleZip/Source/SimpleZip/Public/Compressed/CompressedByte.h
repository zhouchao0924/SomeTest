#pragma once

#include "CoreMinimal.h"
#include "PakInfo.h"

namespace SimpleZip
{
	struct FCompressedByte
	{
		static bool CompressedData(const FString & ReadPath, const FString & SavaPath, const FString &BaseURL = "", int64 FragmentSize = BYTE_30M, EEncryptionMode EncryptionMode = EEncryptionMode::NONE, const FString &Keys = "");
	
		static void AESEncrypData(TArray<uint8> &RawData, const FString &Keys);
	};
}