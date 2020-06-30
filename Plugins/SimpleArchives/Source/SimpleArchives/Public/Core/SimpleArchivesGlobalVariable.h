// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/SimpleArchivesInterface.h"

extern int32 SimpleSlotIndex;

namespace SimpleArchivesGlobalVariable
{
	SIMPLEARCHIVES_API void ClearSimpleArchivesArray();

	TArray<ISimpleArchivesInterface *> &GetSimpleArchivesArray();

	void AddArchivesInterface(ISimpleArchivesInterface *NewArchivesInterface);
	void RemoveArchivesInterface(ISimpleArchivesInterface *NewArchivesInterface);
}

