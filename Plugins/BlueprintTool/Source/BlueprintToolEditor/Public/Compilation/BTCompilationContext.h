#pragma once

#include "CoreMinimal.h"
#include "BTFunctionContext.h"

class FBTCompilerContext
{
public:
	TIndirectArray<FBTFunctionContext> FunctionList;

	void CompileFunction(FBTFunctionContext& Context);
};