// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ReflectionDataPanelCommands.h"

#define LOCTEXT_NAMESPACE "FReflectionDataPanelModule"

void FReflectionDataPanelCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "ReflectionDataPanel", "Bring up ReflectionDataPanel window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
