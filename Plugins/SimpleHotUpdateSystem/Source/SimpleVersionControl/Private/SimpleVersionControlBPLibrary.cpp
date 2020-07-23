// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SimpleVersionControlBPLibrary.h"
#include "SimpleVersionControl.h"

USimpleVersionControlBPLibrary::USimpleVersionControlBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}


bool USimpleVersionControlBPLibrary::InitVersion(FFinishInitVersionDelegate InFinishInitVersionBPDelegate)
{
	FSimpleVersionControlModule * VersionControlModule = FModuleManager::GetModulePtr<FSimpleVersionControlModule>(TEXT("SimpleVersionControl"));

	if (VersionControlModule)
	{
		return VersionControlModule->InitVersion(InFinishInitVersionBPDelegate);
	}

	return false;
}

void USimpleVersionControlBPLibrary::VersionUpdateDelegate(
	FStartSynchronizeServerVersionDelegate Start,
	FSynchronizeServerVersionDelegate Downloading, 
	FEndSynchronizeServerVersionDelegate End,
	FStartSynchronizeServerVersionDelegate AllComplete)
{
	FSimpleVersionControlModule * VersionControlModule = FModuleManager::GetModulePtr<FSimpleVersionControlModule>(TEXT("SimpleVersionControl"));

	if (VersionControlModule)
	{
		VersionControlModule->StartSynchronizeServerVersionDelegate = Start;;
		VersionControlModule->EndSynchronizeServerVersionDelegate = End;
		VersionControlModule->SynchronizeServerVersionDelegate = Downloading;
		VersionControlModule->AllSynchronizeServerMissionAccomplishedDelegate = AllComplete;
	}
}

bool USimpleVersionControlBPLibrary::IsVersionCachePrepared()
{
	FSimpleVersionControlModule * VersionControlModule = FModuleManager::GetModulePtr<FSimpleVersionControlModule>(TEXT("SimpleVersionControl"));

	if (VersionControlModule)
	{
		return VersionControlModule->GetCacheCompleteCount() >= 2;
	}

	return false;
}

EServerVersionResponseType USimpleVersionControlBPLibrary::ValidationVersion()
{
	FSimpleVersionControlModule * VersionControlModule = FModuleManager::GetModulePtr<FSimpleVersionControlModule>(TEXT("SimpleVersionControl"));

	if (VersionControlModule)
	{
		return VersionControlModule->ValidationVersion();
	}

	return EServerVersionResponseType::MAX;
}

bool USimpleVersionControlBPLibrary::IterativeUpdating()
{
	FSimpleVersionControlModule * VersionControlModule = FModuleManager::GetModulePtr<FSimpleVersionControlModule>(TEXT("SimpleVersionControl"));

	if (VersionControlModule)
	{
		return VersionControlModule->IterativeUpdating();
	}

	return false;
}

FString USimpleVersionControlBPLibrary::GetCurrentVersion()
{
	FSimpleVersionControlModule * VersionControlModule = FModuleManager::GetModulePtr<FSimpleVersionControlModule>(TEXT("SimpleVersionControl"));

	if (VersionControlModule)
	{
		return VersionControlModule->GetCurrentVersion();
	}

	return "";
}

FString USimpleVersionControlBPLibrary::GetServerVersion()
{
	FSimpleVersionControlModule * VersionControlModule = FModuleManager::GetModulePtr<FSimpleVersionControlModule>(TEXT("SimpleVersionControl"));

	if (VersionControlModule)
	{
		return VersionControlModule->GetServerVersion();
	}

	return "";
}

bool USimpleVersionControlBPLibrary::RestartGame(const FString &MainVersionName)
{
	FSimpleVersionControlModule * VersionControlModule = FModuleManager::GetModulePtr<FSimpleVersionControlModule>(TEXT("SimpleVersionControl"));

	if (VersionControlModule)
	{
		return VersionControlModule->RestartGame(MainVersionName);
	}

	return false;
}

