#pragma once
#include "AssetThumbnail.h"

class FObjectEditorThumbnailPool : public FAssetThumbnailPool
{
public:
	FObjectEditorThumbnailPool(int32 NumObjectsInPool) : FAssetThumbnailPool(NumObjectsInPool) {}
	static TSharedPtr<FObjectEditorThumbnailPool> Get() { return Instance; }
	static void Create()
	{
		Instance = MakeShareable(new FObjectEditorThumbnailPool(512));
	}
private:
	static TSharedPtr<FObjectEditorThumbnailPool> Instance;
};

//TSharedPtr<FObjectEditorThumbnailPool> FObjectEditorThumbnailPool::Instance;