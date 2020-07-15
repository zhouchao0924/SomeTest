// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UBTCuEdMode.h"
#include "UBTCuEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FUBTCuEdMode::EM_UBTCuEdModeId = TEXT("EM_UBTCuEdMode");

FUBTCuEdMode::FUBTCuEdMode()
{

}

FUBTCuEdMode::~FUBTCuEdMode()
{

}

void FUBTCuEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FUBTCuEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FUBTCuEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FUBTCuEdMode::UsesToolkits() const
{
	return true;
}




