// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestMacroActor.generated.h"

UENUM(Meta = (Bitflags))
enum class EColorBits
{
	ECB_Red,
	ECB_Green,
	ECB_Blue
};

UCLASS()
class SOMETEST_API ATestMacroActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestMacroActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Smartuil|BlueprintFunc")
		void BlueprintCallableFunction();

	UFUNCTION(BlueprintGetter)
		virtual float GetBlueprintGetterAndSetterParam() const
		{
			return BlueprintGetterAndSetterParam * 2.f;
		}

	UFUNCTION(BlueprintSetter)
		virtual void SetBlueprintGetterAndSetterParam(float InFloat)
		{
			BlueprintGetterAndSetterParam = InFloat;
		}

	UPROPERTY(EditAnywhere, BlueprintGetter = GetBlueprintGetterAndSetterParam, BlueprintSetter = SetBlueprintGetterAndSetterParam, Category = "Smartuil|BlueprintFunc")
		float BlueprintGetterAndSetterParam;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, meta = (DisplayName = "Blueprint Implementable Event Function"), Category = "Smartuil|BlueprintFunc")
		float BlueprintImplementableEventFunction(float InFloat);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (DisplayName = "Blueprint Native Event Function"), Category = "Smartuil|BlueprintFunc")
		FString BlueprintNativeEventFunction(AActor* InActor);

	UFUNCTION(BlueprintPure, Category = "Smartuil|BlueprintFunc")
		AActor* BlueprintPureFunction();

	UFUNCTION(CallInEditor, Category = "CallInEditorFunc")
		void BlueprintCallInEditorFunction();

public:
	UPROPERTY(EditAnywhere, Meta = (Bitmask))
		int32 BasicBits;

	UPROPERTY(EditAnywhere, Meta = (Bitmask, BitmaskEnum = "EColorBits"))
		int32 ColorFlags;

	uint32 bIsHungry : 1;
	bool bIsThirsty;

	UPROPERTY(VisibleAnywhere, AdvancedDisplay, Category = "Smartuil")
		FString AdvancedDisplayParam;

	UPROPERTY(BlueprintReadOnly, Category = "Smartuil|BlueprintFunc")
		float BlueprintReadOnlyParam;

	UPROPERTY(BlueprintReadWrite, Category = "Smartuil|BlueprintFunc")
		float BlueprintReadWriteParam;

	UPROPERTY(VisibleAnywhere, Category = "Smartuil|Visible")
		FString VisibleAnywhereParam;

	UPROPERTY(EditAnywhere, Category = "Smartuil|Edit")
		float EditAnywhereParam;

	UPROPERTY(VisibleDefaultsOnly, Category = "Smartuil|Visible")
		FText VisibleDefaultOnlyParam;

	UPROPERTY(EditDefaultsOnly, Category = "Smartuil|Edit")
		FText EditDefaultsOnlyParam;

};
