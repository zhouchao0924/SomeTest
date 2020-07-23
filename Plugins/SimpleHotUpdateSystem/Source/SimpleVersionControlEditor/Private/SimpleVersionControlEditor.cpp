// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SimpleVersionControlEditor.h"
#include "SimpleVCEditorStyle.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Settings/SimpleVCSettings.h"
#include "ISettingsModule.h"
#include "ContentBrowserModule.h"
#include "SimpleUnrealPakEditor.h"
#include "HAL/PlatformFilemanager.h"
#include "Settings/SimpleUnrealPakSettings.h"
#include "SimpleOSSManage.h"
#include "Misc/FileHelper.h"
#include "Field/VersionControlInfo.h"
#include "SimpleVCEditorCommands.h"
#include "LevelEditor.h"
#include "Slate/SVersionControlWdiget.h"
#include "Modules/ModuleManager.h"

static const FName SimpleHotUploadEditorTabName("SimpleHotUploadEditor");

#define LOCTEXT_NAMESPACE "FSimpleVersionControlEditorModule"

void FSimpleVersionControlEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSimpleVCEditorStyle::Initialize();
	FSimpleVCEditorStyle::ReloadTextures();
	FSimpleVCEditorCommands::Register();

	if (ISettingsModule * SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>(TEXT("Settings")))
	{
		SettingsModule->RegisterSettings("Project", "ProjectPak", "VersionControl",
			LOCTEXT("VersionControlSettings", "Simple Version Control Settings."),
			LOCTEXT("VersionControlSettingsTip", "Some operations on Version Control."),
			GetMutableDefault<USimpleVCSettings>());

		SettingsModule->RegisterSettings("Project", "ProjectPak", "VersionControlInfo",
			LOCTEXT("VersionControlInfo", "Simple Version Control Settings."),
			LOCTEXT("VersionControlInfoTip", "Some operations on Version Control."),
			GetMutableDefault<UVersionControlInfo>());
	}
	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FSimpleVCEditorCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FSimpleVersionControlEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FSimpleVersionControlEditorModule::AddToolbarExtension));
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	{
		FContentBrowserModule &ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
		TArray<FContentBrowserMenuExtender_SelectedPaths> &ContentBrowserMenuExtender_SelectedPaths = ContentBrowserModule.GetAllPathViewContextMenuExtenders();
		ContentBrowserMenuExtender_SelectedPaths.Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this, &FSimpleVersionControlEditorModule::OnExtendContentBrowser));

		TArray<FContentBrowserMenuExtender_SelectedAssets> &ContentBrowserMenuExtender_SelectedAssets = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
		ContentBrowserMenuExtender_SelectedAssets.Add(FContentBrowserMenuExtender_SelectedAssets::CreateRaw(this, &FSimpleVersionControlEditorModule::OnExtendContentAssetBrowser));
	}

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SimpleHotUploadEditorTabName, FOnSpawnTab::CreateRaw(this, &FSimpleVersionControlEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("SimpleVersionControlEditorModule", "SimpleVersionControlEditor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}


TSharedRef<class SDockTab> FSimpleVersionControlEditorModule::OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FSimpleUnrealPakViewModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("SimpleUnrealPakView.cpp"))
	);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				CreateEditor()
			]
		];
}

TSharedRef<SWidget> FSimpleVersionControlEditorModule::CreateEditor()
{
	UVersionControlInfo *Info = const_cast<UVersionControlInfo *>(GetDefault<UVersionControlInfo>());
	USimpleUnrealPakSettings *PakSettings = const_cast<USimpleUnrealPakSettings*>(GetDefault<USimpleUnrealPakSettings>());
	USimpleVCSettings *VCSettings = const_cast<USimpleVCSettings*>(GetDefault<USimpleVCSettings>());

	return SNew(SVersionControlWdiget, Info, PakSettings, VCSettings);
}


void FSimpleVersionControlEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(SimpleHotUploadEditorTabName);
}

void FSimpleVersionControlEditorModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FSimpleVCEditorCommands::Get().PluginAction);
}

TSharedRef<FExtender> FSimpleVersionControlEditorModule::OnExtendContentBrowser(const TArray<FString> &NewPaths)
{
	TSharedRef<FExtender> Extender(new FExtender);

	Extender->AddMenuExtension("FolderContext", EExtensionHook::Before, nullptr, FMenuExtensionDelegate::CreateRaw(this, &FSimpleVersionControlEditorModule::CreateSuMenuForContentBrowser, NewPaths, ESimpleSuMenuType::CONTENT));

	return Extender;
}

