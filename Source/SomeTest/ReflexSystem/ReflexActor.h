// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReflexActor.generated.h"

UCLASS()
class SOMETEST_API AReflexActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
		class UBoxComponent* MyBoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "BoxComponent")
		class UBoxComponent* MyBoxComponent2;
	
public:	
	// Sets default values for this actor's properties
	AReflexActor();

public:
	UFUNCTION(BlueprintCallable, Category = "TestFunc")
		void TestFunc1() {}

	UFUNCTION(BlueprintCallable, Category = "TestFunc")
		bool TestFunc2()
		{
			return false;
		}

	UFUNCTION(BlueprintCallable, Category = "TestFunc")
		FString TestFunc3(int32 IntValue, const FString &Name, bool bShowWindows)
		{
			return FString(TEXT("Hllo"));
		}

	UFUNCTION()
		void TestFunc4() {}

	UFUNCTION(BlueprintNativeEvent, Category = "TestFunc")
		void TestFunc5(int32 IntValue);

	UFUNCTION(BlueprintImplementableEvent, Category = "TestFunc")
		void TestFunc6(bool bShowWindows);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
