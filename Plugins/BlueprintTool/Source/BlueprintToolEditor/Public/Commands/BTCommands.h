//$ Copyright 2015 Ali Akbar, Code Respawn Technologies Pvt Ltd - All Rights Reserved $//
#pragma once

/*Command for special output*/
class BLUEPRINTTOOLEDITOR_API FBTCommands : public TCommands<FBTCommands>
{
public:
	FBTCommands();

	virtual void RegisterCommands() override;

public:

	TSharedPtr< FUICommandInfo > Compile;
	TSharedPtr< FUICommandInfo > Help;
	TSharedPtr< FUICommandInfo > Run;
};