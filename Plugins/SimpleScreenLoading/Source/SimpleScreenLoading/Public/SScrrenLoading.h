#pragma once
#include "Widgets/SCompoundWidget.h"

class SScreenLoading :public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SScreenLoading)
	{}
	SLATE_END_ARGS();

	void Construct(const FArguments &InArgs,const FString &InMapname);

private:

	TOptional<float> GetProgress() const;

	FString MapName;
};