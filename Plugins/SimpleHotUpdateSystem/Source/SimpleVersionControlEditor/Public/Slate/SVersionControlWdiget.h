// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UVersionControlInfo;
class USimpleUnrealPakSettings;
class USimpleVCSettings;

class SIMPLEVERSIONCONTROLEDITOR_API SVersionControlWdiget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SVersionControlWdiget)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UVersionControlInfo *Infor, USimpleUnrealPakSettings *InPakSettings, USimpleVCSettings *InVCSettings);

	FReply OnUpdateMainVersion();
private:
	TSharedPtr<class IDetailsView> ConfigPanel;
	TSharedPtr<class IDetailsView> ConfigPanelSettings;
	TSharedPtr<class IDetailsView> PakPanelSettings;
};