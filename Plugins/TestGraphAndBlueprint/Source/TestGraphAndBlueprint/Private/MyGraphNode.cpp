// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGraphNode.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBorder.h"
#include "TestGraphAndBlueprintStyle.h"
#include "SGraphPinNameList.h"
#include "NodeFactory.h"
#include "Widgets/Layout/SBox.h"
#include "EdGraph/EdGraphPin.h"
#include "KismetPins/SGraphPinBool.h"
#include "KismetPins/SGraphPinColor.h"
#include "MyGraphPin.h"
#include "KismetPins/SGraphPinExec.h"
#include "KismetPins/SGraphPinObject.h"
#include "../Private/KismetPins/SGraphPinEnum.h"
#include "SGraphPinNameList.h"
#include "NodeFactory.h"
#include "KismetPins/SGraphPinString.h"
#include "KismetPins/SGraphPinVector2D.h"
#include "KismetPins/SGraphPinVector4.h"
#include "KismetPins/SGraphPinInteger.h"

void SMyGraphNode_HelloWorld::Construct(const FArguments& InArgs, UTestNode_HelloWorld* MarkerNode)
{
	GraphNode = MarkerNode;

	this->SetCursor(EMouseCursor::GrabHand);

	this->UpdateGraphNode();
}

void SMyGraphNode_HelloWorld::UpdateGraphNode()
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
				//.BorderImage_Lambda([&]()
				//{
				//	const FVector2D  ICOSize(64.f, 64.f);
				//	return FTestGraphAndBlueprintStyle::GetImageBurush(TEXT("ButtonIcon_40x"), ICOSize);
				//})
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
		];

	PinBox->SetWidthOverride(300.f);
	PinBox->SetHeightOverride(720.f);

	CreatePinWidgets();
}

#define RESET_PIN(SMyGraphPinType,GraphPinObject,...)\
	NewPin = SNew(SMyGraphPinType, GraphPinObject, __VA_ARGS__);\
	NewPin->SetIsEditable(IsEditable); \
	this->AddPin(NewPin.ToSharedRef());

#define VERSION_SET 0
void SMyGraphNode_HelloWorld::CreatePinWidgets()
{
	UTestNode_HelloWorld* HelloNode = CastChecked<UTestNode_HelloWorld>(GraphNode);

	if (HelloNode)
	{
#if VERSION_SET
		for (UEdGraphPin * CurPin : HelloNode->Pins)
		{
			TSharedPtr<SGraphPin> NewPin = /*FNodeFactory::CreatePinWidget(CurPin)*/SNew(SMyGraphPin, CurPin);
			NewPin->SetIsEditable(IsEditable);

			this->AddPin(NewPin.ToSharedRef());

		}

#else
		static TArray<TSharedPtr<FName>> InNameList;
		InNameList.Add(MakeShareable(new  FName(TEXT("Monster"))));
		InNameList.Add(MakeShareable(new  FName(TEXT("Hello"))));
		InNameList.Add(MakeShareable(new  FName(TEXT("Wood"))));
		InNameList.Add(MakeShareable(new  FName(TEXT("Good"))));

		TSharedPtr<SGraphPin> NewPin = nullptr;
		RESET_PIN(SGraphPinBool, HelloNode->Pins[0]);
		RESET_PIN(SGraphPinColor, HelloNode->Pins[1]);
		RESET_PIN(SGraphPinEnum, HelloNode->Pins[2]);
		RESET_PIN(SGraphPinExec, HelloNode->Pins[3]);
		RESET_PIN(SGraphPinNameList, HelloNode->Pins[4], InNameList);
		RESET_PIN(SGraphPinObject, HelloNode->Pins[5]);
		RESET_PIN(SGraphPinString, HelloNode->Pins[6]);
		RESET_PIN(SGraphPinVector2D, HelloNode->Pins[7]);
		RESET_PIN(SGraphPinVector4, HelloNode->Pins[8]);
		RESET_PIN(SGraphPinInteger, HelloNode->Pins[9]);

		RESET_PIN(SMyGraphPin, HelloNode->Pins[10]);
		RESET_PIN(SMyGraphPin, HelloNode->Pins[11]);
		RESET_PIN(SMyGraphPin, HelloNode->Pins[12]);
		RESET_PIN(SMyGraphPin, HelloNode->Pins[13]);
		RESET_PIN(SMyGraphPin, HelloNode->Pins[14]);
#endif

	}
}

void SMyGraphNode_HelloWorld::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();

	if (PinObj && PinObj->bAdvancedView)
	{
		PinToAdd->SetVisibility(TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced));
	}
#if VERSION_SET 
	PinToAdd->SetDesiredSizeScale(FVector2D(16.f, 16.f));
#endif
	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	{
		LeftNodeBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillHeight(1.0f)
			.Padding(20.0f, 0.0f)
			[
				PinToAdd
			];

		InputPins.Add(PinToAdd);
	}
	else if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Output)
	{
		RightNodeBox->AddSlot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Fill)
			.FillHeight(1.0f)
			.Padding(20.0f, 0.0f)
			[
				PinToAdd
			];

		OutputPins.Add(PinToAdd);
	}
}
