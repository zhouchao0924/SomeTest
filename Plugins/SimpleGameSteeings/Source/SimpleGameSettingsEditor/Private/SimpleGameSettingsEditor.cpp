// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SimpleGameSettingsEditor.h"
#include "ISettingsModule.h"
#include "Core/SimpleGameExtraSettings.h"
#include "Core/SimpleGameExtraSettings.h"
#include "Core/SimpleGameUserSettings.h"
#include "Misc/FileHelper.h"

#define LOCTEXT_NAMESPACE "SimpleGameSettingsEditor"

void FSimpleGameSettingsEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings(
			"Project", 
			"Plugins", 
			"SimpleGameExtraSettings", 
			LOCTEXT("SimpleGameExtraSettings", "Simple Game Extra Settings Page"), 
			LOCTEXT("SimpleGameExtraSettingsDescription", "Simple Game Extra Settings Page Description"),
			GetMutableDefault<USimpleGameExtraSettings>());
	}

	//自动替换我们自定义的GameUserSettings
	ReplaceGasmeUserSettingsCofig();
}

void FSimpleGameSettingsEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FSimpleGameSettingsEditorModule::ReplaceGasmeUserSettingsCofig()
{
	TArray<FString> Result;
	FString CongifPath = FPaths::ProjectConfigDir() / TEXT("DefaultEngine.ini");
	FFileHelper::LoadFileToStringArray(Result,*CongifPath);

	auto IsHasScriptEngineLabel = [&](const FString &InString)->int32
	{
		for (int32 i = 0;i < Result.Num();++i)
		{
			if (Result[i].Contains(InString))
			{
				return i;
			}
		}

		return INDEX_NONE;
	};
	
	auto AddClass = [&](FString &GameUserSettingsClassName)
	{
		FString GameUserSettingsClassNameContent;
		if (GetDefault<USimpleGameExtraSettings>()->GameUserSettingsClass.Get())
		{
			FString ExtraSettingsName = GetDefault<USimpleGameExtraSettings>()->GameUserSettingsClass.Get()->GetName();
			if (ExtraSettingsName == TEXT("SimpleGameUserSettings"))
			{
				GameUserSettingsClassNameContent = TEXT("SimpleGameSettings.SimpleGameUserSettings");
			}
			else
			{
				GameUserSettingsClassNameContent = FString(FApp::GetProjectName()) + TEXT(".") + ExtraSettingsName;
			}
		}
		else
		{
			GameUserSettingsClassNameContent = TEXT("SimpleGameSettings.SimpleGameUserSettings");
		}

		GameUserSettingsClassName = TEXT("GameUserSettingsClassName=/Script") / GameUserSettingsClassNameContent;
	};
	bool bSave = false;

	int32 ScriptInd = IsHasScriptEngineLabel(TEXT("[/Script/Engine.Engine]"));
	if (ScriptInd != INDEX_NONE)
	{
		int32 GameUserSettingsClassIndex = IsHasScriptEngineLabel(TEXT("GameUserSettingsClassName="));
		if (GameUserSettingsClassIndex != INDEX_NONE)
		{
			int32 I = IsHasScriptEngineLabel(TEXT("GameUserSettingsClassName=/Script/Engine.GameUserSettings"));
			if ( I != INDEX_NONE)
			{
				AddClass(Result[I]);
			}
			else
			{
				AddClass(Result[GameUserSettingsClassIndex]);
			}

			bSave = true;
		}
		else
		{
			Result.Insert(FString(), ScriptInd + 1);
			AddClass(Result[ScriptInd + 1]);
			bSave = true;
		}
	}
	else
	{
		int32 Pos = Result.Add(TEXT("[/Script/Engine.Engine]"));
		Result.Insert(FString(), Pos + 1);
		AddClass(Result[Pos + 1]);
		bSave = true;
	}

	if (bSave)
	{
		FFileHelper::SaveStringArrayToFile(Result, *CongifPath);
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleGameSettingsEditorModule, SimpleGameSettingsEditor)