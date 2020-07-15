// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BlueprintToolEditor.h"
#include "AssetToolsModule.h"
#include "EdGraphUtilities.h"
#include "PropertyEditorModule.h"
#include "Details/BlueprintPropertyTypeCustomization.h"
#include "Factory/Blueprint/BlueprintToolFactory.h"
#include "AssetEditor/Architect/AssetArchitectActions.h"
#include "BlueprintEditor/GraphNode/CAssetThumbnailPool.h"

#define LOCTEXT_NAMESPACE "FBlueprintToolEditorModule"

void FBlueprintToolEditorModule::StartupModule()
{
	FObjectEditorThumbnailPool::Create();

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	RegisterAssetTypeAction(AssetTools, MakeShareable(new FBlueprintTypeActions));

	FEdGraphUtilities::RegisterVisualNodeFactory(MakeShareable(new FBToolPanelNodeFactory));
	FEdGraphUtilities::RegisterVisualPinFactory(MakeShareable(new FBToolPanelPinFactory));
	FEdGraphUtilities::RegisterVisualPinConnectionFactory(MakeShareable(new FBToolPanelPinConnectionFactory));

	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	RegisterPropertyTypeCustomizations();

	PropertyModule.NotifyCustomizationModuleChanged();
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

		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		for (auto It = RegisteredPropertyTypes.CreateConstIterator(); It; ++It)
		{
			if (It->IsValid())
			{
				PropertyModule.UnregisterCustomPropertyTypeLayout(*It);
			}
		}

		PropertyModule.NotifyCustomizationModuleChanged();
	}

	MyAssetTypeActions.Empty();
}

void FBlueprintToolEditorModule::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	MyAssetTypeActions.Add(Action);
}

void FBlueprintToolEditorModule::RegisterCustomPropertyTypeLayout(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate)
{
	check(PropertyTypeName != NAME_None);

	RegisteredPropertyTypes.Add(PropertyTypeName);

	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(PropertyTypeName, PropertyTypeLayoutDelegate);
}

void FBlueprintToolEditorModule::RegisterPropertyTypeCustomizations()
{
	RegisterCustomPropertyTypeLayout("DescriptionBPTool", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FBlueprintVariableMappings::MakeInstance));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBlueprintToolEditorModule, BlueprintToolEditor)