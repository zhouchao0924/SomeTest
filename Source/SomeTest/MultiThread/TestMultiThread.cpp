// Fill out your copyright notice in the Description page of Project Settings.


#include "TestMultiThread.h"
#include "Async/TaskGraphInterfaces.h"
TestMultiThread::TestMultiThread()
{
}

uint32 TestMultiThread::Run()
{
	UE_LOG(LogTemp, Log, TEXT("Run"));
	if (TaskInterface)
	{
		TaskInterface->DoWork();
		
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			TestMultiThreadDelegate.ExecuteIfBound();
		}, TStatId(), NULL, ENamedThreads::GameThread);

		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
	}
	return 0;
}

void TestMultiThread::CreateThread(ITestMultiThreadInterface *TestMultiThreadInterface)
{
	TaskInterface = TestMultiThreadInterface;
	TaskThread = FRunnableThread::Create(this, TEXT("Hello"), 0, TPri_Normal);
}

TestMultiThread::~TestMultiThread()
{
}
