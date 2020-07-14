#include "AssetEditor/Architect/AssetArchitectActions.h"
#include "BlueprintData.h"
#include "AssetEditor/Architect/AssetArchitectToolkit.h"


#define LOCTEXT_NAMESPACE "FBlueprintTypeActions"

FText FBlueprintTypeActions::GetName() const
{
	return LOCTEXT("FBlueprintTypeActionsName", "BlueprintTool Theme");
}

FColor FBlueprintTypeActions::GetTypeColor() const
{
	return FColor::Cyan;
}

UClass* FBlueprintTypeActions::GetSupportedClass() const
{
	return UBlueprintData::StaticClass();
}

void FBlueprintTypeActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{

}

void FBlueprintTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor /*= TSharedPtr<IToolkitHost>()*/)
{
	//UE4自带的资源编辑器
	//FSimpleAssetEditor::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, InObjects);

	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		auto BlueprintDataAsset = Cast<UBlueprintData>(*ObjIt);

		if (BlueprintDataAsset != nullptr)
		{
			TSharedRef<FBlueprintToolEditorToolkit> EditorToolkit = MakeShareable(new FBlueprintToolEditorToolkit());
			EditorToolkit->Initialize(BlueprintDataAsset, Mode, EditWithinLevelEditor);
		}
	}
}

uint32 FBlueprintTypeActions::GetCategories()
{
	return EAssetTypeCategories::Basic;
}

#undef LOCTEXT_NAMESPACE