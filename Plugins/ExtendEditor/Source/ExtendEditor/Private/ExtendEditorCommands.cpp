// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ExtendEditorCommands.h"

#define LOCTEXT_NAMESPACE "FExtendEditorModule"

void FExtendEditorCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "ExtendEditor", "Execute ExtendEditor action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
