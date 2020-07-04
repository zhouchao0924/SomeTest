// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ModeEditorTest.h"
#include "ModeEditorTestEdMode.h"

#define LOCTEXT_NAMESPACE "FModeEditorTestModule"

void FModeEditorTestModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FModeEditorTestEdMode>(FModeEditorTestEdMode::EM_ModeEditorTestEdModeId, LOCTEXT("ModeEditorTestEdModeName", "ModeEditorTestEdMode"), FSlateIcon(), true);
}

void FModeEditorTestModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FModeEditorTestEdMode::EM_ModeEditorTestEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FModeEditorTestModule, ModeEditorTest)