#include "AssetEditor/Architect/AssetArchitectToolkit.h"
#include "EditorStyleSet.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "GraphEditor.h"
#include "Viewport/SBlueprintViewportClient.h"
#include "Widgets/Docking/SDockTab.h"
#include "Blueprint/Architect/BPArchitectEdGraph.h"
#include "Palette/BlueprintNodeListPalette.h"


#define LOCTEXT_NAMESPACE "BlueprintToolEditorToolkit"

struct FBPToolID
{
	static const FName ViewportID;
	static const FName PreviewID;
	static const FName ContentBrowserID;
	static const FName DetailsID;
	static const FName BPNodeListID;
	static const FName BlueprintGraphID;
	static const FName PreviewSettingsID;
};

const FName FBPToolID::PreviewID(TEXT("PreviewID"));
const FName FBPToolID::ViewportID(TEXT("ViewportID"));
const FName FBPToolID::ContentBrowserID(TEXT("ContentBrowserID"));
const FName FBPToolID::DetailsID(TEXT("DetailsID"));
const FName FBPToolID::BPNodeListID(TEXT("BPNodeListID"));
const FName FBPToolID::BlueprintGraphID(TEXT("BlueprintGraphID"));
const FName FBPToolID::PreviewSettingsID(TEXT("PreviewSettingsID"));

void FBlueprintToolEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	Super::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(FBPToolID::PreviewID, FOnSpawnTab::CreateSP(this, &FBlueprintToolEditorToolkit::SpawnByPreviewTab))
		.SetDisplayName(LOCTEXT("PreviewTab", "BP_Preview"))
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Viewports"));

	InTabManager->RegisterTabSpawner(FBPToolID::ContentBrowserID, FOnSpawnTab::CreateSP(this, &FBlueprintToolEditorToolkit::SpawnByContentBrowserTab))
		.SetDisplayName(LOCTEXT("ContentBrowserLabel", "BP_Content Browser"))
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.ContentBrowser"));

	InTabManager->RegisterTabSpawner(FBPToolID::BlueprintGraphID, FOnSpawnTab::CreateSP(this, &FBlueprintToolEditorToolkit::SpawnByBlueprintViewTab))
		.SetDisplayName(LOCTEXT("BlueprintGraphTab", "BP_BlueprintGraph"))
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Viewports"));

	InTabManager->RegisterTabSpawner(FBPToolID::BPNodeListID, FOnSpawnTab::CreateSP(this, &FBlueprintToolEditorToolkit::SpawnByPaletteTab))
		.SetDisplayName(LOCTEXT("ActionsTabLabel", "MyNodeList"))
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	InTabManager->RegisterTabSpawner(FBPToolID::DetailsID, FOnSpawnTab::CreateSP(this, &FBlueprintToolEditorToolkit::SpawnByDetailsTab))
		.SetDisplayName(LOCTEXT("DetailsTabLabel", "MyDetails"))
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FBlueprintToolEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	Super::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(FBPToolID::PreviewID);
	InTabManager->UnregisterTabSpawner(FBPToolID::ContentBrowserID);
	InTabManager->UnregisterTabSpawner(FBPToolID::BlueprintGraphID);
}

void FBlueprintToolEditorToolkit::Initialize(UBlueprintData* InTextAsset, const EToolkitMode::Type InMode, const TSharedPtr<IToolkitHost>& InToolkitHost)
{
	//´´½¨À¶Í¼
	{
		UBPArchitectEdGraph* EDGraph = NewObject<UBPArchitectEdGraph>(InTextAsset, UBPArchitectEdGraph::StaticClass(), NAME_None, RF_Transactional);
		EDGraph->InitializeGraph();

		GraphEditor = CreateBPGraphEditor(EDGraph);
	}

	PreviewViewport = SNew(SBlueprintPreviewViewport)
		.BPEditorPtr(SharedThis(this))
		.ObjectToEdit(InTextAsset);

	const TSharedRef<FTabManager::FLayout> StandloneCustomLayout = FTabManager::NewLayout("StandloneBlueprintLayout_Layout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.10f)
				->SetHideTabWell(true)
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Horizontal)
				->SetSizeCoefficient(0.90f)
				->Split
				(
					FTabManager::NewSplitter()
					->SetOrientation(Orient_Vertical)
					->SetSizeCoefficient(0.20f)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.33f)
						->SetHideTabWell(true)
						->AddTab(FBPToolID::PreviewID, ETabState::OpenedTab)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.66f)
						->SetHideTabWell(true)
						->AddTab(FBPToolID::DetailsID, ETabState::OpenedTab)
					)
				)
				->Split
				(
					FTabManager::NewSplitter()
					->SetOrientation(Orient_Vertical)
					->SetSizeCoefficient(0.60f)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.66f)
						->SetHideTabWell(true)
						->AddTab(FBPToolID::BlueprintGraphID, ETabState::OpenedTab)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.33f)
						->SetHideTabWell(true)
						->AddTab(FBPToolID::ContentBrowserID, ETabState::OpenedTab)
					)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.20f)
					->SetHideTabWell(true)
					->AddTab(FBPToolID::BPNodeListID, ETabState::OpenedTab)
				)
			)
		);
	/*->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(FBPToolID::PreviewID, ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(FBPToolID::ContentBrowserID, ETabState::OpenedTab)
			)
		);*/

	InitAssetEditor(
		InMode,
		InToolkitHost,
		FBPToolID::ViewportID,
		StandloneCustomLayout,
		true,
		true,
		InTextAsset
	);

	RegenerateMenusAndToolbars();
}

