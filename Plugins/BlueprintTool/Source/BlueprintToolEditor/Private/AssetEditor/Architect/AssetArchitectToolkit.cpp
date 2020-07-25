#include "AssetEditor/Architect/AssetArchitectToolkit.h"
#include "Viewport/SBlueprintViewportClient.h"
#include "EditorStyleSet.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "GraphEditor.h"
#include "Palette/BlueprintNodeListPalette.h"
#include "PropertyEditorModule.h"
#include "SAdvancedPreviewDetailsTab.h"
#include "Framework/Commands/GenericCommands.h"
#include "Windows/WindowsPlatformApplicationMisc.h"
#include "EdGraphUtilities.h"
#include "ScopedTransaction.h"
#include "SNodePanel.h"
#include "Commands/BTCommands.h"
#include "Compilation/BTCompilationUtilities.h"
#include "SimpleCodeEvent.h"
#include "BlueprintEditor/Core/Architect/BPArchitectEdGraph.h"
#include "Widgets/Docking/SDockTab.h"
#include "BlueprintEditor/GraphNode/SObjectEditorDropTarget.h"
#include "Widgets/Text/STextBlock.h"

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

FBlueprintToolEditorToolkit::~FBlueprintToolEditorToolkit()
{
	if (GraphEditor->GetCurrentGraph())
	{
		GraphEditor->GetCurrentGraph()->RemoveOnGraphChangedHandler(OnGraphChangedHandle);
	}
}

bool FBlueprintToolEditorToolkit::IsTickableInEditor() const
{
	return true;
}

void FBlueprintToolEditorToolkit::Tick(float DeltaTime)
{
	if (bGraphChanged)
	{
		bGraphChanged = false;
		GraphChanged();
	}
}

bool FBlueprintToolEditorToolkit::IsTickable() const
{
	return true;
}

TStatId FBlueprintToolEditorToolkit::GetStatId() const
{
	return TStatId();
}

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

	InTabManager->RegisterTabSpawner(FBPToolID::PreviewSettingsID, FOnSpawnTab::CreateSP(this, &FBlueprintToolEditorToolkit::SpawnByPreviewSettingsTab))
		.SetDisplayName(LOCTEXT("PreviewSettingsTabLabel", "BP_Preview Settings"))
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FBlueprintToolEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	Super::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(FBPToolID::PreviewID);
	InTabManager->UnregisterTabSpawner(FBPToolID::ContentBrowserID);
	InTabManager->UnregisterTabSpawner(FBPToolID::BlueprintGraphID);

	FBTCommands::Unregister();
}

