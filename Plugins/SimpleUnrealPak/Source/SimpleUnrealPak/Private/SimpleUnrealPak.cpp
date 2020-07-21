// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SimpleUnrealPak.h"
#include "IPlatformFilePak.h"
#include "HAL/FileManager.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/CoreDelegates.h"
#include "Settings/SimpleUnrealPakSettings.h"
#include "Misc/App.h"
#include "Json.h"

#define LOCTEXT_NAMESPACE "FSimpleUnrealPakModule"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

FSimpleUnrealPakConfig FSimpleUnrealPakModule::FResetPlatformFile::Config;
FPakPlatformFile * FSimpleUnrealPakModule::FResetPlatformFile::PakPlatformFile = nullptr;
IPlatformFile * FSimpleUnrealPakModule::FResetPlatformFile::EnginePakPlatformFile = nullptr;

FSimpleUnrealPakModule::FSimpleUnrealPakModule()
{
	if (!FSimpleUnrealPakModule::FResetPlatformFile::PakPlatformFile)
	{
		FSimpleUnrealPakModule::FResetPlatformFile::EnginePakPlatformFile = &FPlatformFileManager::Get().GetPlatformFile();

		auto PlatformFile = FPlatformFileManager::Get().FindPlatformFile(TEXT("PakFile"));
		if (PlatformFile)
		{
			FSimpleUnrealPakModule::FResetPlatformFile::PakPlatformFile = static_cast<FPakPlatformFile*>(PlatformFile);
		}
		else
		{
			FSimpleUnrealPakModule::FResetPlatformFile::PakPlatformFile = new FPakPlatformFile();

			if (FSimpleUnrealPakModule::FResetPlatformFile::PakPlatformFile->Initialize(&FPlatformFileManager::Get().GetPlatformFile(), TEXT("")))
			{

			}
		}
	}
}

void FSimpleUnrealPakModule::FResetPlatformFile::InitConfig(const FSimpleUnrealPakConfig &InConfig)
{
	Config = InConfig;
}

FString GetShortMountPoint(const FString &PakMountPoint)
{
	//../../../ProjectName/Content/SS/TT/ //相对路径
	//PakFile.GetMountPoint() 绝对路径 : I:/CC/DD/ProjectName/Content/SS/TT/
	auto Pos = PakMountPoint.Find("Content/");
	auto NewPoint = PakMountPoint.RightChop(Pos);
	return FPaths::ProjectDir() /*+ FApp::GetProjectName()*/ / NewPoint; //../../../ProjectName/Content/SS/TT/ 
}

bool FSimpleUnrealPakModule::FResetPlatformFile::GetSimpleUnrealPakInfo(const FString &PakFilename, FSimpleUnrealPak &SimpleUnrealPak)
{
	if (FPaths::FileExists(PakFilename))
	{
		AESDecrypt(Config.AES, Config.bConfiguration);

		FPakFile PakFile(GetPakPlatformFile(), *PakFilename, false);
		if (PakFile.IsValid())
		{
			SimpleUnrealPak.MountPoint = PakFile.GetMountPoint();

			SimpleUnrealPak.bValidPakFile = PakFile.IsValid();
			SimpleUnrealPak.PakSize = PakFile.TotalSize();
			PakFile.FindFilesAtPath(SimpleUnrealPak.AssetPaths, *PakFile.GetMountPoint(), true, false, true);
			return SimpleUnrealPak.AssetPaths.Num() > 0;
		}
	}

	return false;
}

