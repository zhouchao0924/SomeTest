#pragma once

#include "SEditorViewport.h"
#include "CustomAsset.h"

class SCustomAssetViewport :public SEditorViewport
{
public:
	SLATE_BEGIN_ARGS(SCustomAssetViewport) { }

	SLATE_ATTRIBUTE(UCustomAsset*, CustomAsset)

	SLATE_END_ARGS()

public:

	void Construct(const FArguments& InArgs);

	~SCustomAssetViewport();

	//virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;

	FORCEINLINE TSharedPtr<class FCustomAssetViewportClient> &GetCustomAssetViewportClient() {return EditorViewportClient;}
private:
	class UCustomAsset* CustomAsset;

	TSharedPtr<class FAdvancedPreviewScene> PreviewScene;
	TSharedPtr<class FCustomAssetViewportClient> EditorViewportClient;
};