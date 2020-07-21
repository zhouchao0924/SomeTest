// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SimpleUnrealPakViewCommands.h"

#define LOCTEXT_NAMESPACE "FSimpleUnrealPakViewModule"

void FSimpleUnrealPakViewCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "SimpleUnrealPakView", "Bring up SimpleUnrealPakView window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
