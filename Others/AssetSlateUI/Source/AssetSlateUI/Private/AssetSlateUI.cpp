// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AssetSlateUI.h"
#include "AssetToolsModule.h"
#include "CustomAssetActions.h"
#include "PropertyEditorModule.h"
#include "MeshImportPanel.h"

#define LOCTEXT_NAMESPACE "FAssetSlateUIModule"

void FAssetSlateUIModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	RegisteredAssetTypeActions.Add(MakeShareable(new FCustomAssetActions()));
	AssetTools.RegisterAssetTypeActions(RegisteredAssetTypeActions[RegisteredAssetTypeActions.Num()-1]);

	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	RegisterPropertyTypeCustomizations();

	PropertyModule.NotifyCustomizationModuleChanged();
}

void FAssetSlateUIModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");
	if (AssetToolsModule != nullptr)
	{
		IAssetTools& AssetTools = AssetToolsModule->Get();

		for (auto Action : RegisteredAssetTypeActions)
		{
			AssetTools.UnregisterAssetTypeActions(Action);
		}
	}

	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
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
}

void FAssetSlateUIModule::RegisterPropertyTypeCustomizations()
{
	RegisterCustomPropertyTypeLayout("MeshlImport", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FMeshlImportDetail::MakeInstance));
}

void FAssetSlateUIModule::RegisterCustomPropertyTypeLayout(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate)
{
	check(PropertyTypeName != NAME_None);

	RegisteredPropertyTypes.Add(PropertyTypeName);

	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(PropertyTypeName, PropertyTypeLayoutDelegate);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAssetSlateUIModule, AssetSlateUI)