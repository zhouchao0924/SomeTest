// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SimpleUnrealPakEditorStyle.h"

class FSimpleUnrealPakEditorCommands : public TCommands<FSimpleUnrealPakEditorCommands>
{
public:

	FSimpleUnrealPakEditorCommands()
		: TCommands<FSimpleUnrealPakEditorCommands>(TEXT("SimpleUnrealPak"), NSLOCTEXT("Contexts", "SimpleUnrealPak", "SimpleUnrealPak Plugin"), NAME_None, FSimpleUnrealPakEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
