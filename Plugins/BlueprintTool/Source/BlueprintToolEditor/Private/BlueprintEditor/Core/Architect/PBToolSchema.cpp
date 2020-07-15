#include "BlueprintEditor/Core/Architect/PBToolSchema.h"
#include "BlueprintEditor/Core/Architect/PBToolSchemaAction.h"
#include "BlueprintEditor/GraphNode/VariableType.h"
#include "Utils/BlueSchemaUtils.h"
#include "SimpleCode.h"

#define LOCTEXT_NAMESPACE "PBToolSchema"

void UPBToolSchema::InitVariable(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	FBPTSchemaUtils::CreateAction<UObjectNode>("Object", "Variable", LOCTEXT("NewObject", "a object variable"), ContextMenuBuilder, nullptr);
	FBPTSchemaUtils::CreateAction<UBoolNode>("Bool", "Variable", LOCTEXT("NewBool", "a Bool variable"), ContextMenuBuilder, nullptr);
	FBPTSchemaUtils::CreateAction<UByteNode>("Byte", "Variable", LOCTEXT("NewByte", "a Byte variable"), ContextMenuBuilder, nullptr);
	FBPTSchemaUtils::CreateAction<UFloatNode>("Float", "Variable", LOCTEXT("NewFloat", "a Float variable"), ContextMenuBuilder, nullptr);
	FBPTSchemaUtils::CreateAction<UIntNode>("Int", "Variable", LOCTEXT("NewInt", "a Int variable"), ContextMenuBuilder, nullptr);
	FBPTSchemaUtils::CreateAction<UNameNode>("Name", "Variable", LOCTEXT("NewName", "a Name variable"), ContextMenuBuilder, nullptr);
	FBPTSchemaUtils::CreateAction<UStringNode>("String", "Variable", LOCTEXT("NewString", "a String variable"), ContextMenuBuilder, nullptr);
	FBPTSchemaUtils::CreateAction<UStructRotationNode>("Rotation", "Variable", LOCTEXT("NewRotation", "a Rotation variable"), ContextMenuBuilder, nullptr);
	FBPTSchemaUtils::CreateAction<UStructVectorNode>("Vector", "Variable", LOCTEXT("NewVector", "a Vector variable"), ContextMenuBuilder, nullptr);
	FBPTSchemaUtils::CreateAction<UTextNode>("Text", "Variable", LOCTEXT("NewText", "a Text variable"), ContextMenuBuilder, nullptr);
}

void UPBToolSchema::InitVariable(UEdGraph* OwnerBPGraph, TArray<TSharedPtr<FEdGraphSchemaAction> >& OutActions) const
{
	FBPTSchemaUtils::CreateAction<UObjectNode>("Object", "variable", LOCTEXT("NewObject", "a object variable"), OwnerBPGraph, OutActions, nullptr);
	FBPTSchemaUtils::CreateAction<UBoolNode>("Bool", "Variable", LOCTEXT("NewBool", "a Bool variable"), OwnerBPGraph, OutActions, nullptr);
	FBPTSchemaUtils::CreateAction<UByteNode>("Byte", "Variable", LOCTEXT("NewByte", "a Byte variable"), OwnerBPGraph, OutActions, nullptr);
	FBPTSchemaUtils::CreateAction<UFloatNode>("Float", "Variable", LOCTEXT("NewFloat", "a Float variable"), OwnerBPGraph, OutActions, nullptr);
	FBPTSchemaUtils::CreateAction<UIntNode>("Int", "Variable", LOCTEXT("NewInt", "a Int variable"), OwnerBPGraph, OutActions, nullptr);
	FBPTSchemaUtils::CreateAction<UNameNode>("Name", "Variable", LOCTEXT("NewName", "a Name variable"), OwnerBPGraph, OutActions, nullptr);
	FBPTSchemaUtils::CreateAction<UStringNode>("String", "Variable", LOCTEXT("NewString", "a String variable"), OwnerBPGraph, OutActions, nullptr);
	FBPTSchemaUtils::CreateAction<UStructRotationNode>("Rotation", "Variable", LOCTEXT("NewRotation", "a Rotation variable"), OwnerBPGraph, OutActions, nullptr);
	FBPTSchemaUtils::CreateAction<UStructVectorNode>("Vector", "Variable", LOCTEXT("NewVector", "a Vector variable"), OwnerBPGraph, OutActions, nullptr);
	FBPTSchemaUtils::CreateAction<UTextNode>("Text", "Variable", LOCTEXT("NewText", "a Text variable"), OwnerBPGraph, OutActions, nullptr);
}