EMountInfor FSimpleUnrealPakModule::MountPak(const FString &AssetFileName, const TCHAR *Type,int32 PakOrder /*= INDEX_NONE*/, const FString &MountPath /*= ""*/)
{
	SCOPE_PLATFORMFILE(SCopePlatformFile);

	if (FPaths::FileExists(AssetFileName))
	{
		TArray<FString >PakFilenames;
		SCopePlatformFile.GetMountedPakFilenames(PakFilenames);

		bool bMountedPak = false;
		for (const auto &Tmp : PakFilenames)
		{
			if (AssetFileName == Tmp)
			{
				bMountedPak = true;
				break;
			}
		}

		auto MountFile =[&](const FString &NewMountPath)
		{
			Print_r(NewMountPath);

			if (SCopePlatformFile.GetPakPlatformFile()->Mount(*AssetFileName, GetPakOrder(NewMountPath), (NewMountPath.Len() > 0 ? *NewMountPath : nullptr)))
			{
				return EMountInfor::MOUNT_SUCCESS;
			}
			else
			{
				return EMountInfor::MOUNT_FAIL;
			}
		};

		if (Type == FSimpleAssetFileName::PakTest)
		{	
			return MountFile(GetShortMountPoint(MountPath));
		}
		else if (!bMountedPak)
		{
			SCopePlatformFile.AESDecrypt(SCopePlatformFile.Config.AES, SCopePlatformFile.Config.bConfiguration);
			if (PakOrder == INDEX_NONE)
			{
				PakOrder = GetPakOrder(AssetFileName);
			}

			FSimpleUnrealAsset *UnrealAsset = nullptr;
			if (!FindAsset(UnrealAsset, AssetFileName, Type))
			{
				ResetAsset(Type);
				if (!FindAsset(UnrealAsset, AssetFileName, Type))
				{
					return EMountInfor::FIND_NON_EXISTENT;
				}
			}

			if (UnrealAsset)
			{
				return MountFile(GetShortMountPoint(UnrealAsset->MountPoint));
			}
		}
	}

	return EMountInfor::RESOURCE_NON_EXISTENT;
}

void FSimpleUnrealPakModule::FResetPlatformFile::GetFilenamesInPakFile(const FString& InPakFilename, TArray<FString>& OutFileList)
{
	GetPakPlatformFile()->GetFilenamesInPakFile(InPakFilename, OutFileList);
}

void FSimpleUnrealPakModule::FResetPlatformFile::GetMountedPakFilenames(TArray<FString> &Filenames)
{
	GetPakPlatformFile()->GetMountedPakFilenames(Filenames);
}

bool FSimpleUnrealPakModule::UnmountPak(const FString &PakFilename)
{
	return SCOPE_PLATFORMFILE_ANONYMOUS.GetPakPlatformFile()->Unmount(*PakFilename);
}

bool FSimpleUnrealPakModule::FResetPlatformFile::DirectoryExists(const FString &PakDirectory)
{
	return GetPakPlatformFile()->DirectoryExists(*PakDirectory);
}

bool FSimpleUnrealPakModule::FResetPlatformFile::FindFile(const FString &Directory, TArray<FString> &File, bool bRecursively /*= true*/)
{
	class FSimplePakFindFile :public IPlatformFile::FDirectoryVisitor
	{
	public:
		FSimplePakFindFile(TArray<FString> &VisitFiles)
			:VisitFiles(VisitFiles)
		{}

		/**
		 * Callback for a single file or a directory in a directory iteration.
		 * @param FilenameOrDirectory		If bIsDirectory is true, this is a directory (with no trailing path delimiter), otherwise it is a file name.
		 * @param bIsDirectory				true if FilenameOrDirectory is a directory.
		 * @return							true if the iteration should continue.
		**/
		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory)
		{
			if (!bIsDirectory)
			{
				VisitFiles.Add(FilenameOrDirectory);
			}

			return true;
		}

		TArray<FString> &VisitFiles;
	};

	FSimplePakFindFile Visitor(File);

	if (bRecursively)
	{
		return GetPakPlatformFile()->IterateDirectoryRecursively(*Directory, Visitor);
	}
	
	return GetPakPlatformFile()->IterateDirectory(*Directory, Visitor);
}

void FSimpleUnrealPakModule::FResetPlatformFile::AESDecrypt(const FString &NewAES /*= ""*/, bool bConfiguration)
{
	if (!NewAES.IsEmpty())
	{
		auto PakEncryptionKey = [&](uint8 OutKey[32])
		{
			auto AESData = NewAES.GetCharArray();
			for (int32 i = 0; i < 32; i++)
			{
				OutKey[i] = AESData[i];
			}
		};

		FCoreDelegates::GetPakEncryptionKeyDelegate().BindLambda(PakEncryptionKey);
	}
	else if(bConfiguration)
	{
		auto PakEncryptionKey = [&](uint8 OutKey[32])
		{
			if (GetDefault<USimpleUnrealPakSettings>()->bAES)
			{
				auto AESData = GetDefault<USimpleUnrealPakSettings>()->AES.ToString().GetCharArray();
				for (int32 i = 0; i < 32; i++)
				{
					OutKey[i] = AESData[i];
				}
			}
		};

		FCoreDelegates::GetPakEncryptionKeyDelegate().BindLambda(PakEncryptionKey);
	}
}

