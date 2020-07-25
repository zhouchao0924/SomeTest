#pragma once


#include "CoreMinimal.h"
#include "SimpleCode.h"
#include "ReflectiveData/SimpleCodeLibrary.Reflect.h"
#include "SimpleCodeLibrary.generated.h"


#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

UCLASS()
class BLUEPRINTTOOLCODE_API USimpleCodeLibrary : public USimpleCode
{
	GENERATED_BODY()
public:
	GENERATED_BT_BODY()
public:
	UFUNCTION(Meta = (CodeType = "Describe", Group = "SimpleCodeLibrary"))
	static void SetLocation(UStaticMeshComponent *Target, FVector InLocation);

	UFUNCTION(Meta = (CodeType = "Describe", Group = "SimpleCodeLibrary"))
	static void SetRotator(UStaticMeshComponent *Target, FRotator InRot);

	//设置模型
	UFUNCTION(Meta = (CodeType = "Describe", Group = "SimpleCodeLibrary"))
	static void SetMesh(UStaticMeshComponent *Target, UStaticMesh *InMesh);

	//设置材质
	UFUNCTION(Meta = (CodeType = "Describe", Group = "SimpleCodeLibrary"))
	static void SetMaterial(UStaticMeshComponent *Target, int32 Index, UMaterialInterface *InMaterial);

	//设置材质的颜色
	UFUNCTION(Meta = (CodeType = "Describe", Group = "SimpleCodeLibrary"))
	static void SetMaterialColor(UStaticMeshComponent *Target,FName ColorName,FLinearColor InColor,int32 MatralIndex);
};

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif