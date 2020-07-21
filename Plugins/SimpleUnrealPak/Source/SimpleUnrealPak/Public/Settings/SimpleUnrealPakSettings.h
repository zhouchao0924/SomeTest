// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SimpleUnrealPakSettings.generated.h"

UENUM()
enum class EUnrealPakSettingsPlatform :uint8
{
	Windows,
	Linux,
	Mac,
	Android,
	iOS,
	HTML5,
};

UENUM()
enum class ECookTargetPlatform :uint8
{
	AllDesktop,
	Android,
	Android_ASTC,
	Android_ASTCClient,
	Android_ATC,
	Android_ATCClient,
	Android_DXT,
	Android_DXTClient,
	Android_ETC1,
	Android_ETC1a,
	Android_ETC1aClient,
	Android_ETC1Client,
	Android_ETC2,
	Android_ETC2Client,
	Android_Multi,
	Android_MultiClient,
	Android_PVRTC,
	Android_PVRTCClient,
	AndroidClient,
	HTML5,
	IOS,
	IOSClient,
	Linux,
	LinuxClient,
	LinuxNoEditor,
	LinuxServer,
	Lumin,
	LuminClient,
	Mac,
	MacClient,
	MacNoEditor,
	MacServer,
	TVOS,
	TVOSClient,
	Windows,
	WindowsClient,
	WindowsNoEditor,
	WindowsServer,
	MAX
};

/**
 * 
 */
UCLASS(config = SimpleUnrealPakSettings)
class SIMPLEUNREALPAK_API USimpleUnrealPakSettings : public UObject
{
	GENERATED_BODY()

public:
	USimpleUnrealPakSettings();
	

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealCommon, meta = (ToolTip = "AES specific encryption value ."))
		bool bSynch;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakSettings, meta = (ToolTip = "Save the path of the generated Pak ."))
		FString Version;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakSettings, meta = (ToolTip = "Save the path of the generated Pak ."))
		FDirectoryPath PakSavePaths;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakSettings, meta = (ToolTip = "Save the path of the log ."))
		FDirectoryPath PakLogPaths;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakSettings, meta = (ToolTip = "Corresponding to the packaged platform, the default is windows ."))
		EUnrealPakSettingsPlatform PakPlatform;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakSettings, meta = (ToolTip = "Whether to type a good cooked resource ."))
		bool bCooked;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakSettings, meta = (ToolTip = "Whether to obtain the dependent resources of the packaged resources during the packaging process and then package them ."))
		bool bReferenceAsset;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakSettings, meta = (ToolTip = "You get a smaller package ."))
		bool bCompress;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakSettings, meta = (ToolTip = "Strong conversion of encoding format to utf8 ."))
		bool bForceUTF8;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakSettings, meta = (ToolTip = "installed order"))
		bool bOrder;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakSettings, meta = (ToolTip = "AES encryption or not ."))
		bool bAES;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakSettings, meta = (DisplayName = "Secret key", EditCondition = "bAES",ToolTip = "AES specific encryption value ."))
		FGuid AES;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakCook, meta = (ToolTip = "AES specific encryption value ."))
		ECookTargetPlatform TargetPlatform;

	//UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakCook, meta = (ToolTip = "AES specific encryption value ."))
	//	bool bSkipCook;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakCook, meta = (ToolTip = "AES specific encryption value ."))
		bool bIgnoreCookErrors;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakCook, meta = (ToolTip = "AES specific encryption value ."))
		bool bCookPartialGC;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakCook, meta = (ToolTip = "AES specific encryption value ."))
		bool bCookInEditor;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakCook, meta = (ToolTip = "AES specific encryption value ."))
		FDirectoryPath CookOutputDir;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakCook, meta = (ToolTip = "AES specific encryption value ."))
		FString AdditionalCookerOptions;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakCook, meta = (ToolTip = "AES specific encryption value ."))
		bool bUnversionedCookedContent;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakCook, meta = (ToolTip = "AES specific encryption value ."))
		bool bSkipCookingEditorContent;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakCook, meta = (ToolTip = "AES specific encryption value ."))
		int32 NumCookersToSpawn;

	UPROPERTY(config, EditAnywhere, Category = SimpleUnrealPakCook, meta = (ToolTip = "AES specific encryption value ."))
		bool FastCook;
};
