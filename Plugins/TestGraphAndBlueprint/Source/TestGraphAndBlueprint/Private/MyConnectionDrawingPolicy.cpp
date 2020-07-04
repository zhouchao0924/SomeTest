#include "MyConnectionDrawingPolicy.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

static const FLinearColor DefaultWiringColor(1.0f, 0.0f, 0.0f);

FMyConnectionDrawingPolicy::FMyConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj)
	: FConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements)
	, EdGraphObj(InGraphObj)
{

}

void FMyConnectionDrawingPolicy::DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin, /*inout*/ FConnectionParams& Params)
{
	Params.WireThickness = 5.5f;
	Params.WireColor = DefaultWiringColor;

	if (HoveredPins.Num() > 0)
	{
		ApplyHoverDeemphasis(OutputPin, InputPin, Params.WireThickness, Params.WireColor);
	}
}

void FMyConnectionDrawingPolicy::DrawConnection(int32 LayerId, const FVector2D& Start, const FVector2D& End, const FConnectionParams& Params)
{
	const FVector2D Delta = End - Start;
	const FVector2D DirDelta = Delta.GetSafeNormal();

	float WireThickness = 1.0;
	FLinearColor WireColor = FLinearColor::Blue;

	FSlateDrawElement::MakeDrawSpaceSpline(
		DrawElementsList,
		LayerId,
		Start, DirDelta,
		End, DirDelta,
		//ClippingRect,
		WireThickness,
		ESlateDrawEffect::None,
		WireColor
	);

	if (1)
	{
		FInterpCurve<float> SplineReparamTable;
		float SplineLength = MakeSplineReparamTable(Start, DirDelta, End, DirDelta, SplineReparamTable);

		const float BubbleSpacing = 64.f * ZoomFactor;
		const float BubbleSpeed = 192.f * ZoomFactor;
		const FVector2D BubbleSize = BubbleImage->ImageSize * ZoomFactor * 0.1f * Params.WireThickness;

		float DeltaTime = (FPlatformTime::Seconds() - GStartTime);
		const float BubbleOffset = FMath::Fmod(DeltaTime * BubbleSpeed, BubbleSpacing);
		const int32 NumBubbles = FMath::CeilToInt(SplineLength / BubbleSpacing);

		for (int32 i = 0; i < NumBubbles; ++i)
		{
			const float Distance = ((float)i * BubbleSpacing) + BubbleOffset;
			if (Distance < SplineLength)
			{
				const float Alpha = SplineReparamTable.Eval(Distance, 0.f);
				FVector2D BubblePos = FMath::CubicInterp(Start, DirDelta, End, DirDelta, Alpha);
				BubblePos -= (BubbleSize * 0.5f);

				FSlateDrawElement::MakeBox(
					DrawElementsList,
					LayerId,
					FPaintGeometry(BubblePos, BubbleSize, ZoomFactor),
					BubbleImage,
					//ClippingRect,
					ESlateDrawEffect::None,
					WireColor
				);
			}
		}
	}
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif