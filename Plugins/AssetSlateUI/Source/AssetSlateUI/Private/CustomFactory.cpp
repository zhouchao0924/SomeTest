#include "CustomFactory.h"
#include "CustomAsset.h"

UCustomFactory::UCustomFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UCustomAsset::StaticClass();
}

UObject* UCustomFactory::FactoryCreateNew(
	UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	UObject* Context, 
	FFeedbackContext* Warn,
	FName CallingContext)
{
	return NewObject<UCustomAsset>(InParent, InClass, InName, Flags | RF_Transactional);
}

