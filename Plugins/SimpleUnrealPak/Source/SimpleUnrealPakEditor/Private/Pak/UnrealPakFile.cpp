#include "Pak/UnrealPakFile.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "AssetRegistryModule.h"
#include "Modules/ModuleManager.h"
#include "SourceCodeNavigation.h"
#include "Settings/SimpleUnrealPakSettings.h"
#include "UATHelper/Public/IUATHelperModule.h"
#include "UnrealEdMisc.h"
#include "Misc/App.h"
#include "EditorStyleSet.h"
#include "HAL/PlatformFilemanager.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

namespace SimpleUnrealPakEditor
{
	FString ProjectConfigDir()
	{
		return 	FPaths::ConvertRelativePathToFull(FPaths::ProjectConfigDir());
	}

	FString RootDir()
	{
		return FPaths::ConvertRelativePathToFull(FPaths::RootDir());
	}

	FString EngineContentDirFull()
	{
		return FPaths::ConvertRelativePathToFull(FPaths::EngineContentDir());
	}

	FString ProjectDirFull()
	{
		return FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
	}

	FString EngineDirFull()
	{
		return FPaths::ConvertRelativePathToFull(FPaths::EngineDir());
	}

	FString ProjectContentDirFull()
	{
		return FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
	}

	FString ProjectName()
	{
		return FApp::GetProjectName();
	}
}

void CreateAssetText(const TArray<FString> &ContentAssetPaths, FString &AssetString, bool bCook = true)
{
	FString EngineDir = SimpleUnrealPakEditor::EngineContentDirFull();
	FString ProjectDir = SimpleUnrealPakEditor::ProjectDirFull();

	auto IsDir = [](const FString &ConstTmpPaths,const FString &String) ->bool
	{
		return ConstTmpPaths.Find(String) >= 0;
	};

	auto ReplaceContent = [&AssetString](const FString &ConstTmpPaths, const FString &CookContent,const FString &Extenstion,bool bEngine = false)
	{
		FString UAssetPaths = ConstTmpPaths.Replace(*(bEngine ? SimpleUnrealPakEditor::RootDir() : SimpleUnrealPakEditor::ProjectDirFull()), *CookContent);
		AssetString += UAssetPaths + TEXT("\r\n");

		FString UExpPaths = UAssetPaths.Replace(*Extenstion, TEXT(".uexp"));
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*UExpPaths))
		{
			AssetString += UExpPaths + TEXT("\r\n");
		}

		FString UbulkPaths = UAssetPaths.Replace(*Extenstion, TEXT(".ubulk"));
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*UbulkPaths))
		{
			AssetString += UbulkPaths + TEXT("\r\n");
		}
	};

	for (const auto &ConstTmpPaths : ContentAssetPaths)
	{
		if (bCook)
		{
			FString GameCookContent = ProjectDir + TEXT("Saved/Cooked/WindowsNoEditor") / SimpleUnrealPakEditor::ProjectName() / TEXT("");
			FString EngineCookContent = ProjectDir + TEXT("Saved/Cooked/WindowsNoEditor/");

			FString Extenstion = FPaths::GetExtension(ConstTmpPaths, true);
			if (IsDir(ConstTmpPaths, EngineDir))
			{
				ReplaceContent(ConstTmpPaths, EngineCookContent, Extenstion,true);
			}
			else if (IsDir(ConstTmpPaths, ProjectDir))
			{
				ReplaceContent(ConstTmpPaths, GameCookContent, Extenstion);
			}
			else
			{
				check(0);
			}
		}
		else
		{
			AssetString += ConstTmpPaths + TEXT("\r\n");
		}
	}
}

