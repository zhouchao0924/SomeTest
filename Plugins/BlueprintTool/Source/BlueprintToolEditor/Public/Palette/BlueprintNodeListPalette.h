#pragma once
#include "SGraphPalette.h"

class BLUEPRINTTOOLEDITOR_API SBlueprintNodeListPalette : public SGraphPalette
{
	typedef SGraphPalette Super;
public:
	SLATE_BEGIN_ARGS(SBlueprintNodeListPalette) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TWeakPtr<class FBlueprintToolEditorToolkit> InBlueprintToolEditor);

	virtual TSharedRef<SWidget> OnCreateWidgetForAction(FCreateWidgetForActionData* const InCreateData);

	virtual FReply OnActionDragged(const TArray< TSharedPtr<FEdGraphSchemaAction> >& InActions, const FPointerEvent& MouseEvent);

	virtual void CollectAllActions(FGraphActionListBuilderBase& OutAllActions);
protected:
	TWeakPtr<FBlueprintToolEditorToolkit> BlueprintToolEditor;
};