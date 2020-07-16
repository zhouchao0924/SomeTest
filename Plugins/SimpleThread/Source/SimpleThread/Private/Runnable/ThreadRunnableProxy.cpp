#include "Runnable/ThreadRunnableProxy.h"
#include "HAL/RunnableThread.h"
#include "SimpleTreadPlatform.h"

int32 FThreadRunnable::ThreadCount = 0;

FThreadRunnable::FThreadRunnable(bool InInSuspendAtFirst)
	:IThreadProxy()
	, StopTaskCounter(0)
	, bSuspendAtFirst(InInSuspendAtFirst)
	, Thread(nullptr)
{

}

FThreadRunnable::~FThreadRunnable()
{
	if (Thread != NULL)
	{
		delete Thread;
		Thread = nullptr;
	}
}

void FThreadRunnable::BlockingAndCompletion()
{
	ThreadEvent.Trigger();

	WaitExecuteEvent.Wait();
}

void FThreadRunnable::CreateSafeThread()
{
	RunnableName = *FString::Printf(TEXT("SimpleThread-%i"), ThreadCount);
	Thread = FRunnableThread::Create(this,*RunnableName.ToString(), 0, TPri_BelowNormal);

	ThreadCount++;
}

void FThreadRunnable::WakeupThread()
{
	ThreadEvent.Trigger();
}

bool FThreadRunnable::IsSuspend()
{
	return ThreadEvent.IsWait();
}

void FThreadRunnable::WaitAndCompleted()
{
	Stop();
	
	if (ThreadEvent.IsWait())
	{
		ThreadEvent.Trigger();
		StartUpEvent.Wait();
		FPlatformProcess::Sleep(0.03f);
	}
}

uint32 FThreadRunnable::Run()
{
	while (StopTaskCounter.GetValue() == 0)
	{
		if (!bSuspendAtFirst)
		{
			ThreadEvent.Wait();//挂起线程
		}

		//执行业务逻辑
		if (ThreadDelegate.IsBound())
		{
			ThreadDelegate.Execute();

			ThreadDelegate.Unbind();
		}

		//激活挂起的启动线程
		WaitExecuteEvent.Trigger();

		bSuspendAtFirst = false;
	}

	return 0;
}

bool FThreadRunnable::Init()
{
	if (!FPlatformProcess::SupportsMultithreading())
	{
		Stop();
	}

	return StopTaskCounter.GetValue() == 0;
}

void FThreadRunnable::Stop()
{
	StopTaskCounter.Increment();
}

void FThreadRunnable::Exit()
{
	StartUpEvent.Trigger();
}