#include "BTCompilationContext.h"
#include "BlueprintEditor/Core/Architect/K3Node.h"
#include "Type/BTEType.h"
#include "BTCompilerVMBackend.h"
#include "BlueprintEditor/Core/Architect/VariableNode.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

void GetCurrentParmTerminal(TArray<FFunctionTerminal> &FunTerList, UEdGraphPin *Pin)
{
	if (Pin->PinType.PinCategory != FPC_Public::PC_Exec)
	{
		FFunctionTerminal FunctionTerminal;
		FunctionTerminal.Direction = Pin->Direction;

		if (Pin->Direction == EEdGraphPinDirection::EGPD_Input)
		{
			if (!Pin->LinkedTo.Num())
			{
				if (Pin->PinType.PinCategory == FPC_Public::PC_Text)
				{
					FunctionTerminal.DefaultTextValue = Pin->DefaultTextValue;
				}
				else
				{
					FunctionTerminal.DefaultValue = Pin->DefaultValue;
				}
			}
			else
			{			
				FunctionTerminal.RHS = Pin->LinkedTo[0];
				UK3Node *K3 = Cast<UK3Node>(FunctionTerminal.RHS->GetOwningNode());
				if (K3)
				{
					if (K3->IsPure())
					{
						TArray<FFunctionTerminal> TerminalArray;
						for (UEdGraphPin *MyPin : K3->Pins)
						{
							GetCurrentParmTerminal(TerminalArray, MyPin);
						}

						FunTerList.Append(TerminalArray);
					}
					else if(K3->IsA(UVariableNode::StaticClass()))
					{
						FunctionTerminal.DefaultValue = K3->Pins[0]->DefaultValue;
						FunctionTerminal.IsBPVariable = true;
					}
					else
					{
						//该连接不包含 变量 和 纯节点
						FunctionTerminal.IsLink = true;
					}
				}
			}
		}
		else //Output
		{
			if (Pin->LinkedTo.Num())
			{
				FunctionTerminal.IsLink = true;
				FunctionTerminal.LHS = Pin->LinkedTo;
			}
		}

		FunctionTerminal.PinId = Pin->PinId;
		FunctionTerminal.Name = Pin->PinName;
		FunctionTerminal.Type = Pin->PinType.PinCategory;

		FunTerList.Add(FunctionTerminal);
	}
}

void FBTCompilerContext::CompileFunction(FBTFunctionContext& Context)
{
	UEdGraphNode *EndNode = nullptr;
	for (int32 i = 0; i < Context.LinearExecutionList.Num(); i++)
	{
		UK3Node *K3 = Cast<UK3Node>(Context.LinearExecutionList[i]);
		if (K3->Function)
		{
			//编译语句
			TArray<EVMStatementType> ListStatement;
			if (i == 0)
			{
				ListStatement.Add(EVMStatementType::VMS_CallFunc);
			}

			ListStatement.Add(EVMStatementType::VMS_Nest);

			Context.ListVMStatement.Add(K3, ListStatement);
			EndNode = K3;

			//收集端子
			TArray<FFunctionTerminal> TerminalArray;
			for (UEdGraphPin *Pin : K3->Pins)
			{
				GetCurrentParmTerminal(TerminalArray, Pin);
			}

			if (TerminalArray.Num() > 0)
			{
				Context.TerminalMap.Add(K3->Function, TerminalArray);//参考
				Context.TerminalMapNode.Add(K3, TerminalArray);//收集端子 最可靠
			}
		}
	}
	Context.ListVMStatement[EndNode].Add(EVMStatementType::VMS_Return);

	FVM VM(&Context);
	VM.GenerateBytecode();
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif