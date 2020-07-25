#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "PBToolSchemaAction.generated.h"


USTRUCT()
struct FPBToolSchemaAction :public FEdGraphSchemaAction
{
	GENERATED_BODY()

public:

	FPBToolSchemaAction()
	{}

	FPBToolSchemaAction(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping, FText InKeywords = FText(), int32 InSectionID = 0)
		:FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping, InKeywords, InSectionID)
	{}

	/** Execute this action, given the graph and schema, 
	and possibly a pin that we were dragged from. Returns a node that was created by this action (if any). */
	virtual UEdGraphNode* PerformAction(
		class UEdGraph* ParentGraph, 
		UEdGraphPin* FromPin, 
		const FVector2D Location, 
		bool bSelectNewNode = true);

	class UK3Node *K3Node;
};

