#pragma once

#include "EdGraph/EdGraph.h"
#include "BPArchitectEdGraph.generated.h"


UCLASS()
class UBPArchitectEdGraph : public UEdGraph
{
	//GENERATED_UCLASS_BODY()：属于之前版本的宏，自动生成带有指定参数的构造函数，不用用户再去生明构造函数，需要在CPP中实现，否则报错，之后成员是public。

	//GENERATED_BODY()：属于新版本的宏，不生成构造函数，如果需要自定义，需要自己声明并定义，之后成员是private。

	//总结：使用后者，不要使用前者。

	GENERATED_UCLASS_BODY()
public:

	void InitializeGraph();

	UEdGraphNode* CreateNewNode(UObject* AssetObject, const FVector2D& Location);

	template<typename TNodeClass>
	TNodeClass* CreateNewNode(const FVector2D& Location)
	{
		TNodeClass* Node = NewObject<TNodeClass>(this);
		Node->Rename(NULL, this, REN_NonTransactional);
		this->AddNode(Node, true, false);

		Node->CreateNewGuid();
		Node->PostPlacedNewNode();
		Node->AllocateDefaultPins();

		Node->NodePosX = Location.X;
		Node->NodePosY = Location.Y;

		Node->SnapToGrid(16);

		return Node;
	}
};

