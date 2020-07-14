#pragma once
#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"

struct BLUEPRINTTOOLEDITOR_API FBPTSchemaUtils
{
	template<typename T>
	static void CreateAction(FString Title, const FString &NewCategory, const FText &Tooltip, UEdGraph* BPOwner, TArray<TSharedPtr<FEdGraphSchemaAction> >& OutActions, UField *Struct)
	{
		const FText MenuDesc = FText::FromString(Title);
		const FText Category = FText::FromString(NewCategory == "" ? "Default" : NewCategory);
		const FText TooltipText = Tooltip;

		TSharedPtr<FPBToolSchemaAction> NewAction = TSharedPtr<FPBToolSchemaAction>(new FPBToolSchemaAction(Category, MenuDesc, TooltipText, 0));
		NewAction->K3Node = NewObject<T>(BPOwner);

		if (Cast<UFunction>(Struct))
		{
			NewAction->K3Node->Function = Cast<UFunction>(Struct);
		}
		else if (Cast<UProperty>(Struct))
		{
			NewAction->K3Node->Property = Cast<UProperty>(Struct);
		}

		OutActions.Add(NewAction);
	}

	template<typename T>
	static void CreateAction(const FString &Title, const FString &NewCategory, const FText &Tooltip, FGraphContextMenuBuilder &ContextMenuBuilder, UField *Struct)
	{
		const FText MenuDesc = FText::FromString(Title);;
		const FText Category = FText::FromString(NewCategory == "" ? "Default" : NewCategory);
		const FText TooltipText = Tooltip;

		TSharedPtr<FPBToolSchemaAction> NewAction = TSharedPtr<FPBToolSchemaAction>(new FPBToolSchemaAction(Category, MenuDesc, TooltipText, 0));
		NewAction->K3Node = NewObject<T>(ContextMenuBuilder.OwnerOfTemporaries);
		if (Cast<UFunction>(Struct))
		{
			NewAction->K3Node->Function = Cast<UFunction>(Struct);
		}
		else if (Cast<UFunction>(Struct))
		{
			NewAction->K3Node->Property = Cast<UProperty>(Struct);
		}

		ContextMenuBuilder.AddAction(NewAction);
	}
};