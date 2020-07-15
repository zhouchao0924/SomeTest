#include "BlueprintEditor/GraphNode/SEventNode.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"

void SEventNode::Construct(const FArguments& InArgs, UK3Node* MarkerNode)
{
	NodeName = InArgs._NodeName;
	GraphNode = MarkerNode;

	this->SetCursor(EMouseCursor::GrabHand);

	this->UpdateGraphNode();
}

void SEventNode::UpdateGraphNode()
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
						. HAlign(HAlign_Right)
						. VAlign(VAlign_Fill)
						. FillWidth(1.0f)
						[
							SAssignNew(RightNodeBox, SVerticalBox)
						]
					]
				
				]
			]
		];

	//PinBox->SetWidthOverride(200.f);
	//PinBox->SetHeightOverride(300.f);

	CreatePinWidgets();
}

