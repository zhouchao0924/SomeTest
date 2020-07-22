// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiThread/TestMultiThreadInterface.h"
#include "MultiThread/TestMultiThread.h"
#include "SimpleHTTPType.h"
#include "SomeTestGameModeBase.generated.h"


/**
 * 
 */
class AReflexActor;

UCLASS()
class SOMETEST_API ASomeTestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

//public:
//	UFUNCTION(BlueprintNativeEvent, Category = "TestFunc")
//		FString TestFuncPPP(int32 IntValue);
//	FString TestFuncPPP_Implementation(int32 IntValue) { return ""; }
//
//	UFUNCTION(BlueprintImplementableEvent, Category = "TestFunc")
//		FString TestFuncCCC(bool bShowWindows);
//
//	UFUNCTION(BlueprintCallable, Category = "TestFunc")
//		FString TestFunc3(int32 IntValue, const FString &Name, bool bShowWindows);
//
//	void TestFunc4();
//	FString TestFunc5(int32 Index, TArray<AReflexActor*> &Actor);
//
//	UFUNCTION(BlueprintCallable)
//		FString TestFuncXXX(int32 Index, TArray<class AReflexActor*> &Actor);
//
//	//C++
//	UFUNCTION(BlueprintCallable)
//		UObject *TestFuncCX(int32 Index, float FFF, UObject *NewC, float C, TArray<class AActor*> &Actor);
//
//	UFUNCTION(BlueprintCallable)
//		FString TestFunc6(int32 Index, TArray<class AReflexActor*> &Actor);
//
//	UFUNCTION(BlueprintCallable)
//		FString TestFunc23(int32 Index, float cc);
//
//	UFUNCTION(BlueprintCallable)
//		UObject *TestFunc7();
//
//	UFUNCTION(BlueprintCallable)
//		FString &TestFunc8();
//
//	bool DoFun(UObject* Object, FString FunName, FString String);
//	bool DoFun2(UObject* Object, FString FunName, FString String);
//	bool DoFun3(UObject* Object, FString FunName, FString String);
//	void DoFun4(UObject* Object, FString FunName);
//
//	FString DoFun5(UObject* Object, int32 Index, TArray<AReflexActor*> &Actor);

//private:
//	UFUNCTION()
//		bool TestPrint(FString Index);
//
//	static void execTestFunc4(UObject* Context, FFrame& Stack, /*RESULT_DECL*/ void*const Z_Param__Result)
//	{
//		Stack.Code += !!Stack.Code;
//		{
//			((ThisClass*)Context)->TestFunc4();
//		}
//	}
//
//	static void execTestFunc5c(UObject* Context, FFrame& Stack, /*RESULT_DECL*/ void*const Z_Param__Result)
//	{
//		TArray<AReflexActor*> Z_Param_Out_ActorTemp;
//		TArray<AReflexActor*>& Z_Param_Out_Actor = Stack.StepCompiledInRef<UArrayProperty, TArray<AReflexActor*>>(&Z_Param_Out_ActorTemp);
//
//		UIntProperty::TCppType Z_Param_IntValue = UIntProperty::GetDefaultPropertyValue();
//		Stack.StepCompiledIn<UIntProperty>(&Z_Param_IntValue);
//
//		Stack.Code += !!Stack.Code;
//		{
//			*(FString*)Z_Param__Result = ((ThisClass*)Context)->TestFunc5(Z_Param_IntValue, Z_Param_Out_Actor);
//		}
//	}

//∂‡œﬂ≥Ã≤‚ ‘
public:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void print();

	ITestMultiThreadInterface *NewInterface;

	TestMultiThread *NewTestMultiThread;

	//Pak
	UFUNCTION()
		void TestPak(int32 InSurplus, int64 InTotal, const FString& Mess);
	UFUNCTION()
		void TestPak2(const FSimpleHttpRequest Request, const FSimpleHttpResponse Response, bool bConnectedSuccessfully);
	UFUNCTION()
		void TestPak3(const FSimpleHttpRequest Request, int32 BytesSent, int32 BytesReceived);
	UFUNCTION()
		void TestPak4(const FSimpleHttpRequest Request, const FString HeaderName, const FString NewHeaderValue);

};
