#include "BlueprintEditor/GraphNode/SBoardGrapNode.h"
#include "SGraphPin.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/SBoxPanel.h"
#include "SGraphNode.h"

void SK3Node::Construct(const FArguments& InArgs, UK3Node* MarkerNode)
{
	NodeName = InArgs._NodeName;
	GraphNode = MarkerNode;

	this->SetCursor(EMouseCursor::GrabHand);

	this->UpdateGraphNode();
}

void SK3Node::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	const FSlateBrush* MyNodeIcon = FEditorStyle::GetBrush(TEXT("Graph.StateNode.Icon"));

	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SAssignNew(PinBox, SBox)
			.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SBorder)
			.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.BorderBackgroundColor_Lambda([&]()
	{
		FSlateColor SlateColor(FLinearColor(1.f, 1.f, 1.f));
		return SlateColor;
	})
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.Padding(0.f, 10.f, 0.f, 0.f)
		.HAlign(HAlign_Center)
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(NodeName)
		]

	+ SVerticalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.AutoWidth()
		[
			SAssignNew(LeftNodeBox, SVerticalBox)
		]

	+ SHorizontalBox::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Fill)
		.FillWidth(1.0f)
		[
			SAssignNew(RightNodeBox, SVerticalBox)
		]
		]

		]
		]
		];

	PinBox->SetWidthOverride(200.f);
	PinBox->SetHeightOverride(300.f);

	CreatePinWidgets();
}

void SK3Node::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();

	if (PinObj && PinObj->bAdvancedView)
	{
		PinToAdd->SetVisibility(TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced));
	}

	//	PinToAdd->SetDesiredSizeScale(FVector2D(16.f, 16.f));

	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	{
		LeftNodeBox->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(20.0f, 0.0f)
			[
				PinToAdd
			];

		InputPins.Add(PinToAdd);
	}
	else if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Output)
	{
		RightNodeBox->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			.Padding(20.0f, 0.0f)
			[
				PinToAdd
			];

		OutputPins.Add(PinToAdd);
	}
}

