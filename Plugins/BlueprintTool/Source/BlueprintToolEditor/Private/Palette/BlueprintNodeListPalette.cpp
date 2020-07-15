#include "Palette/BlueprintNodeListPalette.h"
#include "EditorStyleSet.h"
#include "AssetEditor/Architect/AssetArchitectToolkit.h"
#include "GraphEditor.h"
#include "BlueprintEditor/Core/Architect/PBToolSchema.h"

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
	return Super::OnCreateWidgetForAction(InCreateData);
}

FReply SBlueprintNodeListPalette::OnActionDragged(const TArray< TSharedPtr<FEdGraphSchemaAction> >& InActions, const FPointerEvent& MouseEvent)
{
	return Super::OnActionDragged(InActions, MouseEvent);
}

void SBlueprintNodeListPalette::CollectAllActions(FGraphActionListBuilderBase& OutAllActions)
{
	const UPBToolSchema* Schema = GetDefault<UPBToolSchema>();

	TArray<TSharedPtr<FEdGraphSchemaAction> > Actions;
	FGraphActionMenuBuilder ActionMenuBuilder;

	if (BlueprintToolEditor.IsValid())
	{
		TSharedPtr<SGraphEditor> BPGraphEditor = BlueprintToolEditor.Pin()->GetGraphEditor();
		if (BPGraphEditor.IsValid())
		{
			UEdGraph* Graph = BPGraphEditor->GetCurrentGraph();
			Schema->GetActionList(Graph,Actions);

			for (TSharedPtr<FEdGraphSchemaAction> Action : Actions)
			{
				ActionMenuBuilder.AddAction(Action);
			}
		}
	}

	OutAllActions.Append(ActionMenuBuilder);
}

void SBlueprintNodeListPalette::UpdateNodeListPalette()
{
	RefreshActionsList(true);
}

