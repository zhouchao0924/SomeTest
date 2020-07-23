// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SimpleVCSettings.generated.h"

UENUM()
enum class EUploadServerType :uint8
{
	OSSServer,
	OtherServer
};

/**
 * 
 */
UCLASS(config = VersionControlSettings)
class SIMPLEVERSIONCONTROL_API USimpleVCSettings : public UObject
{
	GENERATED_BODY()

public:
	USimpleVCSettings();
	
	UPROPERTY(config, EditAnywhere, Category = HotUploadSettings, meta = (ToolTip = "Need to upload new version automatically."))
		bool bAuto;

	UPROPERTY(config, EditAnywhere, Category = HotUploadSettings, meta = (ToolTip = "Specify the address to upload."))
		FString URL;

	UPROPERTY(config, EditAnywhere, Category = HotUploadSettings, meta = (ToolTip = "Specify the type of server to upload."))
		EUploadServerType DataServerType;

	UPROPERTY(config, EditAnywhere, Category = OSSServer, meta = (ToolTip = "Specify the address to upload."))
		FString Bucket;

	UPROPERTY(config, EditAnywhere, Category = OSSServer, meta = (ToolTip = "Specify the address to upload."))
		FName DLCVersionListName;

	UPROPERTY(config, EditAnywhere, Category = OSSServer, meta = (ToolTip = "Specify the address to upload."))
		FName PatchVersionListName;

	UPROPERTY(config, EditAnywhere, Category = OSSServer, meta = (ToolTip = "Specify the address to upload."))
		FName ServerVersionName;

	UPROPERTY(config, EditAnywhere, Category = OSSServer, meta = (ToolTip = "Specify the address to upload."))
		FName DLCVersionPaths;

	UPROPERTY(config, EditAnywhere, Category = OSSServer, meta = (ToolTip = "Specify the address to upload."))
		FName PatchVersionPaths;
};
