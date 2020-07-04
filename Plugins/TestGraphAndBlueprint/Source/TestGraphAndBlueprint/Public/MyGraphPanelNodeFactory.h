#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"

class TESTGRAPHANDBLUEPRINT_API FMyGraphPanelNodeFactory : public FGraphPanelNodeFactory
{
public:
	FMyGraphPanelNodeFactory();
	virtual TSharedPtr<class SGraphNode> CreateNode(class UEdGraphNode* Node) const;
};

class TESTGRAPHANDBLUEPRINT_API FMyGraphPanelPinFactory : public FGraphPanelPinFactory
{
public:
	virtual TSharedPtr<class SGraphPin> CreatePin(class UEdGraphPin* Pin) const;
};

struct TESTGRAPHANDBLUEPRINT_API FMyGraphPanelPinConnectionFactory : public FGraphPanelPinConnectionFactory
{
public:
	virtual class FConnectionDrawingPolicy* CreateConnectionPolicy(const class UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const class FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const override;
};