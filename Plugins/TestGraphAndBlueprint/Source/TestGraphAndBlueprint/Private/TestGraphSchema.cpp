#include "TestGraphSchema.h"
#include "TestNode_HelloWorld.h"
#include "ScopedTransaction.h"
#include "MyConnectionDrawingPolicy.h"
#include "K2Node_Event.h"

#define LOCTEXT_NAMESPACE "TestGraphSchema"

void UTestGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	//FTestGraphSchemaAction(FText InNodeCategory, FText InMenuDesc, FText InToolTip, const int32 InGrouping, FText InKeywords = FText(), int32 InSectionID = 0)
	TSharedPtr<FTestGraphSchemaAction> NewNodeAction(new
		FTestGraphSchemaAction(
			LOCTEXT("A", "Grap Node"),
			LOCTEXT("Desc", "MyTestAA"),
			LOCTEXT("NewGraphText", "Add a Node"),
			0));

	NewNodeAction->NodeHelloWorld = NewObject<UTestNode_HelloWorld>(ContextMenuBuilder.OwnerOfTemporaries);//参数，储存临时模板节点
	ContextMenuBuilder.AddAction(NewNodeAction);

	//NewNodeAction->K2Node_Event = NewObject<UK2Node_Event>(ContextMenuBuilder.OwnerOfTemporaries);
	//ContextMenuBuilder.AddAction(NewNodeAction);
}

void UTestGraphSchema::GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{
	//接口变了，没有找到FMenuBuilder参数，暂时跳过
}

UEdGraphNode* FTestGraphSchemaAction::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode /*= true*/)
{
	UEdGraphNode *UEdResultNode = nullptr;

	//if (NodeHelloWorld != nullptr)
	//{
	//	const FScopedTransaction Transaction(LOCTEXT("FF", "Hell:NewNode"));

	//	ParentGraph->Modify();

	//	if (FromPin != nullptr)
	//	{
	//		FromPin->Modify();
	//	}

	//	K2Node_Event->Rename(nullptr, ParentGraph);
	//	ParentGraph->AddNode(K2Node_Event, true, bSelectNewNode);

	//	K2Node_Event->CreateNewGuid();
	//	K2Node_Event->PostPlacedNewNode();
	//	//	K2Node_Event->AllocateDefaultPins();
	//	//	K2Node_Event->AutowireNewNode(FromPin);

	//	K2Node_Event->NodePosX = Location.X;
	//	K2Node_Event->NodePosY = Location.Y;

	//	K2Node_Event->SetFlags(RF_Transactional);

	//	UEdResultNode = NodeHelloWorld;
	//}

	if (NodeHelloWorld != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("FF", "Hell:NewNode"));

		ParentGraph->Modify();

		if (FromPin != nullptr)
		{
			FromPin->Modify();
		}

		NodeHelloWorld->Rename(nullptr, ParentGraph);
		ParentGraph->AddNode(NodeHelloWorld, true, bSelectNewNode);

		NodeHelloWorld->CreateNewGuid();
		NodeHelloWorld->PostPlacedNewNode();
		NodeHelloWorld->AllocateDefaultPins();
		NodeHelloWorld->AutowireNewNode(FromPin);

		NodeHelloWorld->NodePosX = Location.X;
		NodeHelloWorld->NodePosY = Location.Y;

		NodeHelloWorld->SetFlags(RF_Transactional);//Object is transactional.非序列化，与GC回收有关

		UEdResultNode = NodeHelloWorld;
	}
	return UEdResultNode;
}

//FConnectionDrawingPolicy* UTestGraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
//{
//	return new FMyConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements, InGraphObj);
//}

#undef LOCTEXT_NAMESPACE