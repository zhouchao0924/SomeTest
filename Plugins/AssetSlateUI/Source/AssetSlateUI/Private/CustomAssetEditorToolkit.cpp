#include "CustomAssetEditorToolkit.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Docking/SDockTab.h"
#include "CustomAsset.h"
//#include "SCustomAssetViewport.h"
//#include "CustomDataattribute.h"

#define LOCTEXT_NAMESPACE "CustomAssetEditor"

namespace CustomAssetEditor
{
	static const FName AppId("CustomAssetEditorApp");
	static const FName AssetEditorID("CustomAssetEd");

	static const FName CustomPanelID("CustomPanelEd");
}

void FCustomAssetEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	Super::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(CustomAssetEditor::AssetEditorID, FOnSpawnTab::CreateLambda(
		[&](const FSpawnTabArgs &Args)
	{
		return SNew(SDockTab)
			[
				SNew(SImage)
				//SNew(SCustomAssetViewport)
				//.CustomAsset(CustomTextAsset)
			];
	}));

	InTabManager->RegisterTabSpawner(CustomAssetEditor::CustomPanelID, FOnSpawnTab::CreateLambda(
		[&](const FSpawnTabArgs &Args)
	{
		return SNew(SDockTab)
			[
				SNew(SImage)
				//SNew(SCustomDataAttribute)
				//.CustomAsset(CustomTextAsset)
			];
	}));
}

void FCustomAssetEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	Super::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(CustomAssetEditor::AssetEditorID);
}

void FCustomAssetEditorToolkit::Initialize(UCustomAsset* InTextAsset, const EToolkitMode::Type InMode, const TSharedPtr<IToolkitHost>& InToolkitHost)
{
	CustomTextAsset = InTextAsset;

	const TSharedRef<FTabManager::FLayout> StandloneCustomLayout = FTabManager::NewLayout("StandloneCustomLayout_Layout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(CustomAssetEditor::AssetEditorID, ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(CustomAssetEditor::CustomPanelID, ETabState::OpenedTab)
			)
		);

	InitAssetEditor(
		InMode,
		InToolkitHost,
		CustomAssetEditor::AppId,
		StandloneCustomLayout,
		true,
		true,
		InTextAsset
	);

	RegenerateMenusAndToolbars();
}

FName FCustomAssetEditorToolkit::GetToolkitFName() const
{
	return FName("CustomAssetEditor");
}

FText FCustomAssetEditorToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "Custom Asset Editor");
}

FString FCustomAssetEditorToolkit::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "CustomAsset ").ToString();
}

FLinearColor FCustomAssetEditorToolkit::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
}

#undef LOCTEXT_NAMESPACE