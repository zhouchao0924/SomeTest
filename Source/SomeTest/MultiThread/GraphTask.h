// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Async/TaskGraphInterfaces.h"
/**
 * 
 */
class GraphTask
{
public:
	GraphTask(float f)
		:m_f(f)
	{

	}

	static ESubsequentsMode::Type GetSubsequentsMode()
	{
		return ESubsequentsMode::TrackSubsequents;
	}

	FORCEINLINE TStatId GetStatId()
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(GraphTask, STATGROUP_TaskGraphTasks);
	}

	void DoTask(ENamedThreads::Type CurrentThread, FGraphEventRef Subsequents)
	{
		UE_LOG(LogTemp, Log, TEXT("GraphTask %f"), m_f);
	}

	static ENamedThreads::Type GetDesiredThread()
	{
		return ENamedThreads::AnyThread;
	}
private:
	float m_f;
};
