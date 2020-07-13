#pragma once

#include "Factories/Factory.h"
#include "AssetFactory.generated.h"

UCLASS()
class UAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UAssetFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext);
};
