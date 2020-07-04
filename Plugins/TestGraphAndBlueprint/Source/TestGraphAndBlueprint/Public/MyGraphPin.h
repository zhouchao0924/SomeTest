//$ Copyright 2015 Ali Akbar, Code Respawn Technologies Pvt Ltd - All Rights Reserved $//
#pragma once
#include "SGraphPin.h"

class TESTGRAPHANDBLUEPRINT_API SMyGraphPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SMyGraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

	TSharedPtr< SGraphNode> GetGraphNode() { return OwnerNodePtr.Pin(); }
protected:
};