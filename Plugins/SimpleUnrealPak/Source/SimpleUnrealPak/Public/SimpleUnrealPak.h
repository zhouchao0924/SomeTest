// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "SimpleUnrealPakType.h"

class FPakPlatformFile;

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

class SIMPLEUNREALPAK_API FSimpleUnrealPakModule : public IModuleInterface
{
public:
	struct SIMPLEUNREALPAK_API FResetPlatformFile
	{
		friend class FSimpleUnrealPakModule;

		FResetPlatformFile();
		~FResetPlatformFile();

		FPakPlatformFile *GetPakPlatformFile();
		bool FindFile(const FString &Directory, TArray<FString> &File, bool bRecursively = true);
		bool GetSimpleUnrealPakInfo(const FString &PakFilename, FSimpleUnrealPak &SimpleUnrealPak);
		void AESDecrypt(const FString &NewAES = "", bool bConfiguration = false);
		void InitConfig(const FSimpleUnrealPakConfig &InConfig);
		bool DirectoryExists(const FString &PakDirectory);
		void GetFilenamesInPakFile(const FString& InPakFilename, TArray<FString>& OutFileList);
		void GetMountedPakFilenames(TArray<FString> &);
		bool DoesPakExist(const FString &PakFilename);
		void Refresh(const TCHAR *FileType, TArray<FSimpleUnrealAsset> &Asset);
	private:
		static FSimpleUnrealPakConfig Config;
		static FPakPlatformFile *PakPlatformFile;
		static IPlatformFile *EnginePakPlatformFile;
	};

	FSimpleUnrealPakModule();

	EMountInfor MountPak( const FString &AssetFileName,const TCHAR *Type = FSimpleAssetFileName::DLC, int32 PakOrder = INDEX_NONE, const FString &MountPath = "");
	
	bool UnmountPak(const FString &);

	TArray<FSimpleUnrealAsset> *FindCacheAsset(const TCHAR *FileType);

	bool FindCacheAsset(const FString &PakFilename,TArray<FString> &Assets, const TCHAR *FileType = FSimpleAssetFileName::PakTest);

	void ResetAsset(const TCHAR *FileType);

	bool FindAsset(FSimpleUnrealAsset *&Asset,const FString &Filename, const TCHAR *FileType = FSimpleAssetFileName::Patchs);

	//WidgetBlueprint'/Game/ThirdPersonCPP/Blueprints/NewWidgetBlueprint.NewWidgetBlueprint'
	//Blueprint'/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter.ThirdPersonCharacter'
	//Material'/Game/ThirdPerson/Meshes/RampMaterial.RampMaterial'
	//StaticMesh'/Game/ThirdPerson/Meshes/Ramp_StaticMesh.Ramp_StaticMesh'
	template<class T>
	T* StaticLoadObjectFromPak(const FString &Filename, const TCHAR *FileType = FSimpleAssetFileName::Patchs)//Filename = /Game/ThirdPerson/Meshes/RampMaterial
	{
		T *ObjectInstance = StaticLoadPakObject<T>(Filename);
		if (ObjectInstance == nullptr)
		{
			RefreshAsset(FileType);
				
			if (UpdateAsset(Filename, FileType))
			{
				ObjectInstance = Cast<T>(StaticLoadPakObject<T>(Filename));
			}
		}

		return ObjectInstance;
	}

	template<class T>
	T* StaticLoadPakObject(const FString &Filename)
	{
		const FString ObjectName = AnalysisClassName(T::StaticClass()->GetName()) + TEXT("'") + Filename + TEXT(".") + FPaths::GetCleanFilename(Filename) + TEXT("'");

		return Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *ObjectName));;
	}

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RefreshAsset(const TCHAR *FileType);

	FString AnalysisClassName(const FString &Name);

	bool UpdateAsset(const FString &Filename, const TCHAR *FileType);
	bool CheckCurrentAsset(const FString &Filename, const TCHAR *FileType, TArray<FSimpleUnrealAsset> &Asset);

	int32 GetPakOrder(const FString &PakPathsName);

	bool ReadCache(const TCHAR *FileType, TArray<FSimpleUnrealAsset> &Asset);
	void SaveCache(const TCHAR *FileType, TArray<FSimpleUnrealAsset> &Asset);

	void Print_r(const FString &Msg);
private:

	TArray<FSimpleUnrealAsset> DLC;
	TArray<FSimpleUnrealAsset> Patchs;
	TArray<FSimpleUnrealAsset> PakTest;
	TArray<FSimpleUnrealAsset> CommontPatchs;
};

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif
