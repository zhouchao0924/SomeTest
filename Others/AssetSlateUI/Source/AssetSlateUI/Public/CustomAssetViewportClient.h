#pragma once
#include "EditorViewportClient.h"
#include "CustomAsset.h"

class UProceduralMeshComponent;

class FCustomAssetViewportClient :public FEditorViewportClient, public TSharedFromThis<FCustomAssetViewportClient>
{
public:
	FCustomAssetViewportClient(FPreviewScene& InPreviewScene, UCustomAsset* InCustomAsset, const TWeakPtr<class SEditorViewport> &InEdiotrViewprtWdget = nullptr);

	virtual void Tick(float DeltaSeconds) override;

	void OnPropertyChanged();

	bool CreateMeshLOD(int32 LODNumber = 1);

private:

	UCustomAsset* CustomAsset;

	TArray<UProceduralMeshComponent*> ProceduralMesh;

	FCustomAssetCache CustomAssetCache;
};