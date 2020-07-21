// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SimpleUnrealPakViewStyle.h"

class FSimpleUnrealPakViewCommands : public TCommands<FSimpleUnrealPakViewCommands>
{
public:

	FSimpleUnrealPakViewCommands()
		: TCommands<FSimpleUnrealPakViewCommands>(TEXT("SimpleUnrealPakView"), NSLOCTEXT("Contexts", "SimpleUnrealPakView", "SimpleUnrealPakView Plugin"), NAME_None, FSimpleUnrealPakViewStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};