FName FBlueprintToolEditorToolkit::GetToolkitFName() const
{
	return FName("BlueprintToolEditorToolkit");
}

FText FBlueprintToolEditorToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("BlueprintToolLabel", "BlueprintTool Asset Editor");
}

FString FBlueprintToolEditorToolkit::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("BlueprintToolEditorPrefix", "BlueprintData").ToString();
}

FLinearColor FBlueprintToolEditorToolkit::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
}

TSharedRef<SDockTab> FBlueprintToolEditorToolkit::SpawnByPreviewTab(const FSpawnTabArgs& Args)
{
	TSharedRef<SDockTab> SpawnedTab =
		SNew(SDockTab)
		.Label(LOCTEXT("MeshComponentPreview", "Preview"))
		.TabColorScale(GetTabColorScale())
		[
			PreviewViewport.ToSharedRef()
		];

	return SpawnedTab;
}

TSharedRef<SDockTab> FBlueprintToolEditorToolkit::SpawnByContentBrowserTab(const FSpawnTabArgs& Args)
{
	TSharedRef<SDockTab> SpawnedTab =
		SNew(SDockTab)
		.Label(LOCTEXT("BPContentBrowserKey", "BluePrint Content Browser"))
		.TabColorScale(GetTabColorScale())
		[
			SNullWidget::NullWidget
		];

	IContentBrowserSingleton& ContentBrowserSingleton = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser").Get();
	FName ContentBrowserID = *("BP_ContentBrowser_" + FGuid::NewGuid().ToString());
	FContentBrowserConfig ContentBrowserConfig;

	TSharedRef<SWidget> ContentBrowser = ContentBrowserSingleton.CreateContentBrowser(ContentBrowserID, SpawnedTab, &ContentBrowserConfig);
	SpawnedTab->SetContent(ContentBrowser);

	return SpawnedTab;
}

TSharedRef<SDockTab> FBlueprintToolEditorToolkit::SpawnByBlueprintViewTab(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
	.Label(LOCTEXT("BPGraph", "BP Graph"))
		.TabColorScale(GetTabColorScale())
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
		[
			GraphEditor.ToSharedRef()
		]
		];
}

TSharedRef<SDockTab> FBlueprintToolEditorToolkit::SpawnByPaletteTab(const FSpawnTabArgs& Args)
{
	ActionPalette = SNew(SBlueprintNodeListPalette, SharedThis(this));

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("Kismet.Tabs.Palette"))
		.Label(LOCTEXT("BToolPlaetteTitle", "Hello Hello"))
		[
			SNew(SBox)
			.AddMetaData<FTagMetaData>(FTagMetaData(TEXT("BToolPalette")))
			[
				ActionPalette.ToSharedRef()
			]
		];
	return SpawnedTab;
}

TSharedRef<SDockTab> FBlueprintToolEditorToolkit::SpawnByDetailsTab(const FSpawnTabArgs& Args)
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	const FDetailsViewArgs DetailsViewArgs(
		false,
		false,
		true,
		FDetailsViewArgs::HideNameArea,
		true,
		this);

	TSharedRef<IDetailsView> PropertyEditorRef = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	PropertyEditor = PropertyEditorRef;

	return SNew(SDockTab)
		.Label(LOCTEXT("BTDetailsTab", "Hello Details"))
		[
			PropertyEditorRef
		];
}

TSharedRef<SGraphEditor> FBlueprintToolEditorToolkit::CreateBPGraphEditor(UEdGraph* InGraph)
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("MyAppearanceCornerText", "BP Editor");

	TSharedRef<SWidget> TitleBarWidget =
		SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush(TEXT("Graph.TitleBackground")))
		.HAlign(HAlign_Fill)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.FillWidth(1.f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("MyText", "Hello C"))
			.TextStyle(FEditorStyle::Get(), TEXT("GraphBreadcrumbButtonText"))
		]
		];
		
	TSharedRef<SGraphEditor> GraphEditorInstance = SNew(SGraphEditor)
		.Appearance(AppearanceInfo)
		.TitleBar(TitleBarWidget)
		.GraphToEdit(InGraph);

	return GraphEditorInstance;
}

#undef LOCTEXT_NAMESPACE