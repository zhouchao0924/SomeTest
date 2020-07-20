// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleZipLibrary.generated.h"

UCLASS()
class USimpleZipLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Compressed", Keywords = "Com"), Category = "SimpleZipLibrary")
		static bool CompressedData(FString ReadPath, FString SavaPath, FString BaseURL = "", int64 FragmentSize = 31457280, EEncryptionMode EncryptionMode = EEncryptionMode::NONE, FString Keys = "");

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Decompressed", Keywords = "Com"), Category = "SimpleZipLibrary")
		static bool DecompressedData(FString ReadPath, FString SavaPath, EEncryptionMode EncryptionMode = EEncryptionMode::NONE, FString Keys = "");
};
