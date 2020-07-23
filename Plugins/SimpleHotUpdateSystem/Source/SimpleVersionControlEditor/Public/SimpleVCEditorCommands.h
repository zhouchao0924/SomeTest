// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SimpleVCEditorStyle.h"

class FSimpleVCEditorCommands : public TCommands<FSimpleVCEditorCommands>
{
public:

	FSimpleVCEditorCommands()
		: TCommands<FSimpleVCEditorCommands>(TEXT("SimpleVCEditorStyle"), NSLOCTEXT("Contexts", "SimpleVCEditorStyle", "SimpleVCEditorStyle Plugin"), NAME_None, FSimpleVCEditorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
