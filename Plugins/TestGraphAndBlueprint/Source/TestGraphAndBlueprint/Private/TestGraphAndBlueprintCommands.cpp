// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TestGraphAndBlueprintCommands.h"

#define LOCTEXT_NAMESPACE "FTestGraphAndBlueprintModule"

void FTestGraphAndBlueprintCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "TestGraphAndBlueprint", "Bring up TestGraphAndBlueprint window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
