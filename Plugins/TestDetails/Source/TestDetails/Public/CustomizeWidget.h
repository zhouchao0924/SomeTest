#pragma once
#include "Widgets/SCompoundWidget.h"


class TESTDETAILS_API SCustomizeWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCustomizeWidget) {}

	SLATE_ATTRIBUTE(class UCustomizeObject*, CustomizeObject)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	TSharedPtr<class IDetailsView> ConfigPanel;

	TAttribute<class UCustomizeObject*> CustomizeObject;
};