bool FSimpleUnrealPakModule::FResetPlatformFile::DoesPakExist(const FString &PakFilename)
{
	TArray<FString> MyPakFile;
	GetMountedPakFilenames(MyPakFile);
	for (const auto &Tmp : MyPakFile)
	{
		if (Tmp == PakFilename)
		{
			return true;
		}
	}

	return false;
}

bool FSimpleUnrealPakModule::FindCacheAsset(const FString &PakFilename, TArray<FString> &Assets ,const TCHAR *FileType)
{
	auto Asset = FindCacheAsset(FileType);

	if (Asset)
	{
		for (const auto& Tmp : *Asset)
		{
			if (Tmp.AssetFileName == PakFilename)
			{
				Assets = Tmp.AssetPaths;

				return Assets.Num() > 0;
			}
		}
	}

	return false;
}

TArray<FSimpleUnrealAsset> * FSimpleUnrealPakModule::FindCacheAsset(const TCHAR *FileType)
{
	if (FileType == FSimpleAssetFileName::CommontPatchs)
	{
		return &CommontPatchs;
	}
	else if (FileType == FSimpleAssetFileName::Patchs)
	{
		return &Patchs;
	}
	else if (FileType == FSimpleAssetFileName::DLC)
	{
		return &DLC;
	}
	else if (FileType == FSimpleAssetFileName::PakTest)
	{
		return &PakTest;
	}

	return nullptr;
}

void FSimpleUnrealPakModule::ResetAsset(const TCHAR *FileType)
{
	SCOPE_PLATFORMFILE(SCopePlatformFile);

	if (FileType == FSimpleAssetFileName::CommontPatchs)
	{
		CommontPatchs.Empty();
		SCopePlatformFile.Refresh(FSimpleAssetFileName::CommontPatchs, CommontPatchs);
	}
	else if (FileType == FSimpleAssetFileName::Patchs)
	{
		Patchs.Empty();
		SCopePlatformFile.Refresh(FSimpleAssetFileName::Patchs, Patchs);
	}
	else if (FileType == FSimpleAssetFileName::DLC)
	{
		DLC.Empty();
		SCopePlatformFile.Refresh(FSimpleAssetFileName::DLC, DLC);
	}
	else if (FileType == FSimpleAssetFileName::PakTest)
	{
		PakTest.Empty();
		SCopePlatformFile.Refresh(FSimpleAssetFileName::PakTest, PakTest);
	}
}

bool FSimpleUnrealPakModule::FindAsset(FSimpleUnrealAsset *&Asset, const FString &Filename, const TCHAR *FileType /*= FSimpleAssetFileName::Patchs*/)
{
	auto FindUnrealAsset = [](TArray<FSimpleUnrealAsset> &Asset, const FString &Filename) ->FSimpleUnrealAsset *
	{
		for (auto &Tmp : Asset)
		{
			if (Tmp.AssetFileName == Filename)
			{
				return &Tmp;
			}
		}

		return nullptr;
	};

	if (FileType == FSimpleAssetFileName::CommontPatchs)
	{
		Asset = FindUnrealAsset(CommontPatchs, Filename);
	}
	else if (FileType == FSimpleAssetFileName::Patchs)
	{
		Asset = FindUnrealAsset(Patchs, Filename);
	}
	else if (FileType == FSimpleAssetFileName::DLC)
	{
		Asset = FindUnrealAsset(DLC, Filename);
	}
	else if (FileType == FSimpleAssetFileName::PakTest)
	{
		Asset = FindUnrealAsset(PakTest, Filename);
	}

	return Asset != nullptr;
}

