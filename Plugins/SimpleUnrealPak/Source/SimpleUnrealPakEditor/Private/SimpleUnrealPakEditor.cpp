// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SimpleUnrealPakEditor.h"
#include "SimpleUnrealPakEditorStyle.h"
#include "SimpleUnrealPakEidtorCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "LevelEditor.h"
#include "ContentBrowserModule.h"
#include "Pak/UnrealPakFile.h"
#include "Settings/SimpleUnrealPakSettings.h"
#include "ISettingsModule.h"

static const FName SimpleUnrealPakTabName("SimpleUnrealPak");

#define LOCTEXT_NAMESPACE "FSimpleUnrealPakModule"

FQueueTaskInterface *RegisteringQueueTask(EQueueTaskType Type)
{
	FQueueTaskInterface *Task = nullptr;

	switch (Type)
	{
	case EQueueTaskType::PAK:
		Task = new FPakQueueTask();
		break;
	case EQueueTaskType::COOK:
		Task = new FCookQueueTask();
		break;
	}

	return Task;
}

void FSimpleUnrealPakEditorModule::Tick(float DeltaTime)
{
	if (QueueTask.Num())
	{
		//清除已经完成的任务
		TArray<FQueueTaskInterface *>RemoveTask;
		for (auto &Tmp : QueueTask)
		{
			if (Tmp->bMissionAccomplished)
			{
				RemoveTask.Add(Tmp);
			}
		}

		for (auto &Tmp : RemoveTask)
		{
			QueueTask.Remove(Tmp);
			delete Tmp;

			Tmp = nullptr;
		}

		for (auto &Tmp : QueueTask)
		{
			Tmp->Tick();
		}
	}
}

void FPakQueueTask::Tick()
{
	if (!bMissionAccomplished)
	{
		bool bPaking = false;
		for (auto &Tmp : PakHandles)
		{
			if (FPlatformProcess::IsProcRunning(Tmp))
			{
				bPaking = true;
				break;
			}
		}	

		if (!bPaking)
		{
			bMissionAccomplished = true;
			MissionAccomplished.ExecuteIfBound(0);
		}
	}
}

void FCookQueueTask::Tick()
{
	if (!bMissionAccomplished)
	{
		if (!FPlatformProcess::IsProcRunning(CookHandles))
		{
			bMissionAccomplished = true;
			int32 ReturnValue;
			FPlatformProcess::GetProcReturnCode(CookHandles, &ReturnValue);
			MissionAccomplished.ExecuteIfBound(ReturnValue);
		}
	}
}

TStatId FSimpleUnrealPakEditorModule::GetStatId() const
{
	return TStatId();
}

void FSimpleUnrealPakEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSimpleUnrealPakEditorStyle::Initialize();
	FSimpleUnrealPakEditorStyle::ReloadTextures();
	FSimpleUnrealPakEditorCommands::Register();

	if (ISettingsModule * SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>(TEXT("Settings")))
	{
		SettingsModule->RegisterSettings("Project", "ProjectPak", "SimpleUnrealPakSettings",
			LOCTEXT("SimpleUnrealPakSettings", "Simple Unreal Pak Settings"),
			LOCTEXT("SimpleUnrealPakSettingsTip", "Package settings for the current platform you have set."),
			GetMutableDefault<USimpleUnrealPakSettings>());
	}
	{
		FContentBrowserModule &ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
		TArray<FContentBrowserMenuExtender_SelectedPaths> &ContentBrowserMenuExtender_SelectedPaths = ContentBrowserModule.GetAllPathViewContextMenuExtenders();
		ContentBrowserMenuExtender_SelectedPaths.Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this, &FSimpleUnrealPakEditorModule::OnExtendContentBrowser));

		TArray<FContentBrowserMenuExtender_SelectedAssets> &ContentBrowserMenuExtender_SelectedAssets = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
		ContentBrowserMenuExtender_SelectedAssets.Add(FContentBrowserMenuExtender_SelectedAssets::CreateRaw(this, &FSimpleUnrealPakEditorModule::OnExtendContentAssetBrowser));
	}
}

void FSimpleUnrealPakEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSimpleUnrealPakEditorStyle::Shutdown();

	FSimpleUnrealPakEditorCommands::Unregister();
}

