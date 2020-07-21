// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ProceduralMeshComponent.h"

DECLARE_DELEGATE_OneParam(FSimpleOneParamDelegate, const TArray<FString> &);

extern UProceduralMeshComponent *GProceduralMeshComponent;
extern UStaticMeshComponent *GMeshComponent;
extern FSimpleOneParamDelegate SimpleOneParamDelegate;
extern UWorld *ViewProtWorld;

extern TArray<AActor*> Actors;
extern UWorld *GetWorld();