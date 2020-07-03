#pragma once
#include "Toolkits/AssetEditorToolkit.h"
#include "Tickable.h"

class UCustomAsset;
class FCustomAssetEditorToolkit : public FAssetEditorToolkit
{
	typedef FAssetEditorToolkit Super;
public:
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	void Initialize(UCustomAsset* InTextAsset, const EToolkitMode::Type InMode, const TSharedPtr<IToolkitHost>& InToolkitHost);

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

private:

	UCustomAsset* CustomTextAsset;
};