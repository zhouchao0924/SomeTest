// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SimpleVersionControl.h"
#include "SimpleHttpManage.h"
#include "Settings/SimpleVCSettings.h"

#define LOCTEXT_NAMESPACE "FSimpleVersionControlModule"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif


FSimpleVersionControlModule::FSimpleVersionControlModule()
	:CacheCompleteCount(0)
	,bInitialization(false)
{

}

void FSimpleVersionControlModule::StartupModule()
{
	FString ClientVersionInfoData;
	FFileHelper::LoadFileToString(ClientVersionInfoData, *(FPaths::ProjectDir() / TEXT(".simple/ClientVersion.con")));
	FFileHelper::LoadFileToString(URL, *(FPaths::ProjectDir() / TEXT(".simple/URL.txt")));

	if (ClientVersionInfoData.Len())
	{
		SimpleVersionControl::Read(ClientVersionInfoData, ClientVersion);
	}
	else
	{
		ClientVersion.Name = TEXT("Release01");
		ClientVersion.Crc = FGuid::NewGuid().ToString();
	}
}

void FSimpleVersionControlModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FString ClientVersionInfoData;
	SimpleVersionControl::Save(ClientVersion,ClientVersionInfoData);

	if (ClientVersionInfoData.Len())
	{
		FFileHelper::SaveStringToFile(ClientVersionInfoData, *(FPaths::ProjectDir() / TEXT(".simple/ClientVersion.con")));
	}

	if (URL.Len())
	{
		FFileHelper::SaveStringToFile(URL, *(FPaths::ProjectDir() / TEXT(".simple/URL.txt")));
	}
}

EServerVersionResponseType FSimpleVersionControlModule::ValidationVersion()
{
	if (ServerVersionCache.Name == ClientVersion.Name)
	{
		if (ServerVersionCache.Crc == ClientVersion.Crc)
		{
			return EServerVersionResponseType::EQUAL;
		}

		return EServerVersionResponseType::CRC_DIFF;
	}

	return EServerVersionResponseType::VERSION_DIFF;
}

bool FSimpleVersionControlModule::IterativeUpdating()
{
	auto Type = ValidationVersion();
	if (Type == EServerVersionResponseType::CRC_DIFF)
	{	
		return UpdateVersion(ClientVersion.Name);;
	}
	else if (Type == EServerVersionResponseType::VERSION_DIFF)
	{	
		return UpdateNestVersion();
	}

	return true;
}

bool FSimpleVersionControlModule::UpdateVersion(const FString &VersionName)
{
	FString PatchVersionPaths = GetDefault<USimpleVCSettings>()->PatchVersionPaths.ToString();
	
	FSimpleHTTPResponseDelegate ResponseDelegate;
	ResponseDelegate.SimpleCompleteDelegate = FSimpleSingleCompleteDelegate::CreateRaw(this, &FSimpleVersionControlModule::InitCurrentVersion);

	return SIMPLE_HTTP.GetObjectToMemory(ResponseDelegate, URL / PatchVersionPaths/ VersionName + TEXT(".con"));
}

bool FSimpleVersionControlModule::UpdateNestVersion()
{
	int32 Index = VersionListCache.Content.Find(ClientVersion.Name);
	if (Index != INDEX_NONE)
	{
		FString NestVersionName = VersionListCache.Content[++Index];

		return UpdateVersion(NestVersionName);
	}

	return false;
}

