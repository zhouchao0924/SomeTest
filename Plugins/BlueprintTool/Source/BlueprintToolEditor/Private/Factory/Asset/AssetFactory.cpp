#include "Factory/Asset/AssetFactory.h"
#include "BlueprintData.h"

//没有这个类，右键菜单没有自定义的类型
UAssetFactory::UAssetFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UBlueprintData::StaticClass();
}

UObject* UAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	return NewObject<UBlueprintData>(InParent, InClass, InName, Flags | RF_Transactional);
}