void FSimpleUnrealPakModule::StartupModule()
{
	SCOPE_PLATFORMFILE(SCopePlatformFile);

	if (!ReadCache(FSimpleAssetFileName::DLC, DLC))
	{
		SCopePlatformFile.Refresh(FSimpleAssetFileName::DLC, DLC);
	}
	if (!ReadCache(FSimpleAssetFileName::Patchs, Patchs))
	{
		SCopePlatformFile.Refresh(FSimpleAssetFileName::Patchs, Patchs);
	}
	if (!ReadCache(FSimpleAssetFileName::PakTest, PakTest))
	{
		SCopePlatformFile.Refresh(FSimpleAssetFileName::PakTest, PakTest);
	}
	if (!ReadCache(FSimpleAssetFileName::CommontPatchs, CommontPatchs))
	{
		SCopePlatformFile.Refresh(FSimpleAssetFileName::CommontPatchs, CommontPatchs);
	}
}

void FSimpleUnrealPakModule::ShutdownModule()
{
	if (FSimpleUnrealPakModule::FResetPlatformFile::PakPlatformFile)
	{
		//auto UnmountMyPak = [](FPakPlatformFile *PakPlatformFile,TArray<FSimpleUnrealAsset> &Asset)
		//{
		//	for (const auto &Tmp : Asset)
		//	{
		//		PakPlatformFile->Unmount(*Tmp.AssetFileName);
		//	}
		//};

		//UnmountMyPak(PakPlatformFile, DLC);
		//UnmountMyPak(PakPlatformFile, Patchs);
		//UnmountMyPak(PakPlatformFile, PakTest);
		//UnmountMyPak(PakPlatformFile, CommontPatchs);
		
	//	FPlatformFileManager::Get().RemovePlatformFile(FSimpleUnrealPakModule::FResetPlatformFile::PakPlatformFile);

		delete FSimpleUnrealPakModule::FResetPlatformFile::PakPlatformFile;
	}

	FSimpleUnrealPakModule::FResetPlatformFile::PakPlatformFile = nullptr;

	SaveCache(FSimpleAssetFileName::DLC,DLC);
	SaveCache(FSimpleAssetFileName::Patchs, Patchs);
	SaveCache(FSimpleAssetFileName::PakTest, PakTest);
	SaveCache(FSimpleAssetFileName::CommontPatchs, CommontPatchs);
}

void FSimpleUnrealPakModule::RefreshAsset(const TCHAR *FileType)
{
	SCOPE_PLATFORMFILE(SCopePlatformFile);

	if (FileType == FSimpleAssetFileName::CommontPatchs)
	{
		SCopePlatformFile.Refresh(FSimpleAssetFileName::CommontPatchs, CommontPatchs);
	}
	else if (FileType == FSimpleAssetFileName::Patchs)
	{
		SCopePlatformFile.Refresh(FSimpleAssetFileName::Patchs, Patchs);
	}
	else if (FileType == FSimpleAssetFileName::DLC)
	{
		SCopePlatformFile.Refresh(FSimpleAssetFileName::DLC, DLC);
	}
	else if (FileType == FSimpleAssetFileName::PakTest)
	{
		SCopePlatformFile.Refresh(FSimpleAssetFileName::PakTest, PakTest);
	}
}

FString FSimpleUnrealPakModule::AnalysisClassName(const FString &Name)
{
	if (Name == TEXT("Actor"))
	{
		return TEXT("Blueprint");
	}

	return Name;
}

void FSimpleUnrealPakModule::FResetPlatformFile::Refresh(const TCHAR *FileType, TArray<FSimpleUnrealAsset> &Asset)
{
	TArray<FString >PathsFilenames;
	if (FindFile(FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / FileType ), PathsFilenames, true))
	{
		for (const auto &Tmp : PathsFilenames)
		{
			if (FPaths::GetExtension(Tmp, true) == ".pak")
			{			
				bool bDiff = true;
				for (const auto &TmpAsset : Asset)
				{
					if (Tmp == TmpAsset.AssetFileName)
					{
						bDiff = false;
						break;
					}
				}

				if (bDiff)
				{
					FSimpleUnrealPak SimpleUnrealPak;
					GetSimpleUnrealPakInfo(Tmp, SimpleUnrealPak);

					Asset.AddUnique(FSimpleUnrealAsset(Tmp, SimpleUnrealPak.AssetPaths, SimpleUnrealPak.MountPoint));
				}
			}
		}
	}
}

