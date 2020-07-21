// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleUnrealPakType.h"
#include "SUPFunctionLibrary.generated.h"


/**
 * 
 */
UCLASS()
class SIMPLEUNREALPAK_API USUPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetPakInfo", Keywords = "IFO"), Category = "SimpleUnrealPak")
	static void InitConfig(const FSimpleUnrealPakConfig &InConfig);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetPakInfo", Keywords = "IFO"), Category = "SimpleUnrealPak")
	static void GetFilenamesInPakFile(const FString& InPakFilename, TArray<FString>& OutFileList);
	/*
	* You can get information about Pak
	* PakFilename :Pak path .
	* MountPoint  :Installation point path .
	* Return	  :Return Pak message  
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetPakInfo", Keywords = "IFO"), Category = "SimpleUnrealPak")
	static FSimpleUnrealPak GetSimpleUnrealPakInfo(FString PakFilename);

	UFUNCTION(BlueprintCallable, meta = (Keywords = "M"), Category = "SimpleUnrealPak")
	static bool MountPak(FString PakFilename, ESimpleAssetFileType Type,int32 PakOrder, FString MountPath);

	UFUNCTION(BlueprintCallable, meta = (Keywords = "UM"), Category = "SimpleUnrealPak")
	static bool UnmountPak(FString PakFilename);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetMountedFilenames", Keywords = "PakInfo"), Category = "SimpleUnrealPak")
	static TArray<FString> GetMountedPakFilenames();

	UFUNCTION(BlueprintCallable, meta = (Keywords = "DE"), Category = "SimpleUnrealPak")
	static bool DirectoryExists( FString PakDirectory);

	UFUNCTION(BlueprintCallable, meta = (Keywords = "FF"), Category = "SimpleUnrealPak")
	static bool FindFile( FString Directory, TArray<FString> &File, bool bRecursively = true);

	UFUNCTION(BlueprintPure, meta = (Keywords = "OB"), Category = "SimpleUnrealPak")
	static UObject *GetObjectFromPak(const FString &Filename, ESimpleAssetFileType Type);

	UFUNCTION(BlueprintPure, meta = (Keywords = "Mesh"), Category = "SimpleUnrealPak")
	static class UStaticMesh *GetStaticMeshFromPak(const FString &Filename, ESimpleAssetFileType Type);

	UFUNCTION(BlueprintPure, meta = (Keywords = "TT"), Category = "SimpleUnrealPak")
	static class UTexture2D *GetTexture2DFromPak(const FString &Filename, ESimpleAssetFileType Type);

	UFUNCTION(BlueprintPure, meta = (Keywords = "AS"), Category = "SimpleUnrealPak")
	static class UAnimSequence *GetAnimSequenceFromPak(const FString &Filename, ESimpleAssetFileType Type);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetMaterialInst", Keywords = "MatI"), Category = "SimpleUnrealPak")
	static class UMaterialInstanceConstant *GetMaterialInstanceConstantFromPak(const FString &Filename, ESimpleAssetFileType Type);

	UFUNCTION(BlueprintPure, meta = (Keywords = "Mat"), Category = "SimpleUnrealPak")
	static class UMaterial *GetMaterialFromPak(const FString &Filename, ESimpleAssetFileType Type);

	UFUNCTION(BlueprintPure, meta = (Keywords = "SB"), Category = "SimpleUnrealPak")
	static class USoundBase *GetSoundBaseFromPak(const FString &Filename, ESimpleAssetFileType Type);

	UFUNCTION(BlueprintPure, meta = (Keywords = "SMesh"), Category = "SimpleUnrealPak")
	static class USkeletalMesh *GetSkeletalMeshFromPak(const FString &Filename, ESimpleAssetFileType Type);
};
