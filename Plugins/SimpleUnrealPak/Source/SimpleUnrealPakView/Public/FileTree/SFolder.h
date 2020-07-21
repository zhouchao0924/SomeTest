// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "FileParsing.h"

class SFolder :public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SFolder)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const SimpleUnrealPakView::FFileList &FileList);

	const FSlateBrush* GetFileTypeICO() const;

private:
	TSharedPtr<class SExpandableArea> Area;
};