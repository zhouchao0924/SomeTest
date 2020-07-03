#pragma once

#include "Factories/Factory.h"
#include "CustomFactory.generated.h"

UCLASS()
class UCustomFactory : public UFactory
{
	GENERATED_BODY()

public:
	UCustomFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext);
};