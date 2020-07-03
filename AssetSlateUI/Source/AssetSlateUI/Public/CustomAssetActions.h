#pragma once
#include "AssetTypeActions_Base.h"

class FCustomAssetActions: public FAssetTypeActions_Base
{
public:

	//可以覆盖当前资源图标
	virtual TSharedPtr<class SWidget> GetThumbnailOverlay(const FAssetData& AssetData) const override;

	//可编辑的资源类型
	virtual UClass* GetSupportedClass() const override;

	//显示图标的颜色
	virtual FColor GetTypeColor() const override;

	//当双击资源的时调用，请求打开编辑器实例
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

	virtual FText GetName() const override;
	virtual uint32 GetCategories() override;
};