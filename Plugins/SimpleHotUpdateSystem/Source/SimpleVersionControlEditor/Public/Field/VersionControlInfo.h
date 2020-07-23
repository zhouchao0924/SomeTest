// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VersionControlInfo.generated.h"

UCLASS(config = UVersionControlInfo)
class SIMPLEVERSIONCONTROLEDITOR_API UVersionControlInfo :public UObject
{
	GENERATED_BODY()

public:
	UVersionControlInfo();

	UPROPERTY(config, EditAnywhere, Category = "Hot update", meta = (ToolTip = "Hot update"))
	FFilePath MainVersion;

	UPROPERTY(config, EditAnywhere, Category = "Hot update", meta = (ToolTip = "Hot update"))
	FFilePath VersionEXE;

	UPROPERTY(config, EditAnywhere, Category = "Hot update", meta = (ToolTip = "Hot update"))
	FFilePath Sig;

	UPROPERTY(config, EditAnywhere, Category = "Hot update", meta = (ToolTip = "Hot update"))
	TArray<FFilePath> AdditionalDependent;

	UPROPERTY(config, EditAnywhere, Category = "Hot update", meta = (ToolTip = "Hot update"))
	TArray<FFilePath> DeletedObjectsInServer;
};