void ConvertPackagenameToFilename(const TArray<FString> &PackageNames, TArray<FString> &OutFilename)
{
	const TCHAR *ScriptStirng = TEXT("/Script/");
	const TCHAR *EngineStirng = TEXT("/Engine/");
	for (auto Tmp : PackageNames)
	{
		if (Tmp.StartsWith(EngineStirng))
		{
			continue;
		}
		else if (Tmp.StartsWith(ScriptStirng))
		{
			continue;
		}
		else
		{
			//.uaseet
			FString AssetPackageName = FPackageName::LongPackageNameToFilename(Tmp, FPackageName::GetAssetPackageExtension());
			Tmp = FPaths::ConvertRelativePathToFull(AssetPackageName);
		}

		OutFilename.AddUnique(Tmp);
	}

	TArray<FString> RemoveMap;
	for (const auto &Tmp : OutFilename)
	{
		FString Extension = FPaths::GetExtension(Tmp);
		if (FPackageName::GetMapPackageExtension().Contains(Extension))
		{
			RemoveMap.AddUnique(Tmp);
		}
	}

	//.uasset
	for (auto &Tmp: RemoveMap)
	{
		if (Tmp.RemoveFromEnd(*FPackageName::GetMapPackageExtension()))
		{
			OutFilename.Remove(Tmp + FPackageName::GetAssetPackageExtension());
		}	
	}
}

void RecursiveFindDependecies(const FString &PackageName,TArray<FString> &AllDependecies)
{
	FAssetRegistryModule &AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	TArray<FName> DependeciesArray;
	AssetRegistryModule.GetDependencies(*PackageName, DependeciesArray, EAssetRegistryDependencyType::Packages);

	for (auto &Tmp : DependeciesArray)
	{
		if (Tmp.IsValid())
		{
			FString DependeciesString = Tmp.ToString();
			if (!AllDependecies.Contains(DependeciesString))
			{
				AllDependecies.AddUnique(DependeciesString);
				RecursiveFindDependecies(DependeciesString, AllDependecies);
			}
		}
	}
}

FString ConvertPlatformToString(EUnrealPakSettingsPlatform Platform)
{
	switch (Platform)
	{
	case EUnrealPakSettingsPlatform::Windows:
		return TEXT("Windows");
	case EUnrealPakSettingsPlatform::Linux:
		return TEXT("Linux");
	case EUnrealPakSettingsPlatform::Mac:
		return TEXT("Mac");
	case EUnrealPakSettingsPlatform::Android:
		return TEXT("Android");
	case EUnrealPakSettingsPlatform::iOS:
		return TEXT("iOS");
	case EUnrealPakSettingsPlatform::HTML5:
		return TEXT("HTML5");
	}

	return "Windows";
}

FProcHandle GeneratedPak(const TArray<FString> &ContentAssetPaths, const FString &ConstTmpPaths)
{
	FString SavePakFile = GetDefault<USimpleUnrealPakSettings>()->PakSavePaths.Path;

	//生成我们的资源表
	FString AssetString;
	CreateAssetText(ContentAssetPaths, AssetString, GetDefault<USimpleUnrealPakSettings>()->bCooked);
	FString AssetStringPaths = FPaths::ProjectSavedDir() / FGuid::NewGuid().ToString() + TEXT(".txt");
	FFileHelper::SaveStringToFile(AssetString, *AssetStringPaths, FFileHelper::EEncodingOptions::ForceUTF8);

	//拼接我们的UnrealPak命令
	FString UnrealPakExePaths = SimpleUnrealPakEditor::EngineDirFull() + TEXT("Binaries/Win64/UnrealPak.exe");
	FString PakNamePaths = SavePakFile / FPaths::GetCleanFilename(ConstTmpPaths) + TEXT(".pak ");

	//是否压缩
	FString CompressParm = "";
	if (GetDefault<USimpleUnrealPakSettings>()->bCompress)
	{
		CompressParm = TEXT(" -compress");
	}

	//是否AES加密
	FString AESParm = "";
	if (GetDefault<USimpleUnrealPakSettings>()->bAES)
	{
		AESParm = TEXT(" -encrypt -encryptindex -aes=") + GetDefault<USimpleUnrealPakSettings>()->AES.ToString();
	}

	//编码格式
	FString UTF8Parm = "";
	if (GetDefault<USimpleUnrealPakSettings>()->bForceUTF8)
	{
		UTF8Parm = TEXT(" -UTF8Output");
	}

	//log
	FString AbslogParm = "";
	{
		AbslogParm = TEXT(" -abslog=") + GetDefault<USimpleUnrealPakSettings>()->PakLogPaths.Path / TEXT("SimpleUnrealPakLog.txt");
	}

	FString OrderParm = "";
	if (GetDefault<USimpleUnrealPakSettings>()->bOrder)
	{
		OrderParm = TEXT(" -installed -order=") + FPaths::ProjectDir() / TEXT("Build/WindowsNoEditor/FileOpenOrder/CookerOpenOrder.log");
	}

	//打包平台
	FString PakPlatformParam = TEXT(" -platform=") + ConvertPlatformToString(GetDefault<USimpleUnrealPakSettings>()->PakPlatform);

	//创建参数的集合
	FString CreateParm = PakNamePaths + TEXT("-create=") + AssetStringPaths + AESParm + AbslogParm + CompressParm + UTF8Parm + PakPlatformParam + OrderParm + TEXT(" -multiprocess -patchpaddingalign=2048");

	FProcHandle Handle = FPlatformProcess::CreateProc(*UnrealPakExePaths, *CreateParm, false, false, false, nullptr, 0, nullptr, nullptr);
	if (GetDefault<USimpleUnrealPakSettings>()->bSynch)
	{
		FPlatformProcess::WaitForProc(Handle);
	}

	return Handle;
}