bool FSimpleUnrealPakModule::UpdateAsset(const FString &Filename, const TCHAR *FileType)
{
	if (FileType == FSimpleAssetFileName::CommontPatchs)
	{
		return CheckCurrentAsset(Filename,FSimpleAssetFileName::CommontPatchs, CommontPatchs);
	}
	else if (FileType == FSimpleAssetFileName::Patchs)
	{
		return CheckCurrentAsset(Filename, FSimpleAssetFileName::Patchs, Patchs);
	}
	else if (FileType == FSimpleAssetFileName::DLC)
	{
		return CheckCurrentAsset(Filename, FSimpleAssetFileName::DLC, DLC);
	}
	else if (FileType == FSimpleAssetFileName::PakTest)
	{
		return CheckCurrentAsset(Filename, FSimpleAssetFileName::PakTest, PakTest);
	}

	return false;
}

bool FSimpleUnrealPakModule::CheckCurrentAsset(const FString &Filename, const TCHAR *FileType,TArray<FSimpleUnrealAsset> &Asset)
{
	//大清除
	TArray<FSimpleUnrealAsset> RemoveAsset;
	for (auto &Tmp :Asset)
	{
		if (!Tmp.IsValid())
		{
			RemoveAsset.Add(Tmp);
		}
	}

	for (auto &Tmp: RemoveAsset)
	{
		Asset.Remove(Tmp);
	}

	if (RemoveAsset.Num())
	{
		SCOPE_PLATFORMFILE_ANONYMOUS.Refresh(FileType, Asset);
	}

	FString ExistFileName = Filename;
	ExistFileName.ReplaceInline(TEXT("/Game/"),TEXT("/Content/"));

	FString AssetFileName;
	FString MountPoint;
	for (const auto &Tmp : Asset)
	{
		for (const auto &AssetTmp: Tmp.AssetPaths)
		{
			if (AssetTmp.Find(ExistFileName) >= 0)
			{
				AssetFileName = Tmp.AssetFileName;
				MountPoint = Tmp.MountPoint;
				break;
			}
		}

		if (!AssetFileName.IsEmpty())
		{
			break;
		}
	}

	if (!AssetFileName.IsEmpty())
	{
		return EMountInfor::MOUNT_SUCCESS == MountPak(AssetFileName, FileType, GetPakOrder(MountPoint), MountPoint.Len() > 0 ? *MountPoint : nullptr);
	}
	
	return false;
}

int32 FSimpleUnrealPakModule::GetPakOrder(const FString &PakPathsName)
{
	if (PakPathsName.StartsWith(FPaths::ProjectSavedDir()))
	{
		return 1;
	}
	else if (PakPathsName.StartsWith(FPaths::EngineContentDir()))
	{
		return 2;
	}
	else if (PakPathsName.StartsWith(FPaths::ProjectContentDir()))
	{
		return 3;
	}
	else if (PakPathsName.StartsWith(FString::Printf(TEXT("%sPaks%s"), *FPaths::ProjectContentDir(),FApp::GetProjectName())))
	{
		return 4;
	}

	return 0;
}

//Field
//{
//"DLC1":
//	{
//		"DLCFileName":"F:/PJ/PluginCore/dd.pak",
//		"AssetPaths":
//			[
//				"F:/PJ/PluginCore/Content/ww/ddd.asset",
//				"F:/PJ/PluginCore/Content/ww/ddd1.asset",
//				"F:/PJ/PluginCore/Content/ww/ddd2.asset",
//				"F:/PJ/PluginCore/Content/ww/ddd3.asset",
//				"F:/PJ/PluginCore/Content/ww/ddd4.asset",
//				"F:/PJ/PluginCore/Content/ww/ddd5.asset"
//			]
//	},
//	"DLC2":
//	{
//		"DLCFileName":"F:/PJ/PluginCore/dd.pak",
//			"AssetPaths" :
//			[
//				"F:/PJ/PluginCore/Content/ww/ddd.asset",
//				"F:/PJ/PluginCore/Content/ww/ddd1.asset",
//				"F:/PJ/PluginCore/Content/ww/ddd2.asset",
//				"F:/PJ/PluginCore/Content/ww/ddd3.asset",
//				"F:/PJ/PluginCore/Content/ww/ddd4.asset",
//				"F:/PJ/PluginCore/Content/ww/ddd5.asset"
//			]
//	}
//}

