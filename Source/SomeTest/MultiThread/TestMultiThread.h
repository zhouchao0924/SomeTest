// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "TestMultiThreadInterface.h"
/**
 * 
 */

DECLARE_DELEGATE(FTestMultiThreadDelegate);

class SOMETEST_API TestMultiThread : public FRunnable
{
public:
	TestMultiThread();

	FTestMultiThreadDelegate TestMultiThreadDelegate;

	virtual uint32 Run();

	void CreateThread(ITestMultiThreadInterface *TestMultiThreadInterface);

	~TestMultiThread();

private:
	FRunnableThread *TaskThread;
	ITestMultiThreadInterface *TaskInterface;
};
