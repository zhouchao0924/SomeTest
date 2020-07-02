// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ExtendEditorStyle.h"

class FExtendEditorCommands : public TCommands<FExtendEditorCommands>
{
public:

	FExtendEditorCommands()
		: TCommands<FExtendEditorCommands>(TEXT("ExtendEditor"), NSLOCTEXT("Contexts", "ExtendEditor", "ExtendEditor Plugin"), NAME_None, FExtendEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
