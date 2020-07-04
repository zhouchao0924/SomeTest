// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ModeEditorTestEdMode.h"
#include "ModeEditorTestEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FModeEditorTestEdMode::EM_ModeEditorTestEdModeId = TEXT("EM_ModeEditorTestEdMode");

FModeEditorTestEdMode::FModeEditorTestEdMode()
{

}

FModeEditorTestEdMode::~FModeEditorTestEdMode()
{

}

void FModeEditorTestEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FModeEditorTestEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FModeEditorTestEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FModeEditorTestEdMode::UsesToolkits() const
{
	return true;
}




