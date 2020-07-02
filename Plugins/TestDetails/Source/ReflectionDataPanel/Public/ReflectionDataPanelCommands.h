// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ReflectionDataPanelStyle.h"

class FReflectionDataPanelCommands : public TCommands<FReflectionDataPanelCommands>
{
public:

	FReflectionDataPanelCommands()
		: TCommands<FReflectionDataPanelCommands>(TEXT("ReflectionDataPanel"), NSLOCTEXT("Contexts", "ReflectionDataPanel", "ReflectionDataPanel Plugin"), NAME_None, FReflectionDataPanelStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};