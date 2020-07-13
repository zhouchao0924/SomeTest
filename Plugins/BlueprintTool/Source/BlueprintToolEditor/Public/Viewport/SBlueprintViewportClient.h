#pragma once

#include "SEditorViewport.h"
#include "AssetEditor/Architect/AssetArchitectToolkit.h"
#include "BlueprintData.h"


class BLUEPRINTTOOLEDITOR_API SBlueprintPreviewViewport : public SEditorViewport
{
public:
	SLATE_BEGIN_ARGS(SBlueprintPreviewViewport) {}
	SLATE_ARGUMENT(TWeakPtr<FBlueprintToolEditorToolkit>, BPEditorPtr)
	SLATE_ARGUMENT(UBlueprintData*, ObjectToEdit)
	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs);

protected:
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;

private:
	TWeakPtr<FBlueprintToolEditorToolkit> BPEditorPtr;
	UBlueprintData* ObjectToEdit;

	TSharedPtr<class FAdvancedPreviewScene> PreviewScene;

	class USkyLightComponent* Skylight;
	class UAtmosphericFogComponent* AtmosphericFog;

	TSharedPtr<class FBPTEditorViewportClient> EditorViewportClient;
};