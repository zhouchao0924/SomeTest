#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"

class BLUEPRINTTOOLEDITOR_API FBToolPanelNodeFactory : public FGraphPanelNodeFactory
{
public:

	virtual TSharedPtr<class SGraphNode> CreateNode(class UEdGraphNode* Node) const;
};

class BLUEPRINTTOOLEDITOR_API FBToolPanelPinFactory : public FGraphPanelPinFactory
{
public:
	virtual TSharedPtr<class SGraphPin> CreatePin(class UEdGraphPin* Pin) const;
};

struct BLUEPRINTTOOLEDITOR_API FBToolPanelPinConnectionFactory : public FGraphPanelPinConnectionFactory
{
public:
	virtual class FConnectionDrawingPolicy* CreateConnectionPolicy(const class UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const class FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const override;
};