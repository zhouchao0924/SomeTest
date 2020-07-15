#pragma once

#include "SGraphNode.h"
#include "BlueprintEditor/Core/Architect/K3Node.h"

class SVariableNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SVariableNode)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UK3Node* MarkerNode);

	virtual void UpdateGraphNode() override;
};