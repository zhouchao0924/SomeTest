#pragma once

#include "CoreMinimal.h"

#define BYTE_30M 31457280

namespace SimplePak
{
	struct FPakHash
	{
		FString UniqueID;

		bool operator ==(const FPakHash &SimpleFPakHash)
		{
			return UniqueID == SimpleFPakHash.UniqueID;
		}

		explicit FPakHash()//·ÀÖ¹ÒþÊ½µ÷ÓÃ
			:UniqueID(FGuid::NewGuid().ToString())
		{

		}
	};

	template<typename ValueType>
	struct TFPakHashMapKeyFuncs :BaseKeyFuncs<TPair<FPakHash, ValueType>, FString>
	{
		typedef BaseKeyFuncs<TPair<FPakHash, ValueType>, FString> Super;

		static Super::KeyInitType GetSetKey(Super::ElementInitType Element)
		{
			return Element.Key.UniqueID;
		}

		static bool Matches(Super::KeyInitType A, Super::KeyInitType B)
		{
			return A.Compare(B, ESearchCase::CaseSensitive) == 0;
		}

		static uint32 GetKeyHash(Super::KeyInitType Key)
		{
			return FCrc::StrCrc32(*Key);
		}
	};

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

	typedef TMap<FPakHash, FByteInfo, FDefaultSetAllocator, TFPakHashMapKeyFuncs<FPakInfo>> TSimpleHashMapPakInfo;

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
