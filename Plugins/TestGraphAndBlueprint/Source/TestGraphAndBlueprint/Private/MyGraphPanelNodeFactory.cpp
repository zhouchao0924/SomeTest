#include "MyGraphPanelNodeFactory.h"
#include "TestNode_HelloWorld.h"
#include "MyConnectionDrawingPolicy.h"
#include "TestGraphSchema.h"
#include "MyGraphNode.h"

FMyGraphPanelNodeFactory::FMyGraphPanelNodeFactory()
{

}

TSharedPtr<class SGraphNode> FMyGraphPanelNodeFactory::CreateNode(class UEdGraphNode* Node) const
{
	if (UTestNode_HelloWorld* MarkerNode = Cast<UTestNode_HelloWorld>(Node))
	{
		return SNew(SMyGraphNode_HelloWorld, MarkerNode);
	}

	return NULL;
}

TSharedPtr<class SGraphPin> FMyGraphPanelPinFactory::CreatePin(class UEdGraphPin* Pin) const
{
	return NULL;
}

FConnectionDrawingPolicy* FMyGraphPanelPinConnectionFactory::CreateConnectionPolicy(const class UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const class FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
{
	if (Schema->IsA(UTestGraphSchema::StaticClass()))
	{
		return new FMyConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements, InGraphObj);
	}

	return nullptr;
}