TArray<FProcHandle> SimpleUnrealPakEditor::FPak::PakFile(const TArray<FString> &Paths)
{
	TArray<FProcHandle> ProcHandle;

	for (const auto &ConstTmpPaths: Paths)
	{
		FString CoypeTmpPaths = ConstTmpPaths;
		if (CoypeTmpPaths.RemoveFromStart(TEXT("/Game/")))
		{
			CoypeTmpPaths = SimpleUnrealPakEditor::ProjectContentDirFull() / CoypeTmpPaths;
			
			//寻找该文件下的所有子文件
			TArray<FString> ContentAssetPaths;
			IFileManager::Get().FindFilesRecursive(ContentAssetPaths, *CoypeTmpPaths, TEXT("*"), true, false);

			//递归寻找依赖
			if (GetDefault<USimpleUnrealPakSettings>()->bReferenceAsset)
			{	
				TArray<FString> AllDependeciesAsset;
				for (auto &TmpString : ContentAssetPaths)
				{
					FString PackageName = FPackageName::FilenameToLongPackageName(TmpString);	

					RecursiveFindDependecies(PackageName, AllDependeciesAsset);			
				}

				ConvertPackagenameToFilename(AllDependeciesAsset, ContentAssetPaths);
			}

			ProcHandle.Add(GeneratedPak(ContentAssetPaths, ConstTmpPaths));
		}
	}

	return ProcHandle;
}

