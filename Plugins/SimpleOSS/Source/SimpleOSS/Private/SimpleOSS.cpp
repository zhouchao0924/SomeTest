// Copyright (C) RenZhai.2019.All Rights Reserved.

#include "SimpleOSS.h"
#include "SimpleOSSManage.h"

#define LOCTEXT_NAMESPACE "FSimpleOSSModule"

void FSimpleOSSModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FSimpleOSSModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSimpleOSSManage::Destroy();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleOSSModule, SimpleOSS)