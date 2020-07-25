#pragma once


#include "CoreMinimal.h"
#include "SimpleCode.h"
#include "ReflectiveData/SimpleCodeBase.Reflect.h"
#include "SimpleCodeBase.generated.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

UCLASS()
class BLUEPRINTTOOLCODE_API USimpleCodeBase : public USimpleCode
{
	GENERATED_BODY()
public:
	GENERATED_BT_BODY()

		//USimpleCodeBase_18_GENERATED_BODY_BT
		//Z_BT_USimpleCodeBase
public:

	//¥Ú”°
	UFUNCTION(Meta = (CodeType = "Describe", Group = "SimpleCode"))
		static void PrintString(FString String);

	UFUNCTION(Meta = (CodeType = "PureDescribe", Group = "BasicVariables"))
		static FString SetString(FString Context);

	UFUNCTION(Meta = (CodeType = "PureDescribe", Group = "BasicVariables"))
		static bool SetBool(bool Context);

	UFUNCTION(Meta = (CodeType = "PureDescribe", Group = "BasicVariables"))
		static float SetFloat(float Context);

	UFUNCTION(Meta = (CodeType = "PureDescribe", Group = "BasicVariables"))
		static int32 SetInt32(int32 Context);

	UFUNCTION(Meta = (CodeType = "PureDescribe", Group = "BasicVariables"))
		static FText SetText(FText Context);

	UFUNCTION(Meta = (CodeType = "PureDescribe", Group = "BasicVariables"))
		static FName SetName(FName Context);

	UFUNCTION(Meta = (CodeType = "PureDescribe", Group = "BasicVariables"))
		static FLinearColor SetColor(FLinearColor Context);

	UFUNCTION(Meta = (CodeType = "PureDescribe", Group = "BasicVariables"))
		static uint8 SetByte(uint8 Context);

	UFUNCTION(Meta = (CodeType = "PureDescribe", Group = "BasicVariables"))
		static UObject* SetObject(UObject *Context);
};

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif