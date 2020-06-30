#include "SScrrenLoading.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Notifications/SProgressBar.h"

void SScreenLoading::Construct(const FArguments &InArgs, const FString &InMapname)
{
	MapName = InMapname;
	ChildSlot
		[
			SNew(SProgressBar)
			.Percent(this, &SScreenLoading::GetProgress)
		];
}

TOptional<float> SScreenLoading::GetProgress() const
{
	//-1 -----------  -1
	float LoadPrecnetage = GetAsyncLoadPercentage(*MapName);

	return LoadPrecnetage * 0.01f;
}

