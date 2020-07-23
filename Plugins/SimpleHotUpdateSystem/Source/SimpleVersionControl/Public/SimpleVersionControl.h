// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "SimpleVersionControlType.h"
#include "Version/VersionInfor.h"
#include "SimpleHTTPType.h"

class FSimpleVersionControlModule : public IModuleInterface
{
public:
	FSimpleVersionControlModule();

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	FStartSynchronizeServerVersionDelegate	StartSynchronizeServerVersionDelegate;
	FEndSynchronizeServerVersionDelegate	EndSynchronizeServerVersionDelegate;
	FSynchronizeServerVersionDelegate		SynchronizeServerVersionDelegate;
	FStartSynchronizeServerVersionDelegate	AllSynchronizeServerMissionAccomplishedDelegate;
	FFinishInitVersionDelegate				FinishInitVersionBPDelegate;

	FSimpleDelegate							FinishInitVersionDelegate;
	FStartUpdateServerDelegate				StartUpdateServerDelegate;
	FEndFUpdateServerDelegate				EndFUpdateServerDelegate;
	FUpdateServerDelegate					UpdateServerDelegate;
	FStartUpdateServerDelegate				AllMissionAccomplishedDelegate;

	bool IterativeUpdating();

	bool UpdateVersion(const FString &VersionName);
	bool UpdateNestVersion();
	void UpdatePatch();
	bool RestartGame(const FString &MainVersionName);
public:
	EServerVersionResponseType ValidationVersion();
	FString GetCurrentVersion();
	FString GetServerVersion();
	bool InitVersion(FSimpleDelegate InFinishInitVersionDelegate);//C++
	bool InitVersion(FFinishInitVersionDelegate InFinishInitVersionBPDelegate, FSimpleDelegate InFinishInitVersionDelegate = FSimpleDelegate());//C++ BP
	FORCEINLINE uint8 GetCacheCompleteCount() { return CacheCompleteCount; }
protected:
	void CompleteDownload(const FSimpleHttpRequest &Request, const FSimpleHttpResponse &Response, bool bConConnect);
	void Downloading(const FSimpleHttpRequest &Request, int32 BytesSent, int32 BytesReceived);
	void CompleteAllDownload();
protected:
	bool Init();
	void InitCurrentVersion(const FSimpleHttpRequest &Request, const FSimpleHttpResponse &Response, bool bConConnect);
	void InitServerVersion(const FSimpleHttpRequest &Request, const FSimpleHttpResponse &Response, bool bConConnect);
	void InitServerVersionList(const FSimpleHttpRequest &Request, const FSimpleHttpResponse &Response, bool bConConnect);

private:
	FRemoteDataDescribe *FindPakVersion(const FString &Name);

private:
	FVersion ClientVersion;
	FVersion LastClientVersion;
	FString URL;

	//Cache
	FVersion ServerVersionCache;
	FVersionList VersionListCache;

	uint8 CacheCompleteCount;
	bool bInitialization;
};
