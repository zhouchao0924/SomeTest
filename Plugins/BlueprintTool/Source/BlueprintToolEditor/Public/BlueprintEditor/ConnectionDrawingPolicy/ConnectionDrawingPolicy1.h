#pragma once
#include "ConnectionDrawingPolicy.h"

class FBPToolConnectionDrawingPolicy : public FConnectionDrawingPolicy
{
public:
	FBPToolConnectionDrawingPolicy(int32 InBackLayerID,
		int32 InFrontLayerID,
		float ZoomFactor,
		const FSlateRect& InClippingRect,
		FSlateWindowElementList& InDrawElements,
		UEdGraph* InGraphObj
	);

	virtual void DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin, /*inout*/ FConnectionParams& Params) override;
};