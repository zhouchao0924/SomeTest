#include "BlueprintEditor/Core/Architect/PBToolSchema.h"
#include "BlueprintEditor/Core/Architect/PBToolSchemaAction.h"
#include "Utils/BlueSchemaUtils.h"
#include "BlueprintEditor/GraphNode/BoardGrapNode.h"

#define LOCTEXT_NAMESPACE "PBToolSchema"

void UPBToolSchema::InitVariable(FGraphContextMenuBuilder& ContextMenuBuilder) const
{

}

void UPBToolSchema::InitVariable(UEdGraph* OwnerBPGraph, TArray<TSharedPtr<FEdGraphSchemaAction> >& OutActions) const
{

}

UPBToolSchema::UPBToolSchema(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{}

void UPBToolSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TSharedPtr<FPBToolSchemaAction> NewNodeAction(new
		FPBToolSchemaAction(
			LOCTEXT("A", "Grap Node"),
			LOCTEXT("Desc", "MyTestAA"),
			LOCTEXT("NewGraphText", "Add a node"),
			0
			)
	);

	NewNodeAction->K3Node = NewObject<UBoardGrapNode>(ContextMenuBuilder.OwnerOfTemporaries);
	ContextMenuBuilder.AddAction(NewNodeAction);
}

//void UPBToolSchema::GetContextMenuActions(const UEdGraph* CurrentGraph, const UEdGraphNode* InGraphNode, const UEdGraphPin* InGraphPin, class FMenuBuilder* MenuBuilder, bool bIsDebugging) const
//{
//
//}
void UPBToolSchema::GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{

}

bool UPBToolSchema::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
	bool ConnectionNade = true;
	if (ConnectionNade)
	{
		for (const auto *TmpB : A->GetOwningNodeUnchecked()->GetAllPins())
		{
			if (TmpB == B)
			{
				ConnectionNade = false;
				break;
			}
		}
	}

	if (ConnectionNade)
	{
		UEdGraphSchema::TryCreateConnection(A, B);
	}

	return ConnectionNade;
}

void UPBToolSchema::GetActionList(UEdGraph* OwnerBPGraph, TArray<TSharedPtr<FEdGraphSchemaAction> >& OutActions) const
{
	const FText MenuDesc = FText::FromString(TEXT("MenuDesc"));
	const FText Category = FText::FromString(TEXT("Category"));
	const FText TooltipText = FText::FromString(TEXT("TooltipText"));

	TSharedPtr<FPBToolSchemaAction> NewAction = TSharedPtr<FPBToolSchemaAction>(new FPBToolSchemaAction(Category, MenuDesc, TooltipText, 0));
	NewAction->K3Node = NewObject<UBoardGrapNode>(OwnerBPGraph);
	OutActions.Add(NewAction);
}

#undef LOCTEXT_NAMESPACE