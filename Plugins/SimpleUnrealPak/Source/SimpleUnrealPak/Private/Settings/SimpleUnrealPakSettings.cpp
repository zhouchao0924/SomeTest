// Fill out your copyright notice in the Description page of Project Settings.
#include "Settings/SimpleUnrealPakSettings.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "GenericPlatform/GenericPlatformFile.h"

USimpleUnrealPakSettings::USimpleUnrealPakSettings()
{
	if (Version.IsEmpty())
	{
		Version = TEXT("Release01");
	}

	IPlatformFile &PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	PakSavePaths.Path = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / TEXT("Pak"));
	if (!PlatformFile.DirectoryExists(*PakSavePaths.Path))
	{
		PlatformFile.CreateDirectory(*PakSavePaths.Path);
	}

	PakLogPaths.Path = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / TEXT("PakLog"));
	if (!PlatformFile.DirectoryExists(*PakLogPaths.Path))
	{
		PlatformFile.CreateDirectory(*PakLogPaths.Path);
	}
	CookOutputDir.Path = FPaths::ProjectSavedDir() / TEXT("Cooked/WindowsNoEditor") / FApp::GetProjectName() / TEXT("Content");

	TargetPlatform = ECookTargetPlatform::WindowsNoEditor;
	bSynch = false;
	bIgnoreCookErrors = false;
	bCookPartialGC = false;
	bCookInEditor = false;
	bUnversionedCookedContent = false;
	bSkipCookingEditorContent = false;
	NumCookersToSpawn = 0;
	FastCook = false;

	bCooked = true;
	bReferenceAsset = false;
	bCompress = true;
	bForceUTF8 = true;
	bAES = false;
	bOrder = false;
	bSynch = false;
}