#include "Factory/Blueprint/BlueprintToolFactory.h"
#include "BlueprintEditor/GraphNode/BoardGrapNode.h"
#include "BlueprintEditor/GraphNode/SBoardGrapNode.h"
#include "BlueprintEditor/Core/Architect/VariableNode.h"
#include "BlueprintEditor/GraphNode/ObjectNode.h"
#include "BlueprintEditor/GraphNode/EventNode.h"
#include "BlueprintEditor/Core/Architect/PBToolSchema.h"
#include "NodeFactory.h"
#include "BlueprintEditor/GraphPin/SBoardGrapPin.h"
#include "BlueprintEditor/ConnectionDrawingPolicy/ConnectionDrawingPolicy1.h"
#include "BlueprintEditor/GraphNode/SObjectNode.h"
#include "BlueprintEditor/GraphNode/SVariableNode.h"
#include "BlueprintEditor/GraphNode/SEventNode.h"

TSharedPtr<class SGraphNode> FBToolPanelNodeFactory::CreateNode(class UEdGraphNode* Node) const
{
	if (UFunctionGrapNode* MarkerNode = Cast<UFunctionGrapNode>(Node))
	{
		return SNew(SK3Node, MarkerNode)
			.NodeName(MarkerNode->GetNodeTitle());
	}
	else if (UVariableNode* MarkerNode2 = Cast<UVariableNode>(Node))
	{
		if (UObjectNode* ObjectNode = Cast<UObjectNode>(Node))
		{
			return SNew(SObjectNode, MarkerNode2);
		}

		return SNew(SVariableNode, MarkerNode2);
	}
	else if (UEventNode* MarkerNode3 = Cast<UEventNode>(Node))
	{
		return SNew(SEventNode, MarkerNode3)
			.NodeName(MarkerNode3->GetNodeTitle());
	}

	return NULL;
}

TSharedPtr<class SGraphPin> FBToolPanelPinFactory::CreatePin(class UEdGraphPin* Pin) const
{
	TSharedPtr<class SGraphPin> NewPin;
	if (const UPBToolSchema* K2Schema = Cast<const UPBToolSchema>(Pin->GetSchema()))
	{
		NewPin = FNodeFactory::CreateK2PinWidget(Pin);//使用ue4pin样式

		if (!NewPin.IsValid())
		{
			NewPin = SNew(SBPToolGraphPin, Pin)
				.PinName(FText::FromName(Pin->PinName));
		}
	}

	return NewPin;
}

class FConnectionDrawingPolicy* FBToolPanelPinConnectionFactory::CreateConnectionPolicy(const class UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const class FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
{
	if (Schema->IsA(UPBToolSchema::StaticClass()))
	{
		return new FBPToolConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements, InGraphObj);
	}

	return nullptr;
}
