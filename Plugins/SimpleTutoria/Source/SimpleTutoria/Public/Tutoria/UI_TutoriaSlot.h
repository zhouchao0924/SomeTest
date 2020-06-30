// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_TutoriaSlot.generated.h"



class UButton;

/**
 * 
 */
UCLASS()
class SIMPLETUTORIA_API UUI_TutoriaSlot : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UButton* PlayButton;
public:
	UUI_TutoriaSlot(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
		FString TutoriaPath;

	bool IsIndexValid();

	UFUNCTION()
		void Play();


	
protected:
	virtual void NativeConstruct() override;
};