FString CoverCookTargetPlatformToString(ECookTargetPlatform TargetPlatform)
{
	FString TargetPlatformStr = TEXT("WindowsNoEditor");
	switch (TargetPlatform)
	{
	case ECookTargetPlatform::AllDesktop:
		TargetPlatformStr = TEXT("AllDesktop");
		break;
	case ECookTargetPlatform::Android:
		TargetPlatformStr = TEXT("Android");
		break;
	case ECookTargetPlatform::Android_ASTC:
		TargetPlatformStr = TEXT("Android_ASTC");
		break;
	case ECookTargetPlatform::Android_ASTCClient:
		TargetPlatformStr = TEXT("Android_ASTCClient");
		break;
	case ECookTargetPlatform::Android_ATC:
		TargetPlatformStr = TEXT("Android_ATC");
		break;
	case ECookTargetPlatform::Android_ATCClient:
		TargetPlatformStr = TEXT("Android_ATCClient");
		break;
	case ECookTargetPlatform::Android_DXT:
		TargetPlatformStr = TEXT("Android_DXT");
		break;
	case ECookTargetPlatform::Android_DXTClient:
		TargetPlatformStr = TEXT("Android_DXTClient");
		break;
	case ECookTargetPlatform::Android_ETC1:
		TargetPlatformStr = TEXT("Android_ETC1");
		break;
	case ECookTargetPlatform::Android_ETC1a:
		TargetPlatformStr = TEXT("Android_ETC1a");
		break;
	case ECookTargetPlatform::Android_ETC1aClient:
		TargetPlatformStr = TEXT("Android_ETC1aClient");
		break;
	case ECookTargetPlatform::Android_ETC1Client:
		TargetPlatformStr = TEXT("Android_ETC1Client");
		break;
	case ECookTargetPlatform::Android_ETC2:
		TargetPlatformStr = TEXT("Android_ETC2");
		break;
	case ECookTargetPlatform::Android_ETC2Client:
		TargetPlatformStr = TEXT("Android_ETC2Client");
		break;
	case ECookTargetPlatform::Android_Multi:
		TargetPlatformStr = TEXT("Android_Multi");
		break;
	case ECookTargetPlatform::Android_MultiClient:
		TargetPlatformStr = TEXT("Android_MultiClient");
		break;
	case ECookTargetPlatform::Android_PVRTC:
		TargetPlatformStr = TEXT("Android_PVRTC");
		break;
	case ECookTargetPlatform::Android_PVRTCClient:
		TargetPlatformStr = TEXT("Android_PVRTCClient");
		break;
	case ECookTargetPlatform::AndroidClient:
		TargetPlatformStr = TEXT("AndroidClient");
		break;
	case ECookTargetPlatform::HTML5:
		TargetPlatformStr = TEXT("HTML5");
		break;
	case ECookTargetPlatform::IOS:
		TargetPlatformStr = TEXT("IOS");
		break;
	case ECookTargetPlatform::IOSClient:
		TargetPlatformStr = TEXT("IOSClient");
		break;
	case ECookTargetPlatform::Linux:
		TargetPlatformStr = TEXT("Linux");
		break;
	case ECookTargetPlatform::LinuxClient:
		TargetPlatformStr = TEXT("LinuxClient");
		break;
	case ECookTargetPlatform::LinuxNoEditor:
		TargetPlatformStr = TEXT("LinuxNoEditor");
		break;
	case ECookTargetPlatform::LinuxServer:
		TargetPlatformStr = TEXT("LinuxServer");
		break;
	case ECookTargetPlatform::Lumin:
		TargetPlatformStr = TEXT("Lumin");
		break;
	case ECookTargetPlatform::LuminClient:
		TargetPlatformStr = TEXT("LuminClient");
		break;
	case ECookTargetPlatform::Mac:
		TargetPlatformStr = TEXT("Mac");
		break;
	case ECookTargetPlatform::MacClient:
		TargetPlatformStr = TEXT("MacClient");
		break;
	case ECookTargetPlatform::MacNoEditor:
		TargetPlatformStr = TEXT("MacNoEditor");
		break;
	case ECookTargetPlatform::MacServer:
		TargetPlatformStr = TEXT("MacServer");
		break;
	case ECookTargetPlatform::TVOS:
		TargetPlatformStr = TEXT("TVOS");
		break;
	case ECookTargetPlatform::TVOSClient:
		TargetPlatformStr = TEXT("TVOSClient");
		break;
	case ECookTargetPlatform::Windows:
		TargetPlatformStr = TEXT("Windows");
		break;
	case ECookTargetPlatform::WindowsClient:
		TargetPlatformStr = TEXT("WindowsClient");
		break;
	case ECookTargetPlatform::WindowsNoEditor:
		TargetPlatformStr = TEXT("WindowsNoEditor");
		break;
	case ECookTargetPlatform::WindowsServer:
		TargetPlatformStr = TEXT("WindowsServer");
		break;
	}

	return TargetPlatformStr;
}

TArray<FProcHandle> SimpleUnrealPakEditor::FPak::PakMapFile(const TArray<FString> &Paths)
{
	TArray<FProcHandle> ProcHandles;
	for (auto &Packagename: Paths)
	{
		TArray<FString> MapDependeciesAsset;

		//Packagename = /Game/SS/TestMap
		//../../Game/SS/TestMap.umap 
		FString MapPackageName = FPackageName::LongPackageNameToFilename(Packagename, FPackageName::GetMapPackageExtension());

		//C:ProjectName/Content/SS/TestMap.umap 
		MapDependeciesAsset.Add(FPaths::ConvertRelativePathToFull(MapPackageName));

		TArray<FString> AllDependeciesAsset;
		RecursiveFindDependecies(Packagename, AllDependeciesAsset);

		ConvertPackagenameToFilename(AllDependeciesAsset, MapDependeciesAsset);

		ProcHandles.Add(GeneratedPak(MapDependeciesAsset, Packagename));
	}

	return ProcHandles;
}

