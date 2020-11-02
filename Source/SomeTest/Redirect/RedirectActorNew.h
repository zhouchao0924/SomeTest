// Fill out your copyright notice in the Description page of Project Settings.

//https://www.cnblogs.com/kekec/p/13447788.html

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RedirectActorNew.generated.h"

UCLASS()
class SOMETEST_API ARedirectActor : public AActor
{
	GENERATED_BODY()
	
public:	

	UFUNCTION(BlueprintCallable, Category = "Smartuil")
		void TestRedirectFunction(){}

};
