// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SimpleUnrealPakEidtorCommands.h"

#define LOCTEXT_NAMESPACE "FSimpleUnrealPakModule"

void FSimpleUnrealPakEditorCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "SimpleUnrealPak", "Execute SimpleUnrealPak action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
