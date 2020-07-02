// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ExtendEditor.h"
#include "ExtendEditorStyle.h"
#include "ExtendEditorCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "LevelEditor.h"

static const FName ExtendEditorTabName("ExtendEditor");

#define LOCTEXT_NAMESPACE "FExtendEditorModule"

void FExtendEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FExtendEditorStyle::Initialize();
	FExtendEditorStyle::ReloadTextures();

	FExtendEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FExtendEditorCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FExtendEditorModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FExtendEditorModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}

	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FExtendEditorModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	{
		TSharedPtr<FExtender> MenuBarExtender = MakeShareable(new FExtender());
		MenuBarExtender->AddMenuBarExtension("Help", EExtensionHook::After, PluginCommands, FMenuBarExtensionDelegate::CreateRaw(this, &FExtendEditorModule::AddMenuBarExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuBarExtender);
	}
}

void FExtendEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FExtendEditorStyle::Shutdown();

	FExtendEditorCommands::Unregister();
}

void FExtendEditorModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FExtendEditorModule::PluginButtonClicked()")),
							FText::FromString(TEXT("ExtendEditor.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FExtendEditorModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FExtendEditorCommands::Get().PluginAction);
}

void FExtendEditorModule::AddMenuBarExtension(FMenuBarBuilder &Builder)
{
	Builder.AddMenuEntry(FExtendEditorCommands::Get().PluginAction);
}

void FExtendEditorModule::PullDwonBar(FMenuBuilder& Builder)
{

}

void FExtendEditorModule::PullDwonSuBar(FMenuBuilder& Builder)
{
	
}

void FExtendEditorModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FExtendEditorCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FExtendEditorModule, ExtendEditor)