// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SimpleVCEditorCommands.h"

#define LOCTEXT_NAMESPACE "FSimpleVCEditorCommands"

void FSimpleVCEditorCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "SimpleVCEditorCommands", "Execute SimpleVCEditorCommands action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