void RecursiveModifyPaths(const TCHAR *FullPaths,const TCHAR *LongFilename,TArray<FString> &ModifyPaths, const TArray<FString> &ComparePaths)
{
	TArray<FString> ContentAssetPaths;
	IFileManager::Get().FindFiles(ContentAssetPaths, FullPaths, false, true);
	TArray<FString> RemoveIndex;
	TArray<FString> RecursiveFind;
	for (auto &Tmp : ContentAssetPaths)
	{
		Tmp = LongFilename + Tmp;
		for (const auto &TmpPaths : ComparePaths)
		{
			if (Tmp == TmpPaths)
			{
				RemoveIndex.AddUnique(Tmp);
			}
			else if (TmpPaths.Contains(Tmp))
			{
				RecursiveFind.AddUnique(Tmp);
				RemoveIndex.AddUnique(Tmp);
			}
		}
	}

	//移除重复的路径
	for (const auto &Tmp : RemoveIndex)
	{
		ContentAssetPaths.Remove(Tmp);
	}

	for (const auto &Tmp : RecursiveFind)
	{
		FString FindPaths = SimpleUnrealPakEditor::ProjectDirFull() + (Tmp.Replace(TEXT("/Game/"), TEXT("Content/"))) + TEXT("/*");
		RecursiveModifyPaths(*FindPaths,*(Tmp + TEXT("/")),ModifyPaths, ComparePaths);
	}

	ModifyPaths.Append(ContentAssetPaths);
}

void CookMap(const FString& InMap)
{
	FString ProjectPath = FPaths::IsProjectFilePathSet() ? FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath()) : FPaths::RootDir() / FApp::GetProjectName() / FApp::GetProjectName() + TEXT(".uproject");
	FString CommandLine = FString::Printf(TEXT("BuildCookRun %s%s -nop4 -project=\"%s\" -cook -skipstage -map=%s -ue4exe=%s -utf8output"),
		TEXT("-nocompile -nocompileeditor"),
		FApp::IsEngineInstalled() ? TEXT(" -installed") : TEXT(""),
		*ProjectPath,
		*InMap,
		*FUnrealEdMisc::Get().GetExecutableForCommandlets()
	);

	IUATHelperModule::Get().CreateUatTask(CommandLine, FText::FromString(TEXT("Windows")), FText::FromString("Cooking content"), FText::FromString("Cooking"), FEditorStyle::GetBrush(TEXT("MainFrame.CookContent")));
}

