// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SomeTest.h"
#include "Modules/ModuleManager.h"
#include "Slate/MenuStyles.h"
#include "Styling/SlateStyleRegistry.h"

//Custom implementation of the Default Game Module. 
class FSomeTestGameModule : public FDefaultGameModuleImpl
{
	// Called whenever the module is starting up. In here, we unregister any style sets 
	// (which may have been added by other modules) sharing our 
	// style set's name, then initialize our style set. 
	virtual void StartupModule() override
	{
		//Hot reload hack
		FSlateStyleRegistry::UnRegisterSlateStyle(FMenuStyles::GetStyleSetName());
		FMenuStyles::Initialize();
	}

	// Called whenever the module is shutting down. Here, we simply tell our MenuStyles to shut down.
	virtual void ShutdownModule() override
	{
		FMenuStyles::Shutdown();
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FSomeTestGameModule, SomeTest, "SomeTest" );
