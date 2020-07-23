// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "SimpleUnrealPakEditorType.h"
#include "Version/VersionInfor.h"

class FToolBarBuilder;
class FMenuBuilder;

class FSimpleVersionControlEditorModule : public IModuleInterface
{
	friend class SVersionControlWdiget;
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
protected:
	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	TSharedRef<SWidget> CreateEditor();

	void PluginButtonClicked();
	void AddToolbarExtension(FToolBarBuilder& Builder);

	TSharedRef<FExtender> OnExtendContentBrowser(const TArray<FString> &NewPaths);
	TSharedRef<FExtender> OnExtendContentAssetBrowser(const TArray<FAssetData> &AssetData);

	void CreateSuMenuForContentBrowser(FMenuBuilder& MunuBuilder, TArray<FString> NewPaths, ESimpleSuMenuType SuMenuType);
	
	void NewServerPatchVersion(TArray<FString> NewPaths, ESimpleSuMenuType SuMenuType);
	void NewServerDLCVersion(TArray<FString> NewPaths, ESimpleSuMenuType SuMenuType);

	void RegisteringPatchCallbacks(int32 ProgramIndex);
	void RegisteringDLCCallbacks(int32 ProgramIndex);
	void RegisteringMainVersionCallbacks(int32 ProgramIndex = 0);

	void InitVersionList();
	void ClearPakCache();
private:
	void RegisteringCallbacks(int32 ProgramIndex, bool bMainVersion = false);
protected:
	TSharedPtr<class FUICommandList> PluginCommands;

	FVersionList VersionListCache;
	FDLCList DLCVersionListCache;
};
