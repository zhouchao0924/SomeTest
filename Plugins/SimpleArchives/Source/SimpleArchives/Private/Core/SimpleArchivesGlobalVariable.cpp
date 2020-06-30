// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SimpleArchivesGlobalVariable.h"

TArray<ISimpleArchivesInterface *> SimpleArchivesArray;

int32 SimpleSlotIndex = INDEX_NONE;

void SimpleArchivesGlobalVariable::ClearSimpleArchivesArray()
{
	SimpleArchivesArray.Empty();
}

TArray<ISimpleArchivesInterface *> & SimpleArchivesGlobalVariable::GetSimpleArchivesArray()
{
	return SimpleArchivesArray;
}

void SimpleArchivesGlobalVariable::AddArchivesInterface(ISimpleArchivesInterface *NewArchivesInterface)
{
	SimpleArchivesArray.AddUnique(NewArchivesInterface);
}

void SimpleArchivesGlobalVariable::RemoveArchivesInterface(ISimpleArchivesInterface *NewArchivesInterface)
{
	SimpleArchivesArray.Remove(NewArchivesInterface);
}
