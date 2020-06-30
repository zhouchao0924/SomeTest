// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_ArchivesWidget.generated.h"

class ISimpleArchivesInterface;
/**
 * 
 */
UCLASS()
class SIMPLEARCHIVES_API UUI_ArchivesWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	ISimpleArchivesInterface *GetCurrentArchivesInterface();
};
