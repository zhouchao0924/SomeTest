// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SimpleHTTP.h"
#include "SimpleHttpManage.h"

#define LOCTEXT_NAMESPACE "FSimpleHTTPModule"

void FSimpleHTTPModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FSimpleHTTPModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FSimpleHttpManage::Destroy();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleHTTPModule, SimpleHTTP)