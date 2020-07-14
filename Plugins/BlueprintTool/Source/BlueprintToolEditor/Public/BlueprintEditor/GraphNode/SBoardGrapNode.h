#pragma once

#include "SGraphNode.h"
#include "BlueprintEditor/Core/Architect/K3Node.h"

class SK3Node : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SK3Node)
	{}
	SLATE_ATTRIBUTE(FText, NodeName)
	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UK3Node* MarkerNode);

	virtual void UpdateGraphNode() override;

	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
protected:
	TSharedPtr<class SBox> PinBox;
	TAttribute<FText> NodeName;
};