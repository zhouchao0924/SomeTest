// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TestDetails.h"
#include "PropertyEditorModule.h"
#include "MyTestDetailCustomization.h"
#include "StructVariableDetail.h"

#define LOCTEXT_NAMESPACE "FTestDetailsModule"

void FTestDetailsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	RegisterPropertyTypeCustomizations();
	RegisterObjectCustomizations();

	/**
	 * Customization modules should call this when that module has been unloaded, loaded, etc...
	 * so the property module can clean up its data.  Needed to support dynamic reloading of modules
	 */
	PropertyModule.NotifyCustomizationModuleChanged();
}

void FTestDetailsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

		for (auto It = RegisteredClassNames.CreateConstIterator(); It; ++It)
		{
			if (It->IsValid())
			{
				PropertyModule.UnregisterCustomClassLayout(*It);
			}
		}

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

void FTestDetailsModule::RegisterPropertyTypeCustomizations()
{
	RegisterCustomPropertyTypeLayout("StructVariable", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FStructVariableDetail::MakeInstance));
}

void FTestDetailsModule::RegisterObjectCustomizations()
{
	RegisterCustomClassLayout("CustomizeObject", FOnGetDetailCustomizationInstance::CreateStatic(&FMyTestDetailCustomization::MakeInstance));
}

void FTestDetailsModule::RegisterCustomClassLayout(FName ClassName, FOnGetDetailCustomizationInstance DetailLayoutDelegate)
{
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(ClassName, DetailLayoutDelegate);
}

void FTestDetailsModule::RegisterCustomPropertyTypeLayout(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate)
{
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(PropertyTypeName, PropertyTypeLayoutDelegate);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTestDetailsModule, TestDetails)