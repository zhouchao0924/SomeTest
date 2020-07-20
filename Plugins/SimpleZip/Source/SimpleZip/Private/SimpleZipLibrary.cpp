// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SimpleZipLibrary.h"
#include "Compressed/CompressedByte.h"
#include "Compressed/DecompressionByte.h"

USimpleZipLibrary::USimpleZipLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

bool USimpleZipLibrary::CompressedData(FString ReadPath, FString SavaPath, FString BaseURL /*= ""*/, int64 FragmentSize /*= 31457280*/, EEncryptionMode EncryptionMode /*= EEncryptionMode::NONE*/, FString Keys /*= ""*/)
{
	return SimpleZip::FCompressedByte::CompressedData(ReadPath, SavaPath, BaseURL, FragmentSize, EncryptionMode, Keys);
}

bool USimpleZipLibrary::DecompressedData(FString ReadPath, FString SavaPath, EEncryptionMode EncryptionMode /*= EEncryptionMode::NONE*/, FString Keys /*= ""*/)
{
	return SimpleZip::FDecompressedByte::DecompressedData(ReadPath, SavaPath, EncryptionMode, Keys);
}