void FBlueprintToolEditorToolkit::Initialize(
	UBlueprintData* InTextAsset, 
	const EToolkitMode::Type InMode, 
	const TSharedPtr<IToolkitHost>& InToolkitHost)
{
	//调用virtual void RegisterCommands() override;
	FBTCommands::Register();/** Use this method to register commands. Usually done in StartupModule(). */

	BindCommands();
	ExtendToolbar();
	//创建蓝图
	{
		if (!InTextAsset->EdGraph)
		{
			InTextAsset->EdGraph = NewObject<UBPArchitectEdGraph>(InTextAsset, UBPArchitectEdGraph::StaticClass(), NAME_None, RF_Transactional);
		}

		UBPArchitectEdGraph* EDGraph = Cast<UBPArchitectEdGraph>(InTextAsset->EdGraph);
		EDGraph->InitializeGraph();
		GraphEditor = CreateBPGraphEditor(EDGraph);
	}

	/*蓝图编辑器管理一个class UEdGraph对象，这个相当于Model
	其他的基于Graph的编辑器可能使用class UEdGraph的派生类，例如Material Editor：class UMaterialGraph : public UEdGraph
	它使用class UEdGraphSchema_K2来定义蓝图Graph的行为，相当于Controller
	这些行为包括：测试Pin之间是否可以连接、创建或删除连接等等
	它是class UEdGraphSchema的派生类
	详见：Source / Editor / BlueprintGraph / Classes / EdGraphSchema_K2.h
	整体的UI、Node布局等，都是一个复用的SGraphEditor，相当于View
	Graph中的每个Node对应一个可扩展的Widget，可以从class SGraphNode派生之后添加的SGraphEditor中。对于蓝图来说，它们都是：class SGraphNodeK2Base的派生类*/
	
	StaticMeshComponent = NewObject<UStaticMeshComponent>();
	PreviewViewport = SNew(SBlueprintPreviewViewport,StaticMeshComponent)
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
						->AddTab(FBPToolID::PreviewSettingsID, ETabState::OpenedTab)
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

	/**
	 * Initializes this asset editor.  Called immediately after construction.  If you override this, remember to
	 * call the base class implementation
	 *
	 * @param	Mode					Asset editing mode for this editor (standalone or world-centric)
	 * @param	InitToolkitHost			When Mode is WorldCentric, this is the level editor instance to spawn this editor within
	 * @param	AppIdentifier			When Mode is Standalone, this is the app identifier of the app that should host this toolkit
	 * @param	StandaloneDefaultLayout	The default layout for a standalone asset editor
	 * @param	bCreateDefaultToolbar	The default toolbar, which can be extended
	 * @param	bCreateDefaultStandaloneMenu	True if in standalone mode, the asset editor should automatically generate a default "asset" menu, or false if you're going to do this yourself in your derived asset editor's implementation
	 * @param	ObjectToEdit			The object to edit
	 * @param	bInIsToolbarFocusable	Whether the buttons on the default toolbar can receive keyboard focus
	 * @param	bUseSmallToolbarIcons	Whether the buttons on the default toolbar use the small icons
	 */
	//void FAssetEditorToolkit::InitAssetEditor(
	//	const EToolkitMode::Type Mode, 
	//	const TSharedPtr< class IToolkitHost >& InitToolkitHost, 
	//	const FName AppIdentifier, 
	//	const TSharedRef<FTabManager::FLayout>& StandaloneDefaultLayout, 
	//	const bool bCreateDefaultStandaloneMenu, 
	//	const bool bCreateDefaultToolbar, 
	//	UObject* ObjectToEdit, 
	//	const bool bInIsToolbarFocusable, 
	//	const bool bInUseSmallToolbarIcons)

	InitAssetEditor(
		InMode,
		InToolkitHost,
		FBPToolID::ViewportID,
		StandloneCustomLayout,
		true,
		true,
		InTextAsset
	);

	OnGraphChangedHandle = GraphEditor->GetCurrentGraph()->AddOnGraphChangedHandler(
		FOnGraphChanged::FDelegate::CreateRaw(this, &FBlueprintToolEditorToolkit::OnGraphChanged));

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

void FBlueprintToolEditorToolkit::BindCommands()
{
	const FBTCommands& Commands = FBTCommands::Get();

	ToolkitCommands->MapAction(
		Commands.Compile, 
		FExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::Compile));

	ToolkitCommands->MapAction(
		Commands.Help,
		FExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::Help));

	ToolkitCommands->MapAction(
		Commands.Run,
		FExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::Run));
}

void FBlueprintToolEditorToolkit::ExtendToolbar()
{
	struct Local
	{
		static void FillToolbar(FToolBarBuilder& ToolbarBuilder)
		{
			ToolbarBuilder.BeginSection("Compile");
			{
				ToolbarBuilder.AddToolBarButton(FBTCommands::Get().Compile);
			}
			ToolbarBuilder.EndSection();

			ToolbarBuilder.BeginSection("Help");
			{
				ToolbarBuilder.AddToolBarButton(FBTCommands::Get().Help);
			}
			ToolbarBuilder.EndSection();

			ToolbarBuilder.BeginSection("Run");
			{
				ToolbarBuilder.AddToolBarButton(FBTCommands::Get().Run);
			}
			ToolbarBuilder.EndSection();
		}
	};

	TSharedPtr<FExtender> ToolbarExtender(new FExtender);
	ToolbarExtender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateStatic(&Local::FillToolbar));

	AddToolbarExtender(ToolbarExtender);
}

void FBlueprintToolEditorToolkit::Compile()
{
	UBlueprintData* InBlueprintData = GetBlueprintData();
	if (InBlueprintData)
	{
		FBTCompilationUtilities::FlushCompilationQueueImpl(InBlueprintData);
	}
}

void FBlueprintToolEditorToolkit::Help()
{

}

void FBlueprintToolEditorToolkit::Run()
{
#if WITH_EDITOR
	Compile();
#endif
	
	USimpleCodeEvent::BlueprintGameBegins(StaticMeshComponent);
}

