//$ Copyright 2015 Ali Akbar, Code Respawn Technologies Pvt Ltd - All Rights Reserved $//
#pragma once

#include "SGraphNode.h"
#include "TestNode_HelloWorld.h"

class SMyGraphNode_HelloWorld : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SMyGraphNode_HelloWorld)
	{}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UTestNode_HelloWorld* MarkerNode);

	virtual void UpdateGraphNode() override;
	virtual void CreatePinWidgets() override;

	//virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
protected:
	TSharedPtr<class SBox> PinBox;
};