void FSimpleUnrealPakEditorModule::SubExtendContentBrowser(FMenuBuilder& MunuBuilder, TArray<FString> NewPaths, ESimpleSuMenuType SuMenuType)
{
	MunuBuilder.BeginSection("SimpleUnrealPakMenu", LOCTEXT("SimpleUnrealPakMenu", "Simple Unreal Pak Menu"));
	{
		MunuBuilder.AddMenuEntry(
			LOCTEXT("Pak", "Pak Resources"),
			LOCTEXT("PakTips", "You can package all the resources in the file directory as Pak ."),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateRaw(this, &FSimpleUnrealPakEditorModule::Pak, NewPaths, SuMenuType, FSimpleProcessDelegate())));

		bool bGame = NewPaths[0] == TEXT("/Game");
		bool bAsset = SuMenuType == ESimpleSuMenuType::ASSET;
		if (bGame || bAsset || 1)
		{
			FString CookString;
			if (bGame)
			{
				CookString = TEXT("Cook All Resources");
			}
			else if(bAsset)
			{
				CookString = TEXT("Cook Map Asset");
			}
			else
			{
				CookString = TEXT("Cook File Resources");
			}

			MunuBuilder.AddMenuEntry(
				FText::Format(LOCTEXT("Cook", "{0}"), FText::FromString(CookString)),
				LOCTEXT("PakTips", "Cook our resources ."),
				FSlateIcon(),
				FUIAction(FExecuteAction::CreateRaw(this, &FSimpleUnrealPakEditorModule::Cook, NewPaths, SuMenuType, FSimpleProcessDelegate())));
		}
	}
	MunuBuilder.EndSection();
}

void FSimpleUnrealPakEditorModule::Pak(TArray<FString> NewPaths, ESimpleSuMenuType SuMenuType, FSimpleProcessDelegate Callback)
{
	if (!IsTypeExistQueueTask(EQueueTaskType::COOK))
	{
		FQueueTaskInterface *Task = RegisteringQueueTask(EQueueTaskType::PAK);
		TArray<FProcHandle> Array;
		SimpleUnrealPakEditor::FPak Pak;
		switch (SuMenuType)
		{
		case ESimpleSuMenuType::ASSET:
			Array = Pak.PakMapFile(NewPaths);
			break;
		case ESimpleSuMenuType::CONTENT:
			Array = Pak.PakFile(NewPaths);
			break;
		}

		for (auto &Tmp : Array)
		{
			Task->Add(Tmp);
		}
		Task->MissionAccomplished = Callback;

		QueueTask.Add(Task);
	}
}

void FSimpleUnrealPakEditorModule::Cook(TArray<FString> NewPaths, ESimpleSuMenuType SuMenuType, FSimpleProcessDelegate Callback)
{
	if (!IsTypeExistQueueTask(EQueueTaskType::PAK))
	{
		FQueueTaskInterface *Task = RegisteringQueueTask(EQueueTaskType::COOK);
		SimpleUnrealPakEditor::FPak Pak;

		Task->Add(Pak.Cook(NewPaths, (bool)SuMenuType));
		Task->MissionAccomplished = Callback;	

		QueueTask.Add(Task);
	}
}

TSharedRef<FExtender> FSimpleUnrealPakEditorModule::OnExtendContentBrowser(const TArray<FString> &NewPaths)
{
	TSharedRef<FExtender> Extender(new FExtender);

	Extender->AddMenuExtension("FolderContext",EExtensionHook::Before,nullptr, FMenuExtensionDelegate::CreateRaw(this,&FSimpleUnrealPakEditorModule::CreateSuMenuForContentBrowser, NewPaths, ESimpleSuMenuType::CONTENT));

	return Extender;
}

TSharedRef<FExtender> FSimpleUnrealPakEditorModule::OnExtendContentAssetBrowser(const TArray<FAssetData> &AssetData)
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

	Extender->AddMenuExtension("AssetContextReferences", EExtensionHook::Before, nullptr, FMenuExtensionDelegate::CreateRaw(this, &FSimpleUnrealPakEditorModule::CreateSuMenuForContentBrowser, NewPaths,ESimpleSuMenuType::ASSET));

	return Extender;
}

void FSimpleUnrealPakEditorModule::CreateSuMenuForContentBrowser(FMenuBuilder& MunuBuilder, TArray<FString> NewPaths, ESimpleSuMenuType SuMenuType)
{
	MunuBuilder.AddSubMenu(
		LOCTEXT("SimpleUnrealPak", "Simple Unreal Pak"),
		LOCTEXT("SimpleUnrealPakDescribe", "Package or perform other operations on the files under the current file ."),
		FNewMenuDelegate::CreateRaw(this, &FSimpleUnrealPakEditorModule::SubExtendContentBrowser, NewPaths, SuMenuType),false,FSlateIcon());
}

bool FSimpleUnrealPakEditorModule::IsTypeExistQueueTask(EQueueTaskType Type)
{
	for (const auto &Tmp : QueueTask)
	{
		if (Tmp->GetQueueTaskType() == Type)
		{
			return true;
		}
	}

	return false;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleUnrealPakEditorModule, SimpleUnrealPakEditor)