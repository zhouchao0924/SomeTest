#include "TestBlueprintToolEditor.h"
#include "AssetEditor/Architect/AssetArchitectActions.h"
#include "AssetEditor/Architect/AssetArchitectToolkit.h"
#include "TestObject.h"


#define LOCTEXT_NAMESPACE "TestBlueprintToolEditor"

FText FTestBlueprintToolEditor::GetName() const
{
	return LOCTEXT("FTestBlueprintToolEditorName", "TestObject");
}

FColor FTestBlueprintToolEditor::GetTypeColor() const
{
	return FColor::Orange; /*Purple*/
}

UClass* FTestBlueprintToolEditor::GetSupportedClass() const
{
	return UTestObject::StaticClass();
}

void FTestBlueprintToolEditor::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{

}

void FTestBlueprintToolEditor::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor /*= TSharedPtr<IToolkitHost>()*/)
{
	//UE4自带的资源编辑器
	FSimpleAssetEditor::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, InObjects);

	//EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	//for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	//{
	//	auto BlueprintDataAsset = Cast<UBlueprintData>(*ObjIt);

	//	if (BlueprintDataAsset != nullptr)
	//	{
	//		TSharedRef<FBlueprintToolEditorToolkit> EditorToolkit = MakeShareable(new FBlueprintToolEditorToolkit());
	//		EditorToolkit->Initialize(BlueprintDataAsset, Mode, EditWithinLevelEditor);
	//	}
	//}
}

uint32 FTestBlueprintToolEditor::GetCategories()
{
	return EAssetTypeCategories::Basic;
}

#undef LOCTEXT_NAMESPACE