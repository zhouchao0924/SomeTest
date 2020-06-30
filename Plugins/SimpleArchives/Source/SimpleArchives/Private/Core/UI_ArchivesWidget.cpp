// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/UI_ArchivesWidget.h"
#include "Core/SimpleArchivesGlobalVariable.h"

ISimpleArchivesInterface * UUI_ArchivesWidget::GetCurrentArchivesInterface()
{
	TArray<ISimpleArchivesInterface *> &MyArchives = SimpleArchivesGlobalVariable::GetSimpleArchivesArray();
	for (const auto &Tmp : MyArchives)
	{
		if (Tmp->GetSimpleWorld())
		{
			return Tmp;
		}
	}

	return nullptr;
}

