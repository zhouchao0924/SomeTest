#pragma once
#include "CoreMinimal.h"

struct FFunctionTerminal
{
	FName Name;

	FName Type;
	FString DefaultValue;
	UObject* DefaultObject;
	FText DefaultTextValue;

	//该端子的连入类型
	TEnumAsByte<enum EEdGraphPinDirection> Direction;

	//作为赋值
	UEdGraphPin *RHS;
	TArray<UEdGraphPin*> LHS;

	FGuid PinId;

	//是否有连接 这个连接不包括纯节点的连接 和 变量的连接
	bool IsLink;
	bool IsBPVariable;
	FFunctionTerminal()
		:DefaultObject(nullptr)	
		,RHS(nullptr)
		,IsLink(false)
		, IsBPVariable(false)
	{}
};