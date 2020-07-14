#pragma once
#include "EditorViewportClient.h"

class UBlueprintData;
class SBlueprintPreviewViewport;

class BLUEPRINTTOOLEDITOR_API FBPTEditorViewportClient : public FEditorViewportClient, public TSharedFromThis<FBPTEditorViewportClient>
{
public:
	FBPTEditorViewportClient(
		TWeakPtr<FAssetEditorToolkit> InBlueprintEditor, 
		TWeakPtr<SBlueprintPreviewViewport> InBlueprintEditorViewport, 
		FPreviewScene& InPreviewScene,
		UBlueprintData* InData);

	virtual void Tick(float DeltaSeconds) override;
};