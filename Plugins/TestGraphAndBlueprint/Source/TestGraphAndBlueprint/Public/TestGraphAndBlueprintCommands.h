// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "TestGraphAndBlueprintStyle.h"

class FTestGraphAndBlueprintCommands : public TCommands<FTestGraphAndBlueprintCommands>
{
public:

	FTestGraphAndBlueprintCommands()
		: TCommands<FTestGraphAndBlueprintCommands>(TEXT("TestGraphAndBlueprint"), NSLOCTEXT("Contexts", "TestGraphAndBlueprint", "TestGraphAndBlueprint Plugin"), NAME_None, FTestGraphAndBlueprintStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};