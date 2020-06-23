// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Input/Events.h"
#include "TestDelegateCharacter.generated.h"



UCLASS()
class UDelegatepTestClass : public UObject
{
	GENERATED_BODY()
protected:
	int m_nValue = 0;
public:
	void DelegateProc1(int nCode)
	{
		this->m_nValue = nCode;
		UE_LOG(LogTemp, Log, TEXT("DelegateProc1 : %d"), nCode);
	}

	void DelegateProc2(int nCode)
	{
		this->m_nValue = nCode;
		UE_LOG(LogTemp, Log, TEXT("DelegateProc2 : %d"), nCode);
	}

	UFUNCTION()
	void DelegateUFunctionProc1(int nCode)
	{
		this->m_nValue = nCode;
		UE_LOG(LogTemp, Log, TEXT("DelegateUFunctionProc1 : %d"), nCode);
	}

	UFUNCTION()
	void DelegateUFunctionProc2(int nCode)
	{
		this->m_nValue = nCode;
		UE_LOG(LogTemp, Log, TEXT("DelegateUFunctionProc2 : %d"), nCode);
	}
};

class DelegateCppTestClass
{
	int m_nValue = 0;
public:

	void CppDelegateProc(int nCode)
	{
		this->m_nValue = nCode;
		UE_LOG(LogTemp, Log, TEXT("CppDelegateProc : %d"), nCode);
	}

	void CppDelegateProc2(int nCode)
	{
		this->m_nValue = nCode;
		UE_LOG(LogTemp, Log, TEXT("CppDelegateProc2 : %d"), nCode);
	}

	void CppDelegateProc3(int nCode)
	{
		this->m_nValue = nCode;
		UE_LOG(LogTemp, Log, TEXT("CppDelegateProc3 : %d"), nCode);
	}

	void CppDelegateProc4(int nCode)
	{
		this->m_nValue = nCode;
		UE_LOG(LogTemp, Log, TEXT("CppDelegateProc4 : %d"), nCode);
	}

	void CppDelegateProc5(int nCode)
	{
		this->m_nValue = nCode;
		UE_LOG(LogTemp, Log, TEXT("CppDelegateProc5 : %d"), nCode);
	}
};

DECLARE_DELEGATE_OneParam(FCharacterDelegate, int); // TBaseDelegate<void, int>
//typedef TBaseDelegate<void, int> FCharacterDelegate;

DECLARE_DELEGATE_RetVal_OneParam(bool, FCharacterDelegate_RetVal, int);  // TBaseDelegate<bool, int>
//typedef TBaseDelegate<bool, int> FCharacterDelegate_RetVal;

DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterDelegate_Multicast, int);  // TMulticastDelegate<void, int>
//typedef TMulticastDelegate<void, int> FCharacterDelegate_Multicast;

//DECLARE_EVENT_OneParam(ATestDelegateCharacter, FCharacterEvent, int);  // TBaseMulticastDelegate<void, int>
//class FCharacterEvent : public TBaseMulticastDelegate<void, int>
//{
//	friend class ATestDelegateCharacter;
//};

DECLARE_DYNAMIC_DELEGATE_OneParam(FCharacterDelegate_Dynamic, int, nCode); // TBaseDynamicDelegate<FWeakObjectPtr, void, int> 
DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FCharacterDelegate_DynamicRetVal, int, nCode);  // TBaseDynamicDelegate<FWeakObjectPtr, bool, int> 

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDelegate_DynamicMulticast, int, nCode); // TBaseDynamicMulticastDelegate<FWeakObjectPtr, void, int>
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDelegate_DynamicMulticast2, int, nCode); // TBaseDynamicMulticastDelegate<FWeakObjectPtr, void, int>

UCLASS(config = Game)
class SOMETEST_API ATestDelegateCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	static void StaticCharacterDelegateProc(int nCode);
	static void StaticDelegateProc(int nCode);

	UFUNCTION(BlueprintCallable)
		void OnBind();
	UFUNCTION(BlueprintCallable)
		void OnExecute();
	UFUNCTION(BlueprintCallable)
		void OnUnbind();

	FCharacterDelegate CharacterDelegate1;
	FCharacterDelegate CharacterDelegate2;
	FCharacterDelegate CharacterDelegate3;
	FCharacterDelegate CharacterDelegate4;
	FCharacterDelegate CharacterDelegate5;
	FCharacterDelegate CharacterDelegate6;
	FCharacterDelegate CharacterDelegate7;
	FCharacterDelegate CharacterDelegate8;
	FCharacterDelegate CharacterDelegate9;
	FCharacterDelegate CharacterDelegate10;
	FCharacterDelegate CharacterDelegate11;
	FCharacterDelegate CharacterDelegate12;
	FCharacterDelegate CharacterDelegate13;

	static void StaticDelegateProc2(int nCode);

	UFUNCTION(BlueprintCallable)
		void OnDelegateMulticastTest();

	FCharacterDelegate_Multicast CharacterDelegateMulticast1;
	FCharacterDelegate_Multicast CharacterDelegateMulticast2;
	FCharacterDelegate_Multicast CharacterDelegateMulticast3;
	FCharacterDelegate_Multicast CharacterDelegateMulticast4;
	FCharacterDelegate_Multicast CharacterDelegateMulticast5;
	FCharacterDelegate_Multicast CharacterDelegateMulticast6;
	FCharacterDelegate_Multicast CharacterDelegateMulticast7;
	FCharacterDelegate_Multicast CharacterDelegateMulticast8;

	DECLARE_EVENT_OneParam(ATestDelegateCharacter, FCharacterEvent, int);

	void OnTriggerEvent(int nCode);

	UFUNCTION(BlueprintCallable)
		void OnEventTest();

	FCharacterEvent CharacterEvent;

	UFUNCTION(BlueprintCallable)
		void OnDelegateDynamicTest();
	UFUNCTION()//注意此处要加宏
		bool DynamicMulticastProc(int nCode);

	FCharacterDelegate_Dynamic CharacterDelegateDynamic;//vs报错请忽略
	FCharacterDelegate_DynamicRetVal CharacterDelegateDynamic2;

	UFUNCTION(BlueprintCallable)
		void OnDelegateDynamicMulticastTest();

	UFUNCTION()//注意此处要加宏
		void DynamicMulticastProc1(int nCode);

	UFUNCTION()//注意此处要加宏
		void DynamicMulticastProc2(int nCode);

	UFUNCTION()//注意此处要加宏
		void DynamicMulticastProc3(int nCode);

	FCharacterDelegate_DynamicMulticast CharacterDelegateDynamicMulticast;

	UFUNCTION(BlueprintCallable)
		void BrocastCharacterDelegateDynamicMulticast(int nCode);

	UPROPERTY(BlueprintAssignable)
		FCharacterDelegate_DynamicMulticast2 CharacterDelegateDynamicMulticast2;
};
