// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Async/AsyncWork.h"
#include "Async/TaskGraphInterfaces.h"
/**
 * 
 */
class MyAsyncTask : public FNonAbandonableTask
{
	friend class FAsyncTask<MyAsyncTask>;

	int32 InstanceInt;

public:
	MyAsyncTask(int32 _InstanceInt)
		:InstanceInt(_InstanceInt)
	{

	}

	void DoWork()
	{
		UE_LOG(LogTemp, Log, TEXT("MyAsyncTask %d"), InstanceInt);
	}

	FORCEINLINE TStatId GetStatId()
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(MyAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};
