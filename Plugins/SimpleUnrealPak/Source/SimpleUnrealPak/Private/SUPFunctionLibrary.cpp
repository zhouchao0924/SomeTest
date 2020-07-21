// Fill out your copyright notice in the Description page of Project Settings.

#include "SUPFunctionLibrary.h"
#include "SimpleUnrealPak.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture2D.h"
#include "Engine/StaticMesh.h"
#include "Sound/SoundBase.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimSequence.h"

const TCHAR *GetSimpleFileType(ESimpleAssetFileType Type)
{
	switch (Type)
	{
	case DLC:
		return FSimpleAssetFileName::DLC;
	case PATCHS:
		return FSimpleAssetFileName::Patchs;
	case COMMONTPATCHS:
		return FSimpleAssetFileName::CommontPatchs;
	}

	return nullptr;
}

void USUPFunctionLibrary::InitConfig(const FSimpleUnrealPakConfig &InConfig)
{
	return SCOPE_PLATFORMFILE_ANONYMOUS.InitConfig(InConfig);
}

void USUPFunctionLibrary::GetFilenamesInPakFile(const FString& InPakFilename, TArray<FString>& OutFileList)
{
	SCOPE_PLATFORMFILE_ANONYMOUS.GetFilenamesInPakFile(InPakFilename, OutFileList);
}

FSimpleUnrealPak USUPFunctionLibrary::GetSimpleUnrealPakInfo(FString PakFilename)
{
	FSimpleUnrealPak SimpleUnrealPak;
	SCOPE_PLATFORMFILE_ANONYMOUS.GetSimpleUnrealPakInfo(PakFilename, SimpleUnrealPak);

	return SimpleUnrealPak;
}

bool USUPFunctionLibrary::MountPak(FString PakFilename,ESimpleAssetFileType Type,int32 PakOrder, FString MountPath)
{
	FSimpleUnrealPakModule &SimpleUnrealPakModule = FModuleManager::LoadModuleChecked<FSimpleUnrealPakModule>(TEXT("SimpleUnrealPak"));

	return SimpleUnrealPakModule.MountPak(PakFilename, GetSimpleFileType(Type), PakOrder,MountPath);
}

bool USUPFunctionLibrary::UnmountPak(FString PakFilename)
{
	FSimpleUnrealPakModule &SimpleUnrealPakModule = FModuleManager::LoadModuleChecked<FSimpleUnrealPakModule>(TEXT("SimpleUnrealPak"));

	return SimpleUnrealPakModule.UnmountPak(PakFilename);
}

TArray<FString> USUPFunctionLibrary::GetMountedPakFilenames()
{
	TArray<FString> FileNames;
	SCOPE_PLATFORMFILE_ANONYMOUS.GetMountedPakFilenames(FileNames);

	return FileNames;
}

bool USUPFunctionLibrary::DirectoryExists(FString PakDirectory)
{
	return SCOPE_PLATFORMFILE_ANONYMOUS.DirectoryExists(PakDirectory);
}

bool USUPFunctionLibrary::FindFile(FString Directory, TArray<FString> &File, bool bRecursively /*= true*/)
{
	return SCOPE_PLATFORMFILE_ANONYMOUS.FindFile(Directory, File, bRecursively);
}

UObject * USUPFunctionLibrary::GetObjectFromPak(const FString &Filename, ESimpleAssetFileType Type)
{
	FSimpleUnrealPakModule &SimpleUnrealPakModule = FModuleManager::LoadModuleChecked<FSimpleUnrealPakModule>(TEXT("SimpleUnrealPak"));

	return SimpleUnrealPakModule.StaticLoadObjectFromPak<UObject>(Filename, GetSimpleFileType(Type));
}

UStaticMesh * USUPFunctionLibrary::GetStaticMeshFromPak(const FString &Filename, ESimpleAssetFileType Type)
{
	FSimpleUnrealPakModule &SimpleUnrealPakModule = FModuleManager::LoadModuleChecked<FSimpleUnrealPakModule>(TEXT("SimpleUnrealPak"));

	return SimpleUnrealPakModule.StaticLoadObjectFromPak<UStaticMesh>(Filename, GetSimpleFileType(Type));
}

UTexture2D * USUPFunctionLibrary::GetTexture2DFromPak(const FString &Filename, ESimpleAssetFileType Type)
{
	FSimpleUnrealPakModule &SimpleUnrealPakModule = FModuleManager::LoadModuleChecked<FSimpleUnrealPakModule>(TEXT("SimpleUnrealPak"));

	return SimpleUnrealPakModule.StaticLoadObjectFromPak<UTexture2D>(Filename, GetSimpleFileType(Type));
}

UAnimSequence * USUPFunctionLibrary::GetAnimSequenceFromPak(const FString &Filename, ESimpleAssetFileType Type)
{
	FSimpleUnrealPakModule &SimpleUnrealPakModule = FModuleManager::LoadModuleChecked<FSimpleUnrealPakModule>(TEXT("SimpleUnrealPak"));

	return SimpleUnrealPakModule.StaticLoadObjectFromPak<UAnimSequence>(Filename, GetSimpleFileType(Type));
}

UMaterialInstanceConstant * USUPFunctionLibrary::GetMaterialInstanceConstantFromPak(const FString &Filename, ESimpleAssetFileType Type)
{
	FSimpleUnrealPakModule &SimpleUnrealPakModule = FModuleManager::LoadModuleChecked<FSimpleUnrealPakModule>(TEXT("SimpleUnrealPak"));

	return SimpleUnrealPakModule.StaticLoadObjectFromPak<UMaterialInstanceConstant>(Filename, GetSimpleFileType(Type));
}

UMaterial * USUPFunctionLibrary::GetMaterialFromPak(const FString &Filename, ESimpleAssetFileType Type)
{
	FSimpleUnrealPakModule &SimpleUnrealPakModule = FModuleManager::LoadModuleChecked<FSimpleUnrealPakModule>(TEXT("SimpleUnrealPak"));

	return SimpleUnrealPakModule.StaticLoadObjectFromPak<UMaterial>(Filename, GetSimpleFileType(Type));
}

USoundBase * USUPFunctionLibrary::GetSoundBaseFromPak(const FString &Filename, ESimpleAssetFileType Type)
{
	FSimpleUnrealPakModule &SimpleUnrealPakModule = FModuleManager::LoadModuleChecked<FSimpleUnrealPakModule>(TEXT("SimpleUnrealPak"));

	return SimpleUnrealPakModule.StaticLoadObjectFromPak<USoundBase>(Filename, GetSimpleFileType(Type));
}

USkeletalMesh * USUPFunctionLibrary::GetSkeletalMeshFromPak(const FString &Filename, ESimpleAssetFileType Type)
{
	FSimpleUnrealPakModule &SimpleUnrealPakModule = FModuleManager::LoadModuleChecked<FSimpleUnrealPakModule>(TEXT("SimpleUnrealPak"));

	return SimpleUnrealPakModule.StaticLoadObjectFromPak<USkeletalMesh>(Filename, GetSimpleFileType(Type));
}
