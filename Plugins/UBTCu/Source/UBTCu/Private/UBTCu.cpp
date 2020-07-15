// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UBTCu.h"
#include "UBTCuEdMode.h"

#define LOCTEXT_NAMESPACE "FUBTCuModule"

void FUBTCuModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FUBTCuEdMode>(FUBTCuEdMode::EM_UBTCuEdModeId, LOCTEXT("UBTCuEdModeName", "UBTCuEdMode"), FSlateIcon(), true);
}

void FUBTCuModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FUBTCuEdMode::EM_UBTCuEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUBTCuModule, UBTCu)