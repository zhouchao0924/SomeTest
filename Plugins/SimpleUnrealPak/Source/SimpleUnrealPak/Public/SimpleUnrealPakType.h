// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleUnrealPakType.generated.h"

USTRUCT(BlueprintType)
struct SIMPLEUNREALPAK_API FSimpleUnrealPak
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SimpleUnrealPak)
	int64 PakSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SimpleUnrealPak)
	bool bValidPakFile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SimpleUnrealPak)
	TArray<FString> AssetPaths;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SimpleUnrealPak)
	FString MountPoint;
};

USTRUCT(BlueprintType)
struct SIMPLEUNREALPAK_API FSimpleUnrealPakConfig
{
	GENERATED_USTRUCT_BODY()

	FSimpleUnrealPakConfig();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
	FString AES;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Config)
	bool bConfiguration;
};

USTRUCT(BlueprintType)
struct SIMPLEUNREALPAK_API FSimpleUnrealAsset
{
	GENERATED_USTRUCT_BODY()

	FSimpleUnrealAsset(){}
	
	FSimpleUnrealAsset(const FString &InAssetFileName, TArray<FString> &InAssetPaths, const FString &InMountPoint);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SimpleUnrealAsset)
	FString AssetFileName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SimpleUnrealAsset)
	TArray<FString> AssetPaths;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SimpleUnrealAsset)
	FString MountPoint;

	FString GetAssetName() const;

	bool IsValid();

	FORCEINLINE friend bool operator==(const FSimpleUnrealAsset& A, const FSimpleUnrealAsset& B)
	{
		return A.AssetFileName == B.AssetFileName;
	}
};

UENUM(BlueprintType)
enum ESimpleAssetFileType
{
	DLC,
	PATCHS,
	COMMONTPATCHS
};

UENUM(BlueprintType)
enum EMountInfor
{
	RESOURCE_NON_EXISTENT,
	FIND_NON_EXISTENT,
	MOUNT_FAIL,
	MOUNT_SUCCESS,
};

struct SIMPLEUNREALPAK_API FSimpleAssetFileName
{
	static const TCHAR *PakTest;
	static const TCHAR *DLC;
	static const TCHAR *Patchs;
	static const TCHAR *CommontPatchs;
};
#define SCOPE_PLATFORMFILE_ANONYMOUS FSimpleUnrealPakModule::FResetPlatformFile()
#define SCOPE_PLATFORMFILE(SCopePlatformFile) FSimpleUnrealPakModule::FResetPlatformFile SCopePlatformFile