UPBToolSchema::UPBToolSchema(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{}

void UPBToolSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	TArray<UClass*> CodeClassArray;
	GetDerivedClasses(USimpleCode::StaticClass(), CodeClassArray, false);

	for (UClass * TmpCode : CodeClassArray)
	{
		for (TFieldIterator<UFunction> i(TmpCode); i; ++i)
		{
			UFunction* Func = *i;
			if (Func->GetMetaData("CodeType") == "Describe" || Func->GetMetaData("CodeType") == "PureDescribe")
			{		
				FBPTSchemaUtils::CreateAction<UFunctionGrapNode>(Func->GetName(), Func->GetMetaData("Group"), Func->GetToolTipText(), ContextMenuBuilder, Func);
			}
			else if (Func->GetMetaData("CodeType") == "Event")
			{
				FBPTSchemaUtils::CreateAction<UEventNode>(Func->GetName(), Func->GetMetaData("Group"), Func->GetToolTipText(), ContextMenuBuilder, Func);
			}
		}

		for (TFieldIterator<UProperty> i(TmpCode); i; ++i)
		{
			UProperty* Prop = *i;
			if (Prop)
			{
				FBPTSchemaUtils::CreateAction<UFunctionGrapNode>(Prop->GetName(), Prop->GetMetaData("Group"), Prop->GetToolTipText(), ContextMenuBuilder, Prop);
			}
		}
	}

	//先初始化变量
	InitVariable(ContextMenuBuilder);

	// Run through all nodes and add any menu items they want to add
	Super::GetGraphContextActions(ContextMenuBuilder);
}


void UPBToolSchema::GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{

}

//void UPBToolSchema::GetContextMenuActions(const UEdGraph* CurrentGraph, const UEdGraphNode* InGraphNode, const UEdGraphPin* InGraphPin, class FMenuBuilder* MenuBuilder, bool bIsDebugging) const
//{
//	MenuBuilder->BeginSection(TEXT("Hall"));
//	{
//		MenuBuilder->AddWidget(
//			SNew(SImage),
//			LOCTEXT("IamgeTask", "just Iamge Task")
//		);
//
//		MenuBuilder->AddEditableText(
//			LOCTEXT("OK_A", "just a Hello"),
//			LOCTEXT("OK_B", "just a task Hello"),
//			FSlateIcon(),
//			LOCTEXT("TTT", "just a TTTTTT")
//		);
//	}
//	MenuBuilder->EndSection();
//}


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
	TArray<UClass*> CodeClassArray;
	GetDerivedClasses(USimpleCode::StaticClass(), CodeClassArray, false);

	for (UClass * TmpCode : CodeClassArray)
	{
		for (TFieldIterator<UFunction> i(TmpCode); i; ++i)
		{
			UFunction* Func = *i;
			if (Func->GetMetaData("CodeType") == "Describe" || Func->GetMetaData("CodeType") == "PureDescribe")
			{
				FBPTSchemaUtils::CreateAction<UFunctionGrapNode>(Func->GetName(), Func->GetMetaData("Group"), Func->GetToolTipText(), OwnerBPGraph, OutActions, Func);
			}
			else if (Func->GetMetaData("CodeType") == "Event")
			{
				FBPTSchemaUtils::CreateAction<UEventNode>(Func->GetName(), Func->GetMetaData("Group"), Func->GetToolTipText(), OwnerBPGraph, OutActions, Func);
			}
		}

		for (TFieldIterator<UProperty> i(TmpCode); i; ++i)
		{
			UProperty* Prop = *i;
			if (Prop)
			{
				FBPTSchemaUtils::CreateAction<UFunctionGrapNode>(Prop->GetName(), Prop->GetMetaData("Group"), Prop->GetToolTipText(), OwnerBPGraph, OutActions, Prop);
			}
		}
	}

	//初始化变量
	InitVariable(OwnerBPGraph, OutActions);
}

#undef LOCTEXT_NAMESPACE