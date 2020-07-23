#pragma once

#include "CoreMinimal.h"
#include "Version/VersionInfor.h"
#include "SimpleVersionControlType.generated.h"

UENUM()
enum class EServerVersionResponseType :uint8
{
	VERSION_DIFF,//更新补丁版本
	CRC_DIFF,//热更新
	EQUAL,
	MAX,
};

//BP
DECLARE_DYNAMIC_DELEGATE(FFinishInitVersionDelegate);
DECLARE_DYNAMIC_DELEGATE_OneParam(FStartSynchronizeServerVersionDelegate,FString , Version);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FEndSynchronizeServerVersionDelegate, FString, Packages, EPakVersionType, PakVersionType, bool, bSuccess);
DECLARE_DYNAMIC_DELEGATE_FourParams(FSynchronizeServerVersionDelegate, FString, DLCName, int32, TotalBytes, int32, BytesReceived, float, Ratio);

//C++
DECLARE_DELEGATE_OneParam(FStartUpdateServerDelegate, FString);
DECLARE_DELEGATE_ThreeParams(FEndFUpdateServerDelegate, FString, EPakVersionType, bool);
DECLARE_DELEGATE_FourParams(FUpdateServerDelegate, FString, int32, int32,float);