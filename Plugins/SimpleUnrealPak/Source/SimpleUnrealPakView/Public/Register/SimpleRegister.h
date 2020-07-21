// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class USimpleUEViewConfigurationInfor;

class SimpleRegister : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SimpleRegister)
	{}
	SLATE_END_ARGS()

	virtual void Construct(const FArguments& InArgs, USimpleUEViewConfigurationInfor *Infor);

	FReply OnClicked();

private:
	TSharedPtr<class IDetailsView> ConfigPanel;
	USimpleUEViewConfigurationInfor *ConfigInfor;
	FString LastPaths;
};