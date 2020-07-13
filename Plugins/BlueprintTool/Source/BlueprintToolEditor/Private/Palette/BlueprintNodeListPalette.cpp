#include "Palette/BlueprintNodeListPalette.h"
#include "EditorStyleSet.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/SOverlay.h"

void SBlueprintNodeListPalette::Construct(const FArguments& InArgs, TWeakPtr<class FBlueprintToolEditorToolkit> InBlueprintToolEditor)
{
	this->BlueprintToolEditor = InBlueprintToolEditor;

	this->ChildSlot
		[
			SNew(SBorder)
			.Padding(2.0f)
			.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
			[
				SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				[
					SNew(SOverlay)

					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SAssignNew(GraphActionMenu, SGraphActionMenu)
						.OnActionDragged(this, &SBlueprintNodeListPalette::OnActionDragged)
						.OnCreateWidgetForAction(this, &SBlueprintNodeListPalette::OnCreateWidgetForAction)
						.OnCollectAllActions(this, &SBlueprintNodeListPalette::CollectAllActions)
					]
				]
			]
		];
}

TSharedRef<SWidget> SBlueprintNodeListPalette::OnCreateWidgetForAction(FCreateWidgetForActionData* const InCreateData)
{
	return SNew(SImage);
}

FReply SBlueprintNodeListPalette::OnActionDragged(const TArray< TSharedPtr<FEdGraphSchemaAction> >& InActions, const FPointerEvent& MouseEvent)
{
	return Super::OnActionDragged(InActions, MouseEvent);
}

void SBlueprintNodeListPalette::CollectAllActions(FGraphActionListBuilderBase& OutAllActions)
{

}

