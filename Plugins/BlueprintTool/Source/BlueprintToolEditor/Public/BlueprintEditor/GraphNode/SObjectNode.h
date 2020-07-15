#pragma once

#include "SGraphNode.h"
#include "BlueprintEditor/Core/Architect/K3Node.h"
#include "SVariableNode.h"

class SObjectNode : public SVariableNode
{
public:
	SLATE_BEGIN_ARGS(SObjectNode)
	{}
	SLATE_END_ARGS()

	struct FObjectAssetData
	{
		UObject* Object;
		FAssetData AssetData;

		FObjectAssetData(UObject* InObject = NULL)
			:Object(InObject)
		{
			AssetData = InObject != NULL && !InObject->IsA<AActor>() ? FAssetData(InObject) : FAssetData();
		}

		FObjectAssetData(const FAssetData& InAssetData)
			: Object(NULL)
			, AssetData(InAssetData)
		{
		}

		bool IsValid() const
		{
			return Object != NULL || AssetData.IsValid();
		}
	};

	void Construct(const FArguments& InArgs, UK3Node* MarkerNode);

	virtual void UpdateGraphNode() override;
	void SetThumbnail(UObject* AssetObject);
	void BuildThumbnail();
	void UpdateObjectThumbnailWidget();
	bool GetValue(FObjectAssetData& OutValue) const;
private:
	FIntPoint ThumbnailSize;//ËõÂÔÍ¼¾«¶È
	TSharedPtr<class FAssetThumbnail> AssetThumbnail;
};