#include "BlueprintEditor/GraphNode/SObjectNode.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "BlueprintEditor/GraphNode/ObjectNode.h"
#include "AssetThumbnail.h"
#include "BlueprintEditor/GraphNode/CAssetThumbnailPool.h"


void SObjectNode::Construct(const FArguments& InArgs, UK3Node* MarkerNode)
{
	ThumbnailSize = FIntPoint(512, 512);
	GraphNode = MarkerNode;
	UObjectNode *K3Node = Cast<UObjectNode>(GraphNode);
	if (K3Node)
	{
		K3Node->Update.BindSP(this, &SObjectNode::UpdateObjectThumbnailWidget);
	}

	this->SetCursor(EMouseCursor::GrabHand);

	BuildThumbnail();

	this->UpdateGraphNode();
}

void SObjectNode::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	RightNodeBox = SNew(SVerticalBox);

	const FSlateBrush* MyNodeIcon = FEditorStyle::GetBrush(TEXT("Graph.StateNode.Icon"));

	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				RightNodeBox.ToSharedRef()
			]

			+ SVerticalBox::Slot()
			.Padding(10.f)
			[
				SNew(SBox)
				.HeightOverride(64)
				.WidthOverride(64)
				[
					AssetThumbnail->MakeThumbnailWidget()
				]
			]		
		];

	CreatePinWidgets();
}

void SObjectNode::SetThumbnail(UObject* AssetObject)
{
	AssetThumbnail->SetAsset(FAssetData(AssetObject));
}

void SObjectNode::BuildThumbnail()
{
	FObjectAssetData Value;
	GetValue(Value);
	AssetThumbnail = MakeShareable(new FAssetThumbnail(Value.AssetData, ThumbnailSize.X, ThumbnailSize.Y, FObjectEditorThumbnailPool::Get()));
}

void SObjectNode::UpdateObjectThumbnailWidget()
{
	UObjectNode *K3Node = Cast<UObjectNode>(GraphNode);
	SetThumbnail(K3Node->Value);
}

bool SObjectNode::GetValue(FObjectAssetData& OutValue) const
{
	if (GIsSavingPackage || IsGarbageCollecting())
	{
		return false;
	}

	UObjectNode *K3Node = Cast<UObjectNode>(GraphNode);
	if (K3Node)
	{
		OutValue = FObjectAssetData(K3Node->Value);

		return K3Node->Value != nullptr;
	}
	else
	{
		return false;
	}
}
