// Fill out your copyright notice in the Description page of Project Settings.


#include "AddTestObjectAssetFactory.h"
#include "Factory/Asset/AssetFactory.h"
#include "TestObject.h"

UAddTestObjectAssetFactory::UAddTestObjectAssetFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UTestObject::StaticClass();
}

UObject* UAddTestObjectAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	return NewObject<UTestObject>(InParent, InClass, InName, Flags | RF_Transactional);
}
