// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BlueprintToolEditor.h"
#include "AssetToolsModule.h"
#include "AssetEditor/Architect/AssetArchitectActions.h"
#include "Factory/Blueprint/BlueprintToolFactory.h"
#include "EdGraphUtilities.h"

#define LOCTEXT_NAMESPACE "FBlueprintToolEditorModule"

void FBlueprintToolEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	RegisterAssetTypeAction(AssetTools, MakeShareable(new FBlueprintTypeActions));

	FEdGraphUtilities::RegisterVisualNodeFactory(MakeShareable(new FBToolPanelNodeFactory));
	FEdGraphUtilities::RegisterVisualPinFactory(MakeShareable(new FBToolPanelPinFactory));
	FEdGraphUtilities::RegisterVisualPinConnectionFactory(MakeShareable(new FBToolPanelPinConnectionFactory));
}

void FBlueprintToolEditorModule::ShutdownModule()
{
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