#pragma once

#include "CoreMinimal.h"
#include "PakInfo.generated.h"

#define BYTE_30M 31457280

UENUM()
enum class EEncryptionMode : uint8
{
	NONE,
	AES,
	MAX
};

namespace SimpleZip
{
	//FGuid::NewGuid().ToString()
	typedef FGuid FPakHash;

	struct FByteInfo
	{
		FByteInfo()
			:ByteBegin(0)
			, ByteEnd(BYTE_30M)
		{

		}

		FByteInfo(uint32 InByteBegin, uint32 InByteEnd)
			:ByteBegin(InByteBegin)
			,ByteEnd(InByteEnd)
		{

		}

		uint32 ByteBegin;
		uint32 ByteEnd;
	};

	typedef TMap<FPakHash, FByteInfo> TSimpleHashMapPakInfo;

	struct FPakInfo
	{
		FPakInfo()
			:ByteTotalSize(0)
		{

		}

		FPakInfo(const FString &InRelativePath,uint32 InByteTotalSize)
			:RelativePath(InRelativePath)
			,ByteTotalSize(InByteTotalSize)
		{

		}

		FString RelativePath;
		uint32 ByteTotalSize;
		TSimpleHashMapPakInfo StickyByte;
	};
}
