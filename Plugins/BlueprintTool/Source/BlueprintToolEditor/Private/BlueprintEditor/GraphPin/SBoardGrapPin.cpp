#include "BlueprintEditor/GraphPin/SBoardGrapPin.h"

void SBPToolGraphPin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	PinName = InArgs._PinName;

	this->SetCursor(EMouseCursor::Hand);

	IsEditable = true;

	GraphPinObj = InPin;
	check(GraphPinObj != NULL);

	const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
	check(Schema);

	SBorder::Construct(SBorder::FArguments()
		.BorderBackgroundColor_Lambda([&]()
	{
		FSlateColor SlateColor(FLinearColor(1.f, 1.f, 1.f));
		return SlateColor;
	})
		.OnMouseButtonDown(this, &SGraphPin::OnPinNameMouseDown)
		[
			SNew(STextBlock)
			.Text(PinName)
		]
	);
}

