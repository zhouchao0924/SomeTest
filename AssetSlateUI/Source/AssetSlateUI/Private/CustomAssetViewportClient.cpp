#include "CustomAssetViewportClient.h"
#include "AssetEditorModeManager.h"
#include "SEditorViewport.h"

FCustomAssetViewportClient::FCustomAssetViewportClient(/*FPreviewScene& InPreviewScene,*/ /*UCustomAsset* InCustomAsset,*/ const TWeakPtr<SEditorViewport> &InEdiotrViewprtWdget /*= nullptr*/)
	: FEditorViewportClient(nullptr, &DrawScene, InEdiotrViewprtWdget)
	//, CustomAsset(/*InCustomAsset*/nullptr)
{
	PreviewScene = &DrawScene;
	//((FAssetEditorModeManager*)ModeTools)->SetPreviewScene(PreviewScene);

	//DrawHelper.bDrawGrid = true;
	//DrawHelper.bDrawPivot = true;

	//SetRealtime(true);

	//SetViewMode(VMI_Lit);

	//SetViewLocation(FVector(500, 300, 500));

	//CreateMeshLOD();

	//OnPropertyChanged();
}

//void FCustomAssetViewportClient::Tick(float DeltaSeconds)
//{
//	//FEditorViewportClient::Tick(DeltaSeconds);
//
//	//DrawScene.GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
//}

//void FCustomAssetViewportClient::OnPropertyChanged()
//{
//	
//}
//
//bool FCustomAssetViewportClient::CreateMeshLOD(int32 LODNumber)
//{
//	bool bCreateMeshLOD = false;
//
//	return bCreateMeshLOD;
//}
