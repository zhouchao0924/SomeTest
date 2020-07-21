#pragma once
#include "EditorViewportClient.h"

class SIMPLEUNREALPAKVIEW_API FPakViewEditorViewportClient : public FEditorViewportClient, public TSharedFromThis<FPakViewEditorViewportClient>
{
public:
	FPakViewEditorViewportClient(FPreviewScene& InPreviewScene);

	virtual void Tick(float DeltaSeconds) override;
};