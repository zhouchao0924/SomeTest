// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TestThreadCharacter.generated.h"

UCLASS()
class SOMETEST_API ATestThreadCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//从蓝图加载资源路径
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		TArray<FSoftObjectPath> ObjectPath;

public:
	// Sets default values for this character's properties
	ATestThreadCharacter();

	//测试 UObject
	UFUNCTION()
		void T1(int32 i);

	//测试 UFunction
	UFUNCTION()
		void T2(int32 i, FString Mes);

	UFUNCTION()
		void Do();

	UFUNCTION()
		void Run();

	UFUNCTION()
		void OK();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	FTimerHandle Handle;

};
