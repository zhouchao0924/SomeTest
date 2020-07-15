#pragma once
#include "CoreMinimal.h"
#include "BTBlueprintCompiledStatement.h"
#include "BTTerminal.h"

class UEdGraphNode;
class FBTFunctionContext
{
public:
	TArray<UEdGraphNode*> LinearExecutionList;

	TMap<UEdGraphNode*, TArray<EVMStatementType>> ListVMStatement;

	TMap<UFunction*, TArray< FFunctionTerminal>> TerminalMap;

	TMap<UEdGraphNode*, TArray< FFunctionTerminal>> TerminalMapNode;

	FFunctionTerminal FindTerm(UFunction* FindFunc, UProperty *Pin);

	FFunctionTerminal FindTerm(UEdGraphNode* FindFunc, UProperty *Pin);
};