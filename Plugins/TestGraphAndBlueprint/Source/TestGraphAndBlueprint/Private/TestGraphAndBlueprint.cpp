// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TestGraphAndBlueprint.h"
#include "TestGraphAndBlueprintStyle.h"
#include "TestGraphAndBlueprintCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "EdGraph/EdGraph.h"
#include "GraphEditor.h"
#include "EdGraph/EdGraphSchema.h"
#include "TestGraphSchema.h"
#include "MyGraphPanelNodeFactory.h"

static const FName TestGraphAndBlueprintTabName("TestGraphAndBlueprint");

#define LOCTEXT_NAMESPACE "FTestGraphAndBlueprintModule"

void FTestGraphAndBlueprintModule::StartupModule()
{
	GraphObject = nullptr;
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FTestGraphAndBlueprintStyle::Initialize();
	FTestGraphAndBlueprintStyle::ReloadTextures();

	FTestGraphAndBlueprintCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FTestGraphAndBlueprintCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FTestGraphAndBlueprintModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FTestGraphAndBlueprintModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FTestGraphAndBlueprintModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(TestGraphAndBlueprintTabName, FOnSpawnTab::CreateRaw(this, &FTestGraphAndBlueprintModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FTestGraphAndBlueprintTabTitle", "TestGraphAndBlueprint"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	FEdGraphUtilities::RegisterVisualNodeFactory(MakeShareable(new FMyGraphPanelNodeFactory));
	FEdGraphUtilities::RegisterVisualPinFactory(MakeShareable(new FMyGraphPanelPinFactory));
	FEdGraphUtilities::RegisterVisualPinConnectionFactory(MakeShareable(new FMyGraphPanelPinConnectionFactory));
}

void FTestGraphAndBlueprintModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FTestGraphAndBlueprintStyle::Shutdown();

	FTestGraphAndBlueprintCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TestGraphAndBlueprintTabName);

	if (GraphObject != nullptr)
	{
		GraphObject = nullptr;
	}
}

TSharedRef<SDockTab> FTestGraphAndBlueprintModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FTestGraphAndBlueprintModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("TestGraphAndBlueprint.cpp"))
		);

	if (GraphObject != nullptr)
	{
		GraphObject = nullptr;
	}

	GraphObject = NewObject<UEdGraph>();
	GraphObject->Schema = UTestGraphSchema::StaticClass();
	GraphObject->AddToRoot();

	GraphEditorSlate = SNew(SGraphEditor)
	.GraphToEdit(GraphObject);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				/*SNew(STextBlock)
				.Text(WidgetText)*/
				GraphEditorSlate.ToSharedRef()
			]
		];
}

void FTestGraphAndBlueprintModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(TestGraphAndBlueprintTabName);
}

void FTestGraphAndBlueprintModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FTestGraphAndBlueprintCommands::Get().OpenPluginWindow);
}

void FTestGraphAndBlueprintModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FTestGraphAndBlueprintCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTestGraphAndBlueprintModule, TestGraphAndBlueprint)