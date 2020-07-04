#pragma once
#include "ConnectionDrawingPolicy.h"

class FMyConnectionDrawingPolicy : public FConnectionDrawingPolicy
{
public:
	FMyConnectionDrawingPolicy(int32 InBackLayerID,
		int32 InFrontLayerID,
		float ZoomFactor,
		const FSlateRect& InClippingRect,
		FSlateWindowElementList& InDrawElements,
		UEdGraph* InGraphObj
	);

	virtual void DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin, /*inout*/ FConnectionParams& Params) override;

	virtual void DrawConnection(int32 LayerId, const FVector2D& Start, const FVector2D& End, const FConnectionParams& Params) override;
protected:
	UEdGraph* EdGraphObj;

	TMap<UEdGraphNode*, int32> EdNodeWidgetMap;
};