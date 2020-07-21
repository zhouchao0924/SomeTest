#include "SimpleUnrealPakType.h"

const TCHAR *FSimpleAssetFileName::PakTest = TEXT("PakTest");
const TCHAR *FSimpleAssetFileName::DLC = TEXT("DLC");
const TCHAR *FSimpleAssetFileName::Patchs = TEXT("Patch");
const TCHAR *FSimpleAssetFileName::CommontPatchs = TEXT("CommontPatch");

FSimpleUnrealAsset::FSimpleUnrealAsset(const FString &InAssetFileName, TArray<FString> &InAssetPaths, const FString &InMountPoint)
	:AssetFileName(InAssetFileName)
	,AssetPaths(Forward<TArray<FString>>(InAssetPaths))
	,MountPoint(InMountPoint)
{

}

FString FSimpleUnrealAsset::GetAssetName() const
{ 
	return FPackageName::GetShortName(AssetFileName);
}

bool FSimpleUnrealAsset::IsValid()
{
	return AssetFileName.Len() > 0 && AssetPaths.Num() > 0 && !MountPoint.IsEmpty();
}

FSimpleUnrealPakConfig::FSimpleUnrealPakConfig()
	:bConfiguration(false)
{

}
