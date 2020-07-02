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

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};

class FTaskABC
{
public:

	static TSharedRef<SWidget> MakeWidget(TSharedPtr<class FUICommandList> PluginCommands);
};
