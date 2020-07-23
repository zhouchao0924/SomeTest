// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleZIPFunctionLibrary.generated.h"

UCLASS()
class SIMPLEZIP_API USimpleZIPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "SimpleZIP")
	static bool Compress(const TArray<uint8> &UncompressedData,TArray<uint8> &OutData);

	UFUNCTION(BlueprintCallable, Category = "SimpleZIP")
	static bool Decompress(const TArray<uint8> &CompressedData, TArray<uint8> &OutData);

	UFUNCTION(BlueprintCallable, Category = "SimpleZIP")
	static bool ZipByLoacl(const FString &SrcPath, const FString &DestFilename, const FString &Password);
	
	UFUNCTION(BlueprintCallable, Category = "SimpleZIP")
	static bool UnzipByLoacl(const FString &SrcPath, const FString &DestFilename, const FString &Password, bool bOverwrite = true);
};