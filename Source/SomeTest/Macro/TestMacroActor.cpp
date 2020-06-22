// Fill out your copyright notice in the Description page of Project Settings.


#include "TestMacroActor.h"

// Sets default values
ATestMacroActor::ATestMacroActor()
	:BlueprintGetterAndSetterParam(10)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestMacroActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestMacroActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestMacroActor::BlueprintCallableFunction()
{

}

AActor* ATestMacroActor::BlueprintPureFunction()
{
	return nullptr;
}

void ATestMacroActor::BlueprintCallInEditorFunction()
{
	
}

FString ATestMacroActor::BlueprintNativeEventFunction_Implementation(AActor* InActor)
{
	return InActor->GetName();
}

