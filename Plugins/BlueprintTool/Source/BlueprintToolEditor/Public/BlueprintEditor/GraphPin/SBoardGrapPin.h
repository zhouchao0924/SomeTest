//$ Copyright 2015 Ali Akbar, Code Respawn Technologies Pvt Ltd - All Rights Reserved $//
#pragma once
#include "SGraphPin.h"

class BLUEPRINTTOOLEDITOR_API SBPToolGraphPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SBPToolGraphPin) {}
	SLATE_ATTRIBUTE(FText, PinName)
	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

	TSharedPtr< SGraphNode> GetGraphNode() { return OwnerNodePtr.Pin(); }

private:
	TAttribute<FText> PinName;
};