void FBlueprintToolEditorToolkit::SaveAsset_Execute()
{
	UBlueprintData* InBlueprintData = GetBlueprintData();
	if (InBlueprintData)
	{
		FBTCompilationUtilities::ClearByte(InBlueprintData);

		FAssetEditorToolkit::SaveAsset_Execute();

		FBTCompilationUtilities::FlushCompilationQueueImpl(InBlueprintData);
	}
	else
	{
		FAssetEditorToolkit::SaveAsset_Execute();
	}
}

UBlueprintData* FBlueprintToolEditorToolkit::GetBlueprintData() const
{
	return GetEditingObjects().Num() == 1 ? Cast<UBlueprintData>(GetEditingObjects()[0]) : NULL;
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
		.Label(LOCTEXT("BPContentBrowserKey", "Content Browser"))
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
	AssetDropTarget = SNew(SObjectEditorDropTarget)//处理从content拖入对象
		.OnAssetDropped(this, &FBlueprintToolEditorToolkit::AssetDropped)
		.OnIsAssetAcceptableForDrop(this, &FBlueprintToolEditorToolkit::IsAssetAcceptableForDrop)
		.Visibility(EVisibility::HitTestInvisible);

	return SNew(SDockTab)
		.Label(LOCTEXT("BPGraph", "BP Graph"))
		.TabColorScale(GetTabColorScale())
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				GraphEditor.ToSharedRef()
			]

			+ SOverlay::Slot()
			[
				AssetDropTarget.ToSharedRef()
			]	
		];
}

TSharedRef<SDockTab> FBlueprintToolEditorToolkit::SpawnByPaletteTab(const FSpawnTabArgs& Args)
{
	ActionPalette = SNew(SBlueprintNodeListPalette, SharedThis(this));

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("Kismet.Tabs.Palette"))
		.Label(LOCTEXT("BToolPlaetteTitle", "Palette"))
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
		.Label(LOCTEXT("BTDetailsTab", "Details"))
		[
			PropertyEditorRef
		];
}

TSharedRef<SDockTab> FBlueprintToolEditorToolkit::SpawnByPreviewSettingsTab(const FSpawnTabArgs& Args)
{
	TSharedPtr<FAdvancedPreviewScene> PreviewScene;
	TSharedPtr<SWidget> SettingsView = SNullWidget::NullWidget;;

	if (PreviewViewport.IsValid())
	{
		PreviewScene = PreviewViewport->GetPreviewScene();
		TSharedPtr<SAdvancedPreviewDetailsTab> PreviewSettingsView = SNew(SAdvancedPreviewDetailsTab, PreviewScene.ToSharedRef());

		PreviewSettingsView->Refresh();

		SettingsView = PreviewSettingsView;
	}

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("Kismet.Tabs.Palette"))
		.Label(LOCTEXT("PreviewSettingsTitle", "Preview Settings"))
		[
			SNew(SBox)
			.AddMetaData<FTagMetaData>(FTagMetaData(TEXT("BPTool Preview Settings")))
			[
				SettingsView.ToSharedRef()
			]
		];
}

TSharedRef<SGraphEditor> FBlueprintToolEditorToolkit::CreateBPGraphEditor(UEdGraph* InGraph)
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("MyAppearanceCornerText", "BP Editor");//右下角的字

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
				.Text(LOCTEXT("MyText", "Smartuil's Blueprint"))
				.TextStyle(FEditorStyle::Get(), TEXT("GraphBreadcrumbButtonText"))
			]
		];

	//绑定对蓝图粘贴删除等操作
	GraphEditorCommands = MakeShareable(new FUICommandList);
	{
		GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
			FExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::DuplicateNodes),
			FCanExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::CanDuplicateNodes)
		);

		GraphEditorCommands->MapAction(FGenericCommands::Get().SelectAll,
			FExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::SelectAllNodes),
			FCanExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::CanSelectAllNodes)
		);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
			FExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::DeleteSelectedNodes),
			FCanExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::CanDeleteNode)
		);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
			FExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::CopySelectedNodes),
			FCanExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::CanCopyNodes)
		);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
			FExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::PasteNodes),
			FCanExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::CanPasteNodes)
		);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
			FExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::CutSelectedNodes),
			FCanExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::CanCutNodes)
		);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Undo,
			FExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::UndoGraphAction)
		);

		GraphEditorCommands->MapAction(FGenericCommands::Get().Redo,
			FExecuteAction::CreateSP(this, &FBlueprintToolEditorToolkit::RedoGraphAction)
		);
	}

	//处理点击不同node
	SGraphEditor::FGraphEditorEvents InGraphEvents;
	InGraphEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FBlueprintToolEditorToolkit::OnSelectedBPNodesChanged);

	TSharedRef<SGraphEditor> GraphEditorInstance = SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.Appearance(AppearanceInfo)
		.TitleBar(TitleBarWidget)
		.GraphToEdit(InGraph)
		.GraphEvents(InGraphEvents);

	return GraphEditorInstance;
}

