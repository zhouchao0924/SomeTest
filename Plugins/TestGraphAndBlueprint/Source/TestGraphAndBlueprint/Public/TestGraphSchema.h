// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "TestGraphSchema.generated.h"

USTRUCT()
struct FTestGraphSchemaAction :public FEdGraphSchemaAction//提供右键菜单选项
{
	GENERATED_BODY()

public:

	FTestGraphSchemaAction()
	{}

	FTestGraphSchemaAction(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping, FText InKeywords = FText(), int32 InSectionID = 0)
		:FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping, InKeywords, InSectionID)
	{}

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true);

	class UTestNode_HelloWorld *NodeHelloWorld;
	class UK2Node_Event *K2Node_Event;
};

/**
 * 
 */
UCLASS(MinimalAPI)
class UTestGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()
public:
	/**
	 * Get all actions that can be performed when right clicking on a graph or drag-releasing on a graph from a pin
	 *
	 * @param [in,out]	ContextMenuBuilder	The context (graph, dragged pin, etc...) and output menu builder.
	 */
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	
	/**
	 * Gets actions that should be added to the right-click context menu for a node or pin
	 *
	 * @param	Menu				The menu to append actions to.
	 * @param	Context				The menu's context.
	 */
	virtual void GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const;

	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, TEXT("Not implemented by this schema"));
	}

	virtual class FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const;
};