FProcHandle SimpleUnrealPakEditor::FPak::Cook(const TArray<FString> &MapPaths, bool bMap)
{
	FProcHandle Handle;

	if (MapPaths.Num())
	{
		bool bCookAllContent = MapPaths[0] == TEXT("/Game");

		auto ExePath = SimpleUnrealPakEditor::RootDir() + TEXT("Engine/Binaries/Win64/UE4Editor-Cmd.exe");// -NoLogTimes -fileopenlog -unversioned -stdout -CrashForUAT -unattended -iterate
		auto Parm = SimpleUnrealPakEditor::ProjectDirFull() + FApp::GetProjectName() + TEXT(".uproject -run=Cook -CrashForUAT");

		//设置目标平台
		FString TargetPlatformPram = CoverCookTargetPlatformToString(GetDefault<USimpleUnrealPakSettings>()->TargetPlatform);
		Parm += (TEXT(" -TargetPlatform=") + TargetPlatformPram);

		//添加额外的命令
		if (!GetDefault<USimpleUnrealPakSettings>()->AdditionalCookerOptions.IsEmpty())
		{
			FString AdditionalCookerOptions = GetDefault<USimpleUnrealPakSettings>()->AdditionalCookerOptions;
			AdditionalCookerOptions.TrimStartAndEndInline();
			Parm += TEXT(" -AdditionalCookerOptions=") + AdditionalCookerOptions;
		}

		//强制UTF8
		if (GetDefault<USimpleUnrealPakSettings>()->bForceUTF8)
		{
			Parm += TEXT(" -UTF8Output");
		}

		if (GetDefault<USimpleUnrealPakSettings>()->bCookPartialGC)
		{
			Parm += TEXT(" -CookPartialGC");
		}

		if (!bCookAllContent)
		{
			TArray<FString> DefaultGame;
			FFileHelper::LoadFileToStringArray(DefaultGame,*(SimpleUnrealPakEditor::ProjectConfigDir() + TEXT("DefaultGame.ini")));

			TArray<FString> RemoveIndex;
			int32 ProjectPackagingSettings = INDEX_NONE;
			for (int32 i=0;i < DefaultGame.Num();i++)
			{
				if (DefaultGame[i].Contains("+DirectoriesToNeverCook=(Path=") ||
					DefaultGame[i].Contains("+DirectoriesToAlwaysCook=(Path=") || 
					DefaultGame[i].Contains("+MapsToCook=(FilePath="))
				{
					RemoveIndex.Add(DefaultGame[i]);
				}
				else if(DefaultGame[i] == TEXT("[/Script/UnrealEd.ProjectPackagingSettings]"))
				{
					ProjectPackagingSettings = i;
				}
				else if (DefaultGame[i] == TEXT("bCookMapsOnly=True"))
				{
					DefaultGame[i] = TEXT("bCookMapsOnly=False");
				}
				else if (DefaultGame[i] == TEXT("bCookAll=True"))
				{
					DefaultGame[i] = TEXT("bCookAll=False");
				}
			}

			//Remove it
			for (const auto &Tmp  : RemoveIndex)
			{
				DefaultGame.Remove(Tmp);
			}

			if (ProjectPackagingSettings == INDEX_NONE)
			{
				ProjectPackagingSettings = DefaultGame.Num();
				DefaultGame.Add("[/Script/UnrealEd.ProjectPackagingSettings]");
			}

			if (bMap)
			{
				for (const auto &Tmp : MapPaths)
				{
					DefaultGame.Add(TEXT("+MapsToCook=(FilePath=\"") + Tmp + TEXT("\")"));
				}

				for (auto &Tmp : DefaultGame)
				{
					if (Tmp == TEXT("bCookMapsOnly=False"))
					{
						Tmp = TEXT("bCookMapsOnly=True");
					}
					if (Tmp == TEXT("bCookAll=False"))
					{
						Tmp = TEXT("bCookAll=True");
					}
				}
			}
			else
			{
				TArray<FString> DirectoriesToNeverCook;
				RecursiveModifyPaths(*(SimpleUnrealPakEditor::ProjectContentDirFull() + TEXT("*")), TEXT("/Game/"), DirectoriesToNeverCook, MapPaths);

				//Directories To Never Cook
				for (const auto &Tmp : DirectoriesToNeverCook)
				{
					FString TmpDir = "+DirectoriesToNeverCook=(Path=\"" + Tmp + TEXT("\")");
					DefaultGame.Insert(TmpDir, ++ProjectPackagingSettings);
				}

				//Cook
				for (const auto &Tmp : MapPaths)
				{
					FString TmpDir = "+DirectoriesToAlwaysCook=(Path=\"" + Tmp + TEXT("\")");
					DefaultGame.Insert(TmpDir, ++ProjectPackagingSettings);
				}
			}

			FFileHelper::SaveStringArrayToFile(DefaultGame, *(SimpleUnrealPakEditor::ProjectConfigDir() + TEXT("DefaultGame.ini")),FFileHelper::EEncodingOptions::ForceUTF8);
		}
		//
		Handle = FPlatformProcess::CreateProc(*ExePath, *Parm, false, false, false, nullptr, 0, nullptr, nullptr);
		if (GetDefault<USimpleUnrealPakSettings>()->bSynch)
		{
			FPlatformProcess::WaitForProc(Handle);
		}
	}

	return Handle;
}


//
//bool SaveToCookAsset(UPackage * InPackage, const FString & InSavePath)
//{
//	ITargetPlatformManagerModule& TPM = GetTargetPlatformManagerRef();
//	const TArray<ITargetPlatform*>& TargetPlatforms = TPM.GetTargetPlatforms();
//	ITargetPlatform* Target = nullptr;
//
//	for (ITargetPlatform *TargetPlatform : TargetPlatforms)
//	{
//		if (TargetPlatform->PlatformName() == "WindowsNoEditor")
//		{
//			Target = TargetPlatform;
//			break;
//		}
//	}
//
//	InPackage->SetPackageFlags(PKG_FilterEditorOnly);
//	uint32 saveFlags = SAVE_KeepGUID | SAVE_Async | SAVE_Unversioned;
//	FSavePackageResultStruct Result = GEditor->Save(InPackage, nullptr, RF_Public, *InSavePath, GError, NULL, false, false, saveFlags, Target, FDateTime::MinValue(), false);
//
//	return Result == ESavePackageResult::Success;
//}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif