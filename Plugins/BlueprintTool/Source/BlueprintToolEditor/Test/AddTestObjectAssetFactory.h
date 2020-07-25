#pragma once

#include "Factories/Factory.h"
#include "AddTestObjectAssetFactory.generated.h"

UCLASS()
class UAddTestObjectAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UAddTestObjectAssetFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext);
};