void FSimpleVersionControlModule::UpdatePatch()
{
	FString SavePaths = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / TEXT("Patch"));

	StartSynchronizeServerVersionDelegate.ExecuteIfBound(ClientVersion.Name);
	StartUpdateServerDelegate.ExecuteIfBound(ClientVersion.Name);

	FSimpleHTTPResponseDelegate ResponseDelegate;
	ResponseDelegate.SimpleCompleteDelegate = FSimpleSingleCompleteDelegate::CreateRaw(this, &FSimpleVersionControlModule::CompleteDownload);
	ResponseDelegate.SimpleSingleRequestProgressDelegate = FSimpleSingleRequestProgressDelegate::CreateRaw(this, &FSimpleVersionControlModule::Downloading);
	ResponseDelegate.AllTasksCompletedDelegate = FSimpleDelegate::CreateRaw(this, &FSimpleVersionControlModule::CompleteAllDownload);
	
	TArray<FString> LinkArray;
	for (const auto &Tmp : ClientVersion.Content)
	{
		bool bHotUpdate = true;
		for (const auto &LastTmp : LastClientVersion.Content)
		{
			if (LastTmp.Crc == Tmp.Crc)
			{
				bHotUpdate = false;
				break;
			}
		}

		if (bHotUpdate)
		{
			LinkArray.AddUnique(Tmp.Link);
		}	
	}

	TArray<FString> RemovePak;
	for (const auto &Tmp : ClientVersion.Content)
	{
		FString LinkName = FPaths::GetCleanFilename(Tmp.Link);
		if (Tmp.bDiscard)
		{
			RemovePak.Add(LinkName);
		}
	}

	if (RemovePak.Num())
	{
		TArray<FString> PakNames;
		FString PatchPaths = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / TEXT("Patch/"));
		IFileManager::Get().FindFiles(PakNames, *(PatchPaths + TEXT("*")), true, false);

		IPlatformFile &PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		for (auto &Tmp : PakNames)
		{
			if (RemovePak.Find(Tmp) != INDEX_NONE)
			{
				if (PlatformFile.DeleteFile(*(PatchPaths + Tmp)))
				{

				}
			}
		}
	}

	SIMPLE_HTTP.GetObjectsToLocal(ResponseDelegate, LinkArray, SavePaths);
}

bool FSimpleVersionControlModule::RestartGame(const FString &MainVersionName)
{
	FString Platform = TEXT("Win64");

	FString Pram;
	FString BlankSpace = TEXT(" ");
	FString From = TEXT("From=");
	FString To = TEXT("To=");

	TArray<FString> PakNames;
	FString ExePath = FPaths::ProjectDir() / TEXT("Binaries") / Platform / ("ResetGame.exe");
	FString BinariesPaths = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / TEXT("Binaries") / Platform);
	FString PatchPaths = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / TEXT("Patch"));
	FString ContentPakPaths = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir() / TEXT("Paks"));

	IFileManager::Get().FindFiles(PakNames, *(PatchPaths / TEXT("*")), true, false);
	for (auto &Tmp : PakNames)
	{
		FString Extension = FPaths::GetExtension(Tmp,true);
		if (Extension == TEXT(".pak") || Extension == TEXT(".sig"))
		{
			From += (PatchPaths / Tmp + TEXT(";"));
			To += (ContentPakPaths / Tmp + TEXT(";"));
		}
		else if(Extension == TEXT(".dll") || Extension == TEXT(".lib") || Extension == TEXT(".exe"))
		{
			From += (PatchPaths / Tmp + TEXT(";"));
			To += (BinariesPaths / Tmp + TEXT(";"));
		}
	}
	From.RemoveFromEnd(";");
	To.RemoveFromEnd(";");

	FString ProcessId = TEXT("ProcessID=");
	{
		ProcessId += FString::FromInt(FPlatformProcess::GetCurrentProcessId());
	}

	Pram += (From + BlankSpace + To + BlankSpace + ProcessId);

	FProcHandle Handle = FPlatformProcess::CreateProc(*ExePath, *Pram, false, false, false, nullptr, 0, nullptr, nullptr);

	return Handle.IsValid();
}

FString FSimpleVersionControlModule::GetCurrentVersion()
{
	return ClientVersion.Name;
}

void FSimpleVersionControlModule::CompleteDownload(const FSimpleHttpRequest &Request, const FSimpleHttpResponse &Response, bool bConConnect)
{
	FString Name = FPaths::GetCleanFilename(Request.URL);
	auto PakVersion = FindPakVersion(Name);
	if (PakVersion)
	{
		EPakVersionType Type = PakVersion->PakVersionType;

		Name.RemoveFromEnd(TEXT(".pak"));

		EndFUpdateServerDelegate.ExecuteIfBound(Name, Type, bConConnect);
		EndSynchronizeServerVersionDelegate.ExecuteIfBound(Name, Type, bConConnect);
	}
}

void FSimpleVersionControlModule::Downloading(const FSimpleHttpRequest &Request, int32 BytesSent, int32 BytesReceived)
{
	FString Name = FPaths::GetCleanFilename(Request.URL);
	auto PakVersion = FindPakVersion(Name);
	if (PakVersion)
	{
		Name.RemoveFromEnd(TEXT(".pak"));

		if (PakVersion->Len > 0)
		{
			float Ratio = BytesReceived / PakVersion->Len;

			UpdateServerDelegate.ExecuteIfBound(Name, PakVersion->Len, BytesReceived, Ratio);
			SynchronizeServerVersionDelegate.ExecuteIfBound(Name, PakVersion->Len, BytesReceived, Ratio);
		}
	}
}