TSharedRef<FExtender> FSimpleVersionControlEditorModule::OnExtendContentAssetBrowser(const TArray<FAssetData> &AssetData)
{
	TSharedRef<FExtender> Extender(new FExtender);
	TArray<FString> NewPaths;
	for (const auto &Tmp : AssetData)
	{
		if (Tmp.AssetClass == "World")
		{
			NewPaths.Add(Tmp.PackageName.ToString());
		}
		else
		{
			return Extender;
		}
	}

	Extender->AddMenuExtension("AssetContextReferences", EExtensionHook::Before, nullptr, FMenuExtensionDelegate::CreateRaw(this, &FSimpleVersionControlEditorModule::CreateSuMenuForContentBrowser, NewPaths, ESimpleSuMenuType::ASSET));

	return Extender;
}


void FSimpleVersionControlEditorModule::CreateSuMenuForContentBrowser(FMenuBuilder& MunuBuilder, TArray<FString> NewPaths, ESimpleSuMenuType SuMenuType)
{
	MunuBuilder.BeginSection("NewServerVersion ", LOCTEXT("NewServerVersion", "New server version."));
	{
		bool bGame = NewPaths[0] == TEXT("/Game");
		bool bAsset = SuMenuType == ESimpleSuMenuType::ASSET;
		if (!bGame)
		{
			FString DLCString;
			FString PatchString;
			if (bAsset)
			{
				DLCString = TEXT("Upload DLC About Maps");
				PatchString = TEXT("Upload Patch About Maps");
			}
			else
			{
				DLCString = TEXT("Upload DLCs");
				PatchString = TEXT("Upload Patch");
			}

			MunuBuilder.AddMenuEntry(
				FText::Format(LOCTEXT("DLC", "{0}"), FText::FromString(DLCString)),
				LOCTEXT("DLCTips", "Click update our data. If you don't want to update automatically, you can set it to manual in setting ."),
				FSlateIcon(),
				FUIAction(FExecuteAction::CreateRaw(this, &FSimpleVersionControlEditorModule::NewServerDLCVersion, NewPaths, SuMenuType)));
		
			MunuBuilder.AddMenuEntry(
				FText::Format(LOCTEXT("Patch", "{0}"), FText::FromString(PatchString)),
				LOCTEXT("PatchTips", "Click update our data. If you don't want to update automatically, you can set it to manual in setting ."),
				FSlateIcon(),
				FUIAction(FExecuteAction::CreateRaw(this, &FSimpleVersionControlEditorModule::NewServerPatchVersion, NewPaths, SuMenuType)));
		}
	}
	MunuBuilder.EndSection();
}


void FSimpleVersionControlEditorModule::NewServerPatchVersion(TArray<FString> NewPaths, ESimpleSuMenuType SuMenuType)
{
	ClearPakCache();

	FSimpleUnrealPakEditorModule * PakEditor = FModuleManager::GetModulePtr<FSimpleUnrealPakEditorModule>(TEXT("SimpleUnrealPakEditor"));
	if (PakEditor)
	{
		PakEditor->Pak(NewPaths, SuMenuType, FSimpleProcessDelegate::CreateRaw(this,&FSimpleVersionControlEditorModule::RegisteringPatchCallbacks));
	}
}


void FSimpleVersionControlEditorModule::NewServerDLCVersion(TArray<FString> NewPaths, ESimpleSuMenuType SuMenuType)
{
	ClearPakCache();

	FSimpleUnrealPakEditorModule * PakEditor = FModuleManager::GetModulePtr<FSimpleUnrealPakEditorModule>(TEXT("SimpleUnrealPakEditor"));
	if (PakEditor)
	{
		PakEditor->Pak(NewPaths, SuMenuType, FSimpleProcessDelegate::CreateRaw(this, &FSimpleVersionControlEditorModule::RegisteringDLCCallbacks));
	}
}


void FSimpleVersionControlEditorModule::RegisteringPatchCallbacks(int32 ProgramIndex)
{
	RegisteringCallbacks(ProgramIndex);
}

