
#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class FMyTestDetailCustomization : public IDetailCustomization
{
public:

	//映射到对象面板上
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)override;

	//实例化
	static TSharedRef< IDetailCustomization > MakeInstance();
};