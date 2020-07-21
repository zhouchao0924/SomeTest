// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FSimpleUnrealPakViewModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	void RegisterTabSpawners();
	TSharedRef<SWidget> CreateEditor();
private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

	void UpdateFile(const TArray<FString> &Files);

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
private:
	TSharedRef<SDockTab> SpawnByUnrealPakViewTab(const FSpawnTabArgs &Args);
	TSharedRef<SDockTab> SpawnByUnrealPakRegisterTab(const FSpawnTabArgs &Args);
	TSharedRef<SDockTab> SpawnByUnrealPakFileTreeTab(const FSpawnTabArgs &Args);
private:

	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<class SScrollBox> ScrollBox;
	class USimpleUEViewConfigurationInfor *Info;
};
