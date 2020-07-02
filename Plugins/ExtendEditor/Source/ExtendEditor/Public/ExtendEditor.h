// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FExtendEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	
private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);
	void AddMenuBarExtension(FMenuBarBuilder &Builder);

	void PullDwonBar(FMenuBuilder& Builder);
	void PullDwonSuBar(FMenuBuilder& Builder);

	void EdtorPrint(FString MyString);

	TSharedRef<FExtender> SelectedCurrentActors(const TSharedRef<FUICommandList> MyUICommandList, const TArray<AActor*> AllActor);

	void AddSelectActorButton(FMenuBuilder& Builder);

	TSharedRef<FExtender> GetPathsFormEditor(const TArray<FString>& NewPaths);

private:
	TSharedPtr<class FUICommandList> PluginCommands;

	FDelegateHandle LevelViewportMenuExtender_SelectedActor;
};

class FTaskABC
{
public:

	static TSharedRef<SWidget> MakeWidget(TSharedPtr<class FUICommandList> PluginCommands);
};