bool FSimpleUnrealPakModule::ReadCache(const TCHAR *FileType, TArray<FSimpleUnrealAsset> &Asset)
{
	FString JsonValue;

	if (FFileHelper::LoadFileToString(JsonValue, *(FPaths::ProjectSavedDir() / TEXT("PakCache") / FString(FileType) + TEXT(".bin"))))
	{
		TSharedPtr<FJsonValue> JsonParsed;

		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonValue);
		if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
		{
			bool bReadSuccess = true;

			const TSharedPtr<FJsonObject> JsonObject = JsonParsed->AsObject();
			for (const auto &JsonTmp : JsonObject->Values)
			{
				FSimpleUnrealAsset UnrealAsset;
				if (!JsonTmp.Value->AsObject()->TryGetStringField(TEXT("AssetFilename"), UnrealAsset.AssetFileName))
				{
					bReadSuccess = false;
				}

				if (!JsonTmp.Value->AsObject()->TryGetStringField(TEXT("MountPoint"), UnrealAsset.MountPoint))
				{
					bReadSuccess = false;
				}

				const TArray<TSharedPtr<FJsonValue>>* JsonArrays;
				if (!JsonTmp.Value->AsObject()->TryGetArrayField(TEXT("AssetPaths"), JsonArrays))
				{
					bReadSuccess = false;
				}
				else
				{
					if (JsonArrays)
					{
						for (const auto &Tmp: *JsonArrays)
						{
							UnrealAsset.AssetPaths.Add(Tmp->AsString());
						}
					}
				}

				if (!UnrealAsset.IsValid())
				{
					bReadSuccess = false;
				}

				if (bReadSuccess)
				{
					Asset.Add(UnrealAsset);
				}
			}

			return bReadSuccess;
		}
	}

	return false;
}

void FSimpleUnrealPakModule::SaveCache(const TCHAR *FileType, TArray<FSimpleUnrealAsset> &Asset)
{
	FString JsonStr;
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);

	//{
	JsonWriter->WriteObjectStart();
	for (const auto &Tmp: Asset)
	{
		//"DLC1":{
		JsonWriter->WriteObjectStart(Tmp.GetAssetName());
		{
			//"UnrealAsset":"F:/PJ/PluginCore/dd.pak",
			JsonWriter->WriteValue(TEXT("AssetFilename"), Tmp.AssetFileName);
			JsonWriter->WriteValue(TEXT("MountPoint"), Tmp.MountPoint);
			//	"AssetPaths" :[
			JsonWriter->WriteArrayStart(TEXT("AssetPaths"));
			for (const auto& TmpPaths : Tmp.AssetPaths)
			{
				//"F:/PJ/PluginCore/Content/ww/ddd.asset",
				JsonWriter->WriteValue(TmpPaths);
			}
			//]
			JsonWriter->WriteArrayEnd();
		}
		//}
		JsonWriter->WriteObjectEnd();
	}
	//}
	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();

	if (JsonStr.Len() > 0)
	{
		FFileHelper::SaveStringToFile(JsonStr,*(FPaths::ProjectSavedDir() / TEXT("PakCache") / FString(FileType) + TEXT(".bin")), FFileHelper::EEncodingOptions::ForceUTF8);
	}
}

void FSimpleUnrealPakModule::Print_r(const FString &Msg)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, *Msg);
	}
}

FSimpleUnrealPakModule::FResetPlatformFile::FResetPlatformFile()
{
	FPlatformFileManager::Get().SetPlatformFile(*PakPlatformFile);
}

FSimpleUnrealPakModule::FResetPlatformFile::~FResetPlatformFile()
{
	FPlatformFileManager::Get().SetPlatformFile(*EnginePakPlatformFile);
}

FPakPlatformFile * FSimpleUnrealPakModule::FResetPlatformFile::GetPakPlatformFile()
{
	return PakPlatformFile;
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleUnrealPakModule, SimpleUnrealPak)