void FBlueprintToolEditorToolkit::OnSelectedBPNodesChanged(const TSet<class UObject*>& SelectionNode)
{
	if (SelectionNode.Num() > 0)
	{
		PropertyEditor->SetObjects(SelectionNode.Array());
	}
	else//修复点击空白处，details面板没有清空的bug
	{
		TArray<UObject*> InObjects;
		PropertyEditor->SetObjects(InObjects);
	}
}

void FBlueprintToolEditorToolkit::AssetDropped(UObject* AssetObject)
{
	if (GraphEditor.IsValid())
	{
		UBPArchitectEdGraph* ThemeGraph = Cast<UBPArchitectEdGraph>(GraphEditor->GetCurrentGraph());
		if (ThemeGraph)
		{
			FVector2D GridLocation = GetAssetDropGridLocation();
			ThemeGraph->CreateNewNode(AssetObject, GridLocation);
		}
	}
}

bool FBlueprintToolEditorToolkit::IsAssetAcceptableForDrop(const UObject* AssetObject) const
{
	if (!AssetObject)
	{
		return false;
	}

	UClass* AssetClass = AssetObject->GetClass();

	bool bValidClass = AssetClass->IsChildOf<UStaticMesh>()
		|| AssetClass->IsChildOf<UClass>()
		|| AssetClass->IsChildOf<AActor>()
		|| AssetClass->IsChildOf<UBlueprint>();

	return bValidClass;
}

FVector2D FBlueprintToolEditorToolkit::GetAssetDropGridLocation() const
{
	if (!AssetDropTarget.IsValid())
	{
		return FVector2D::ZeroVector;
	}

	FVector2D PanelCoord = AssetDropTarget->GetPanelCoordDropPosition();
	FVector2D ViewLocation = FVector2D::ZeroVector;
	float ZoomAmount = 1.0f;

	GraphEditor->GetViewLocation(ViewLocation, ZoomAmount);
	FVector2D GridLocation = PanelCoord / ZoomAmount + ViewLocation;

	return GridLocation;
}

void FBlueprintToolEditorToolkit::OnGraphChanged(const FEdGraphEditAction& Action)
{
	bGraphChanged = true;
}

void FBlueprintToolEditorToolkit::GraphChanged()
{
	if (ActionPalette.IsValid())
	{
		ActionPalette->UpdateNodeListPalette();
	}
}

void FBlueprintToolEditorToolkit::DeleteSelectedNodes()
{
	TArray<UEdGraphNode*> NodesToDelete;
	const FGraphPanelSelectionSet SelectedNodes = GraphEditor->GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		NodesToDelete.Add(CastChecked<UEdGraphNode>(*NodeIt));
	}

	if (NodesToDelete.Num() > 0)
	{
		for (int32 i = 0; i < NodesToDelete.Num(); ++i)
		{
			NodesToDelete[i]->BreakAllNodeLinks();

			const UEdGraphSchema* Schema = nullptr;
			if (UEdGraph* MyGraphObj = NodesToDelete[i]->GetGraph())
			{
				MyGraphObj->Modify();
				Schema = MyGraphObj->GetSchema();
			}

			NodesToDelete[i]->Modify();

			if (Schema)
			{
				Schema->BreakNodeLinks(*NodesToDelete[i]);
			}

			NodesToDelete[i]->DestroyNode();
		}
	}
}

bool FBlueprintToolEditorToolkit::CanDeleteNode() const
{
	return true;
}

