#pragma once
#include "CoreMinimal.h"
#include "BTFunctionContext.h"

class FVM
{
public:
	FVM(FBTFunctionContext *NewFunctionList)
		:FunctionList(NewFunctionList)
	{
	}
	void GenerateBytecode();
private:
	FBTFunctionContext *FunctionList;
};