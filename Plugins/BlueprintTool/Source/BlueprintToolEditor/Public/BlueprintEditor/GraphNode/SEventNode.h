#pragma once

#include "SGraphNode.h"
#include "BlueprintEditor/Core/Architect/K3Node.h"

class SEventNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SEventNode)
	{}
	SLATE_ATTRIBUTE(FText, NodeName)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UK3Node* MarkerNode);

	virtual void UpdateGraphNode() override;
protected:
	TSharedPtr<class SBox> PinBox;
	TAttribute<FText> NodeName;
};