void FBlueprintToolEditorToolkit::CopySelectedNodes()
{
	const FGraphPanelSelectionSet SelectedNodes = GraphEditor->GetSelectedNodes();
	FString ExportedText;
	FEdGraphUtilities::ExportNodesToText(SelectedNodes, /*out*/ ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

bool FBlueprintToolEditorToolkit::CanCopyNodes() const
{
	return true;
}

void FBlueprintToolEditorToolkit::PasteNodes()
{
	const FScopedTransaction Transaction(NSLOCTEXT("BPToolNodes", "BPToolNodesPaste", "Hello Node We are paste!!"));
	GraphEditor->ClearSelectionSet();

	GraphEditor->GetCurrentGraph()->Modify();

	FString TextToImport;
	FPlatformApplicationMisc::ClipboardPaste(TextToImport);

	TSet<UEdGraphNode*> PastedNodes;
	FEdGraphUtilities::ImportNodesFromText(GraphEditor->GetCurrentGraph(), TextToImport, /*out*/ PastedNodes);

	FVector2D AvgNodePosition(0.0f, 0.0f);

	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UEdGraphNode* Node = *It;
		AvgNodePosition.X += Node->NodePosX;
		AvgNodePosition.Y += Node->NodePosY;
	}

	if (PastedNodes.Num() > 0)
	{
		float InvNumNodes = 1.0f / float(PastedNodes.Num());
		AvgNodePosition.X *= InvNumNodes;
		AvgNodePosition.Y *= InvNumNodes;
	}

	const FVector2D Location = GraphEditor->GetPasteLocation();
	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UEdGraphNode* Node = *It;

		GraphEditor->SetNodeSelection(Node, true);

		Node->NodePosX = (Node->NodePosX - AvgNodePosition.X) + Location.X;
		Node->NodePosY = (Node->NodePosY - AvgNodePosition.Y) + Location.Y;

		Node->SnapToGrid(SNodePanel::GetSnapGridSize());

		Node->CreateNewGuid();
//		Node->PostPlacedNewNode();
//		Node->AllocateDefaultPins();

		//创建新的GraphPinID 为后面的虚拟机做判断
		for (UEdGraphPin *Pin : Node->Pins)
		{
			Pin->PinId = FGuid::NewGuid();
		}
	}

	GraphEditor->NotifyGraphChanged();
}

bool FBlueprintToolEditorToolkit::CanPasteNodes() const
{
	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

	return FEdGraphUtilities::CanImportNodesFromText(GraphEditor->GetCurrentGraph(), ClipboardContent);
}

void FBlueprintToolEditorToolkit::CutSelectedNodes()
{
	CopySelectedNodes();

	const FGraphPanelSelectionSet OldSelectedNodes = GraphEditor->GetSelectedNodes();

	FGraphPanelSelectionSet RemainingNodes;
	GraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if ((Node != NULL) && Node->CanDuplicateNode())
		{
			GraphEditor->SetNodeSelection(Node, true);
		}
		else
		{
			RemainingNodes.Add(Node);
		}
	}

	DeleteSelectedNodes();

	GraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(RemainingNodes); SelectedIter; ++SelectedIter)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter))
		{
			GraphEditor->SetNodeSelection(Node, true);
		}
	}
}

bool FBlueprintToolEditorToolkit::CanCutNodes() const
{
	return CanCopyNodes() && CanDeleteNode();
}

void FBlueprintToolEditorToolkit::DuplicateNodes()
{
	CopySelectedNodes();
	PasteNodes();
}

bool FBlueprintToolEditorToolkit::CanDuplicateNodes() const
{
	return CanCopyNodes();
}

void FBlueprintToolEditorToolkit::SelectAllNodes()
{
	GraphEditor->SelectAllNodes();
}

bool FBlueprintToolEditorToolkit::CanSelectAllNodes() const
{
	return GraphEditor.IsValid();
}

void FBlueprintToolEditorToolkit::UndoGraphAction()
{
	GEditor->UndoTransaction();

	GraphEditor->NotifyGraphChanged();
}

void FBlueprintToolEditorToolkit::RedoGraphAction()
{
	GEditor->RedoTransaction(); /*GEditor->NoteSelectionChange();*/
}

#undef LOCTEXT_NAMESPACE