void FSimpleVersionControlEditorModule::RegisteringDLCCallbacks(int32 ProgramIndex)
{
	if (ProgramIndex == 0)
	{
		if (GetDefault<USimpleVCSettings>()->bAuto)
		{
			if (GetDefault<USimpleVCSettings>()->DataServerType == EUploadServerType::OSSServer)
			{
				TArray<FDLCInfor> Version;

				const FString BucketName = GetDefault<USimpleVCSettings>()->Bucket;
				const FString PakLocalPaths = GetDefault<USimpleUnrealPakSettings>()->PakSavePaths.Path;

				TArray<FString> PakPaths;
				IFileManager::Get().FindFilesRecursive(PakPaths, *PakLocalPaths, TEXT("*"), true, false);

				FString URL = TEXT("https:/") / BucketName + TEXT(".") + SIMPLE_OSS.GetEndpoint();

				for (const auto &Tmp : PakPaths)
				{
					FDLCInfor DLCInfor;
					FString DLCName = FPaths::GetCleanFilename(Tmp);
					DLCInfor.Name = DLCName;
					DLCInfor.Name.RemoveFromEnd(TEXT(".pak"));
					DLCInfor.Crc = FGuid::NewGuid().ToString();

					//Patch/4.27/hello.pak
					FString ObjectName = TEXT("DLC") / DLCName;
					DLCInfor.Link = URL / ObjectName;
					if (SIMPLE_OSS.PutObject(BucketName, Tmp, ObjectName))
					{
						Version.Add(DLCInfor);
					}
				}

				//上传版本信息
				if (Version.Num())
				{
					for (const auto &NewDLC : Version)
					{
						FString JsonString;
						SimpleVersionControl::Save(NewDLC, JsonString);

						FString DLCVersionPaths = GetDefault<USimpleVCSettings>()->DLCVersionPaths.ToString();
						
						//上传当前版本对应的信息
						if (SIMPLE_OSS.PutObjectByMemory(BucketName, JsonString, DLCVersionPaths / NewDLC.Name + TEXT(".con")))
						{
							int32 OldNumber = DLCVersionListCache.Num();
							DLCVersionListCache.AddUnique(NewDLC.Name);
							if (DLCVersionListCache.Num() > OldNumber)
							{
								JsonString.Empty();
								SimpleVersionControl::Save(DLCVersionListCache, JsonString);
								FString DLCVersionListName = GetDefault<USimpleVCSettings>()->DLCVersionListName.ToString();
								if (SIMPLE_OSS.PutObjectByMemory(BucketName, JsonString, DLCVersionListName))
								{

								}
							}
						}
					}	
				}
			}
		}
	}
}


void FSimpleVersionControlEditorModule::RegisteringMainVersionCallbacks(int32 ProgramIndex /*= 0*/)
{
	RegisteringCallbacks(ProgramIndex, true);
}

void FSimpleVersionControlEditorModule::InitVersionList()
{
	const FString BucketName = GetDefault<USimpleVCSettings>()->Bucket;

	//获取服务器关于补丁的版本列表
	{
		FString JsonString;
		FString PatchVersionListName = GetDefault<USimpleVCSettings>()->PatchVersionListName.ToString();
		if (SIMPLE_OSS.GetObjectToMemory(BucketName, PatchVersionListName, JsonString))
		{
			VersionListCache.Empty();
			SimpleVersionControl::Read(JsonString,VersionListCache);
		}
	}

	//获取服务器关于DLC的版本列表
	{
		FString JsonString;
		FString DLCVersionListName = GetDefault<USimpleVCSettings>()->DLCVersionListName.ToString();
		if (SIMPLE_OSS.GetObjectToMemory(BucketName, DLCVersionListName, JsonString))
		{
			VersionListCache.Empty();
			SimpleVersionControl::Read(JsonString, DLCVersionListCache);
		}
	}
}

void FSimpleVersionControlEditorModule::ClearPakCache()
{
	IPlatformFile &PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString PakLocalPaths = GetDefault<USimpleUnrealPakSettings>()->PakSavePaths.Path;

	TArray<FString> PakPaths;
	IFileManager::Get().FindFilesRecursive(PakPaths, *PakLocalPaths, TEXT("*"), true, false);
	
	for (const auto &Tmp : PakPaths)
	{
		if (PlatformFile.DeleteFile(*Tmp))
		{

		}
	}
}


