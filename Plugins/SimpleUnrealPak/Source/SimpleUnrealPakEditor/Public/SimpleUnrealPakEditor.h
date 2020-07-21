// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "SimpleUnrealPakEditorType.h"
#include "TickableEditorObject.h"

class FToolBarBuilder;
class FMenuBuilder;

class FSimpleUnrealPakEditorModule : public IModuleInterface ,public FTickableEditorObject
{
	virtual void Tick(float DeltaTime);

	/** return the stat id to use for this tickable **/
	virtual TStatId GetStatId() const ;
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	SIMPLEUNREALPAKEDITOR_API void Pak(TArray<FString> NewPaths, ESimpleSuMenuType SuMenuType, FSimpleProcessDelegate Callback = nullptr);
	SIMPLEUNREALPAKEDITOR_API void Cook(TArray<FString> NewPaths, ESimpleSuMenuType SuMenuType, FSimpleProcessDelegate Callback = nullptr);
private:

	void SubExtendContentBrowser(FMenuBuilder& MunuBuilder, TArray<FString> NewPaths, ESimpleSuMenuType SuMenuType);

	TSharedRef<FExtender> OnExtendContentBrowser(const TArray<FString> &NewPaths);
	TSharedRef<FExtender> OnExtendContentAssetBrowser(const TArray<FAssetData> &AssetData);

	void CreateSuMenuForContentBrowser(FMenuBuilder& MunuBuilder,TArray<FString> NewPaths, ESimpleSuMenuType SuMenuType);

	bool IsTypeExistQueueTask(EQueueTaskType Type);
private:
	TSharedPtr<class FUICommandList> PluginCommands;

	TArray<FQueueTaskInterface *> QueueTask;
};
