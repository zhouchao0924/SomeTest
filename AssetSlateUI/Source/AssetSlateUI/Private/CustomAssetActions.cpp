#include "CustomAssetActions.h"
#include "Widgets/Images/SImage.h"
#include "CustomAsset.h"
#include "CustomAssetEditorToolkit.h"


#define LOCTEXT_NAMESPACE "FCustomAssetActions"

TSharedPtr<class SWidget> FCustomAssetActions::GetThumbnailOverlay(const FAssetData& AssetData) const
{
	return SNew(SImage);
}

UClass* FCustomAssetActions::GetSupportedClass() const
{
	return UCustomAsset::StaticClass();
}

FColor FCustomAssetActions::GetTypeColor() const
{
	return FColor::White;
}

void FCustomAssetActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor /*= TSharedPtr<IToolkitHost>()*/)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid()? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		auto CustomAsset = Cast<UCustomAsset>(*ObjIt);

		if (CustomAsset != nullptr)
		{
			TSharedRef<FCustomAssetEditorToolkit> EditorToolkit = MakeShareable(new FCustomAssetEditorToolkit());
			EditorToolkit->Initialize(CustomAsset, Mode, EditWithinLevelEditor);
		}
	}
}

FText FCustomAssetActions::GetName() const
{
	return LOCTEXT("AssetTypeActions_CustomAsset", "Custom Asset");
}

uint32 FCustomAssetActions::GetCategories()
{
	return EAssetTypeCategories::UI;
}


#undef LOCTEXT_NAMESPACE