#include "Factory/Blueprint/BlueprintToolFactory.h"
#include "BlueprintEditor/GraphNode/BoardGrapNode.h"
#include "BlueprintEditor/GraphPin/SBoardGrapPin.h"
#include "BlueprintEditor/Core/Architect/PBToolSchema.h"
#include "BlueprintEditor/ConnectionDrawingPolicy/ConnectionDrawingPolicy1.h"
#include "BlueprintEditor/GraphNode/SBoardGrapNode.h"


TSharedPtr<class SGraphNode> FBToolPanelNodeFactory::CreateNode(class UEdGraphNode* Node) const
{
	//if (UFunctionGrapNode* MarkerNode = Cast<UFunctionGrapNode>(Node))
	//{
	//	return SNew(SK3Node, MarkerNode)
	//		.NodeName(MarkerNode->GetNodeTitle());
	//}
	//else if (UVariableNode* MarkerNode = Cast<UVariableNode>(Node))
	//{
	//	if (UObjectNode* ObjectNode = Cast<UObjectNode>(Node))
	//	{
	//		return SNew(SObjectNode, MarkerNode);
	//	}

	//	return SNew(SVariableNode, MarkerNode);
	//}
	//else if (UEventNode* MarkerNode = Cast<UEventNode>(Node))
	//{
	//	return SNew(SEventNode, MarkerNode)
	//		.NodeName(MarkerNode->GetNodeTitle());
	//}
	if (UBoardGrapNode* MarkerNode = Cast<UBoardGrapNode>(Node))
	{
		return SNew(SK3Node, MarkerNode);
	}

	return NULL;
}

TSharedPtr<class SGraphPin> FBToolPanelPinFactory::CreatePin(class UEdGraphPin* Pin) const
{
	//TSharedPtr<class SGraphPin> NewPin;
	//if (const UPBToolSchema* K2Schema = Cast<const UPBToolSchema>(Pin->GetSchema()))
	//{
	//	NewPin = FNodeFactory::CreateK2PinWidget(Pin);

	//	if (!NewPin.IsValid())
	//	{
	//		NewPin = SNew(SBPToolGraphPin, Pin)
	//			.PinName(FText::FromName(Pin->PinName));
	//	}
	//}
	if (UBoardGrapNode* MarkerNode = Cast<UBoardGrapNode>(Pin->GetOuter()))
	{
		return SNew(SBPToolGraphPin, Pin);
	}
	return nullptr;
	//return NewPin;
}

FConnectionDrawingPolicy* FBToolPanelPinConnectionFactory::CreateConnectionPolicy(const class UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const class FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
{
	if (Schema->IsA(UPBToolSchema::StaticClass()))
	{
		return new FBPToolConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements, InGraphObj);
	}
	return nullptr;
}
