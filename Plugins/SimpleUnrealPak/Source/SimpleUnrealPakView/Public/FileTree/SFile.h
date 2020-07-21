// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "FileParsing.h"

class SFile :public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SFile)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const SimpleUnrealPakView::FFileList &FileList);

	FReply OnClicked();

private:

	FString AssetPaths;
};