void FSimpleVersionControlModule::CompleteAllDownload()
{
	AllMissionAccomplishedDelegate.ExecuteIfBound(ClientVersion.Name);
	AllSynchronizeServerMissionAccomplishedDelegate.ExecuteIfBound(ClientVersion.Name);

	//当前的版本是否是最新的版本
//	IsCurrentVersionTheLatestVersion();
}

bool FSimpleVersionControlModule::Init()
{
	return false;
}

FString FSimpleVersionControlModule::GetServerVersion()
{
	return ServerVersionCache.Name;
}

bool FSimpleVersionControlModule::InitVersion(FFinishInitVersionDelegate InFinishInitVersionBPDelegate, FSimpleDelegate InFinishInitVersionDelegate)
{
	if (!bInitialization)
	{
		FSimpleHTTPResponseDelegate ResponseDelegate;
		ResponseDelegate.SimpleCompleteDelegate = FSimpleSingleCompleteDelegate::CreateRaw(this, &FSimpleVersionControlModule::InitServerVersionList);

		//这里是我们的补丁版本列表更新
		FString PatchVersionListName = GetDefault<USimpleVCSettings>()->PatchVersionListName.ToString();
		if (!SIMPLE_HTTP.GetObjectToMemory(ResponseDelegate, URL / PatchVersionListName))
		{
			return false;
		}

		//这里是我们的服务器版本
		ResponseDelegate.SimpleCompleteDelegate = FSimpleSingleCompleteDelegate::CreateRaw(this, &FSimpleVersionControlModule::InitServerVersion);
		FString ServerVersionName = GetDefault<USimpleVCSettings>()->ServerVersionName.ToString();
		if (!SIMPLE_HTTP.GetObjectToMemory(ResponseDelegate, URL / ServerVersionName))
		{
			return false;
		}

		//bind delegate
		FinishInitVersionBPDelegate = InFinishInitVersionBPDelegate;
		FinishInitVersionDelegate = InFinishInitVersionDelegate;

		bInitialization = true;
	}
	else
	{
		return false;
	}

	return true;
}


bool FSimpleVersionControlModule::InitVersion(FSimpleDelegate InFinishInitVersionDelegate)
{
	return InitVersion(FFinishInitVersionDelegate(),InFinishInitVersionDelegate);
}

void FSimpleVersionControlModule::InitCurrentVersion(const FSimpleHttpRequest &Request, const FSimpleHttpResponse &Response, bool bConConnect)
{
	if (bConConnect)
	{
		LastClientVersion = ClientVersion;

		ClientVersion.Empty();
		if (SimpleVersionControl::Read(Response.ResponseMessage, ClientVersion))
		{
			//更新补丁
			UpdatePatch();
		}
	}
}

void FSimpleVersionControlModule::InitServerVersion(const FSimpleHttpRequest &Request, const FSimpleHttpResponse &Response, bool bConConnect)
{
	if (bConConnect)
	{
		ServerVersionCache.Empty();
		if (SimpleVersionControl::Read(Response.ResponseMessage, ServerVersionCache))
		{
			CacheCompleteCount++;
		}
	}
}

void FSimpleVersionControlModule::InitServerVersionList(const FSimpleHttpRequest &Request, const FSimpleHttpResponse &Response, bool bConConnect)
{
	if (bConConnect)
	{
		VersionListCache.Empty();
		if (SimpleVersionControl::Read(Response.ResponseMessage, VersionListCache))
		{
			CacheCompleteCount++;
		}
	}
}


FRemoteDataDescribe *FSimpleVersionControlModule::FindPakVersion(const FString &Name)
{
	for (auto &Tmp : ClientVersion.Content)
	{
		FString LinkName = FPaths::GetCleanFilename(Tmp.Link);
		if (LinkName == Name)
		{
			return &Tmp;
		}
	}

	return NULL;
}

#undef LOCTEXT_NAMESPACE

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif

IMPLEMENT_MODULE(FSimpleVersionControlModule, SimpleVersionControl)