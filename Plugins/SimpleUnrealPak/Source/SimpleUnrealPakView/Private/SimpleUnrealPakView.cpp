// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SimpleUnrealPakView.h"
#include "SimpleUnrealPakViewStyle.h"
#include "SimpleUnrealPakViewCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Viewport/SimplePakPreviewViewport.h"
#include "SimpleUnrealPakViewType.h"
#include "Register/SimpleUEViewConfigurationInfor.h"
#include "Register/SimpleRegister.h"
#include "EditorStyleSet.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SScrollBox.h"
#include "FileTree/FileParsing.h"
#include "FileTree/SFolder.h"

static const FName SimpleUnrealPakViewTabName("SimpleUnrealPakView");

struct FSimpleUnrealPakViewID
{
	static const FName ViewportID;
	static const FName RegisterID;
	static const FName FileTreeID;
};

const FName FSimpleUnrealPakViewID::ViewportID(TEXT("UnrealPakViewportID"));
const FName FSimpleUnrealPakViewID::RegisterID(TEXT("UnrealPakRegisterID"));
const FName FSimpleUnrealPakViewID::FileTreeID(TEXT("UnrealPakFileTreeID"));

#define LOCTEXT_NAMESPACE "FSimpleUnrealPakViewModule"

void FSimpleUnrealPakViewModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSimpleUnrealPakViewStyle::Initialize();
	FSimpleUnrealPakViewStyle::ReloadTextures();

	FSimpleUnrealPakViewCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSimpleUnrealPakViewCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FSimpleUnrealPakViewModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FSimpleUnrealPakViewModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FSimpleUnrealPakViewModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SimpleUnrealPakViewTabName, FOnSpawnTab::CreateRaw(this, &FSimpleUnrealPakViewModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FSimpleUnrealPakViewTabTitle", "SimpleUnrealPakView"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	RegisterTabSpawners();

	if (!GMeshComponent)
	{
		GMeshComponent = NewObject<UStaticMeshComponent>();
		GMeshComponent->AddToRoot();
	}

	if (!Info)
	{
		Info = NewObject<USimpleUEViewConfigurationInfor>();
		Info->AddToRoot();
	}

	if (!GProceduralMeshComponent)
	{
		GProceduralMeshComponent = NewObject<UProceduralMeshComponent>();
		GProceduralMeshComponent->AddToRoot();
	}

	SimpleOneParamDelegate.BindRaw(this, &FSimpleUnrealPakViewModule::UpdateFile);
}

void FSimpleUnrealPakViewModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSimpleUnrealPakViewStyle::Shutdown();

	FSimpleUnrealPakViewCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SimpleUnrealPakViewTabName);

	//if (GProceduralMeshComponent)
	//{
	//	GProceduralMeshComponent->ConditionalBeginDestroy();
	//}
	//GProceduralMeshComponent = nullptr;

	//if (GMeshComponent)
	//{
	//	GMeshComponent->ConditionalBeginDestroy();
	//}
	//GMeshComponent = nullptr;

	//if (Info)
	//{
	//	Info->ConditionalBeginDestroy();
	//}
	//Info = nullptr;
}

TSharedRef<SDockTab> FSimpleUnrealPakViewModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
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

TSharedRef<SDockTab> FSimpleUnrealPakViewModule::SpawnByUnrealPakViewTab(const FSpawnTabArgs &Args)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SimplePakPreviewViewport)
			]
		];
}

TSharedRef<SDockTab> FSimpleUnrealPakViewModule::SpawnByUnrealPakRegisterTab(const FSpawnTabArgs &Args)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SimpleRegister,Info)
			]
		];
}

TSharedRef<SDockTab> FSimpleUnrealPakViewModule::SpawnByUnrealPakFileTreeTab(const FSpawnTabArgs &Args)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SOverlay)

				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SImage)
					.Image(FEditorStyle::Get().GetBrush("ExternalImagePicker.BlankImage"))
				]

				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SAssignNew(ScrollBox,SScrollBox)
					.Orientation(Orient_Vertical)
				]
			]
		];
}

void FSimpleUnrealPakViewModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(SimpleUnrealPakViewTabName);
}

void FSimpleUnrealPakViewModule::RegisterTabSpawners()
{
	FGlobalTabmanager::Get()->RegisterTabSpawner(
		FSimpleUnrealPakViewID::ViewportID,
		FOnSpawnTab::CreateRaw(this,&FSimpleUnrealPakViewModule::SpawnByUnrealPakViewTab))
		.SetDisplayName(LOCTEXT("SimplePakViewViewportID", "Rendering"));

	FGlobalTabmanager::Get()->RegisterTabSpawner(
		FSimpleUnrealPakViewID::RegisterID,
		FOnSpawnTab::CreateRaw(this, &FSimpleUnrealPakViewModule::SpawnByUnrealPakRegisterTab))
		.SetDisplayName(LOCTEXT("SimplePakViewRegisterID", "Register"));

	FGlobalTabmanager::Get()->RegisterTabSpawner(
		FSimpleUnrealPakViewID::FileTreeID,
		FOnSpawnTab::CreateRaw(this, &FSimpleUnrealPakViewModule::SpawnByUnrealPakFileTreeTab))
		.SetDisplayName(LOCTEXT("SimplePakViewFileTreeID", "File Tree"));
}

TSharedRef<SWidget> FSimpleUnrealPakViewModule::CreateEditor()
{
	TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("SimpleUnrealPakView_Layout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Vertical)
				->SetSizeCoefficient(0.3f)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.4f)
					->SetHideTabWell(true)
					->AddTab(FSimpleUnrealPakViewID::RegisterID, ETabState::OpenedTab)		
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.6f)
					->SetHideTabWell(true)
					->AddTab(FSimpleUnrealPakViewID::FileTreeID, ETabState::OpenedTab)
				)
			)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.7f)
				->SetHideTabWell(true)
				->AddTab(FSimpleUnrealPakViewID::ViewportID,ETabState::OpenedTab)
			)	
		);

	TSharedPtr<SWindow> SimpleWidows = SNew(SWindow);

	return FGlobalTabmanager::Get()->RestoreFrom(Layout, SimpleWidows).ToSharedRef();
}

void FSimpleUnrealPakViewModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FSimpleUnrealPakViewCommands::Get().OpenPluginWindow);
}

void FSimpleUnrealPakViewModule::UpdateFile(const TArray<FString> &Files)
{
	if (ScrollBox.IsValid())
	{
		SimpleUnrealPakView::FFileList FileList;
		SimpleUnrealPakView::FilesParsing(Files, FileList);

		ScrollBox->ClearChildren();
		ScrollBox->AddSlot()
		[
			SNew(SFolder,FileList)
		];
	}
}

void FSimpleUnrealPakViewModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FSimpleUnrealPakViewCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleUnrealPakViewModule, SimpleUnrealPakView)