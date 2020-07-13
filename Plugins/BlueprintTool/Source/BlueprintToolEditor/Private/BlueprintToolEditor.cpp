// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BlueprintToolEditor.h"
#include "AssetToolsModule.h"
#include "AssetEditor/Architect/AssetArchitectActions.h"

#define LOCTEXT_NAMESPACE "FBlueprintToolEditorModule"

void FBlueprintToolEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	RegisterAssetTypeAction(AssetTools, MakeShareable(new FBlueprintTypeActions));
}

void FBlueprintToolEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (int32 Index = 0; Index < MyAssetTypeActions.Num(); ++Index)
		{
			AssetTools.UnregisterAssetTypeActions(MyAssetTypeActions[Index].ToSharedRef());
		}
	}

	MyAssetTypeActions.Empty();
}

void FBlueprintToolEditorModule::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	MyAssetTypeActions.Add(Action);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBlueprintToolEditorModule, BlueprintToolEditor)