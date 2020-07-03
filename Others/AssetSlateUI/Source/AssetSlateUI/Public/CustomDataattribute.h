#pragma once
#include "Widgets/SCompoundWidget.h"
#include "IDetailsView.h"

class SCustomDataAttribute : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SCustomDataAttribute) {}

	SLATE_ATTRIBUTE(class UCustomAsset*, CustomAsset)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	class UCustomAsset* CustomAsset;

	TSharedPtr<IDetailsView> ConfigPanel;
};