void FSimpleVersionControlEditorModule::RegisteringCallbacks(int32 ProgramIndex, bool bMainVersion /*= false*/)
{
	if (ProgramIndex == 0)
	{
		if (GetDefault<USimpleVCSettings>()->bAuto)
		{
			if (GetDefault<USimpleVCSettings>()->DataServerType == EUploadServerType::OSSServer)
			{
				FVersion Version;

				const FString BucketName = GetDefault<USimpleVCSettings>()->Bucket;
				
				if (!BucketName.Len())
				{
					return;
				}

				FString PakLocalPaths;
				FString SigPaths;
				FString VersionEXE;
				if (!bMainVersion)
				{
					PakLocalPaths = GetDefault<USimpleUnrealPakSettings>()->PakSavePaths.Path;
				}
				else
				{
					PakLocalPaths = GetDefault<UVersionControlInfo>()->MainVersion.FilePath;
					SigPaths = GetDefault<UVersionControlInfo>()->Sig.FilePath;
					VersionEXE = GetDefault<UVersionControlInfo>()->VersionEXE.FilePath;
				}

				if (!PakLocalPaths.Len())
				{
					return;
				}

				FString JsonData;
				const FString ServerVersionName = GetDefault<USimpleVCSettings>()->ServerVersionName.ToString();
				if (SIMPLE_OSS.GetObjectToMemory(BucketName, ServerVersionName, JsonData))
				{
					SimpleVersionControl::Read(JsonData, Version);
				}

				//判断版本是否一样 如果一样代表热更新 不一样代表有新版本上传
				if (Version.Name == GetDefault<USimpleUnrealPakSettings>()->Version)
				{
					Version.Crc = FGuid::NewGuid().ToString();
				}
				else
				{
					Version.Empty();
					Version.Name = GetDefault<USimpleUnrealPakSettings>()->Version;
					Version.Crc = FGuid::NewGuid().ToString();
				}

				FString URL = TEXT("https:/") / BucketName + TEXT(".") + SIMPLE_OSS.GetEndpoint();

				auto UpdateNewData = [&](const FString &NewPath)
				{
					if (!NewPath.IsEmpty())
					{
						bool bExists = false;

						FString PatchName = FPaths::GetCleanFilename(NewPath);

						FRemoteDataDescribe Describe;
						FRemoteDataDescribe *RemoteDataDescribe = &Describe;
						for (auto &TmpVersion : Version.Content)
						{
							FString LinkName = FPaths::GetCleanFilename(TmpVersion.Link);
							if (LinkName == PatchName)
							{
								RemoteDataDescribe = &TmpVersion;
								bExists = true;
								break;
							}
						}

						//len
						{
							TArray<uint8> DataInstance;
							FFileHelper::LoadFileToArray(DataInstance, *NewPath);
							RemoteDataDescribe->Len = DataInstance.Num();
						}

						if (bMainVersion)
						{
							RemoteDataDescribe->PakVersionType = EPakVersionType::MAIN_VERSION;
						}

						RemoteDataDescribe->Crc = FGuid::NewGuid().ToString();

						//Patch/4.27/hello.pak
						FString ObjectName = TEXT("Patch") / Version.Name / PatchName;
						if (SIMPLE_OSS.PutObject(BucketName, NewPath, ObjectName))
						{
							RemoteDataDescribe->Link = URL / ObjectName;
							if (!bExists)
							{
								Version.Content.Add(*RemoteDataDescribe);
							}
						}
					}
				};
				if (bMainVersion)
				{
					UpdateNewData(PakLocalPaths);
					UpdateNewData(VersionEXE);

					for (auto &Tmp : GetDefault<UVersionControlInfo>()->AdditionalDependent)
					{
						UpdateNewData(Tmp.FilePath);
					}

					for (const auto &Tmp : GetDefault<UVersionControlInfo>()->DeletedObjectsInServer)
					{
						FString TmpName = FPaths::GetCleanFilename(Tmp.FilePath);
						for (auto &TmpConent : Version.Content)
						{
							FString TmpConentName = FPaths::GetCleanFilename(TmpConent.Link);
							if (TmpName == TmpConentName)
							{
								TmpConent.bDiscard = true;
								break;
							}
						}
					}

					//上传我们的签名
					if (SigPaths.Len())
					{
						UpdateNewData(SigPaths);
					}
				}
				else
				{
					TArray<FString> PakPaths;
					IFileManager::Get().FindFilesRecursive(PakPaths, *PakLocalPaths, TEXT("*"), true, false);
					for (const auto &Tmp : PakPaths)
					{
						UpdateNewData(Tmp);
					}
				}

				//上传版本信息
				if (Version.Content.Num())
				{
					FString JsonString;
					SimpleVersionControl::Save(Version, JsonString);

					if (SIMPLE_OSS.PutObjectByMemory(BucketName, JsonString, ServerVersionName))
					{
						const FString PatchVersionPaths = GetDefault<USimpleVCSettings>()->PatchVersionPaths.ToString();
						if (SIMPLE_OSS.PutObjectByMemory(BucketName, JsonString, PatchVersionPaths / Version.Name + TEXT(".con")))
						{
							//更新版本列表
							VersionListCache.Name = Version.Name;
							VersionListCache.Crc = Version.Crc;
							VersionListCache.Content.AddUnique(Version.Name);

							//上传服务器列表
							JsonString.Empty();
							SimpleVersionControl::Save(VersionListCache, JsonString);

							const FString PatchVersionListName = GetDefault<USimpleVCSettings>()->PatchVersionListName.ToString();
							if (SIMPLE_OSS.PutObjectByMemory(BucketName, JsonString, PatchVersionListName))
							{

							}
						}
					}
				}
			}
		}
	}
}

void FSimpleVersionControlEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSimpleVCEditorStyle::Shutdown();
	FSimpleVCEditorCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleVersionControlEditorModule, SimpleVersionControlEditor)