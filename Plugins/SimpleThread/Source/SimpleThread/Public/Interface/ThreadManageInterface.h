#pragma once
/*
下面的网址讲解了如何使用本套插件
DocURL：			https://zhuanlan.zhihu.com/p/78023533

下面的网址讲解如何开发当前的插件
MarketplaceURL :  http://www.aboutcg.org/course/tut_ue4suite_190813/?action=

如果想了解更多关于UE4教程请参考：
URL :				https://zhuanlan.zhihu.com/p/60117613

如果想系统了解人宅系列教程以及相关插件迭代更新 可以关注我的博客
URL :				http://renzhai.net/

如果想了解我们下一节的课程安排可以 可以在微信公众号搜所 人宅 关注即可 我们会推送很多富有技术的文章

新浪博客 https://weibo.com/BZRZ/profile?s=6cm7D0  //这个博客从16年到现在 有三四年没怎么用过 以后说不定可以重新启用 先发个连接

*/

#include "CoreMinimal.h"
#include "Interface/ProxyInterface.h"
#include "Containers/Queue.h"
#include "Core/ThreadCoreMacro.h"
#include "Abandonable/SimpleAbandonable.h"
#include "Coroutines/SimpleCoroutines.h"
#include "Async/TaskGraphInterfaces.h"
#include "Engine/StreamableManager.h"
#include "Runnable/ThreadRunnableProxy.h"

//线程容器接口
class IThreadContainer
{
public:
	IThreadContainer() {}
	virtual ~IThreadContainer() {}

protected:
	FCriticalSection Mutex;
};

//可以自由创建线程，不受限制，创建完毕的线程不会马上销毁，如果有需要可以再次使用，
//具有同步和异步功能，一般运用在小场景
class IThreadProxyContainer :public TArray<TSharedPtr<IThreadProxy>>, public IThreadContainer
{
protected:
	typedef TArray<TSharedPtr<IThreadProxy>>	TProxyArray;

public:
	IThreadProxyContainer &operator<<(const TSharedPtr<IThreadProxy> &ThreadProxy)
	{
		MUTEX_LOCL;

		ThreadProxy->CreateSafeThread();
		this->Add(ThreadProxy);

		return *this;
	}

	FThreadHandle operator>>(const FSimpleDelegate &ThreadProxy)
	{
		FThreadHandle ThreadHandle = nullptr;
		{
			MUTEX_LOCL;
			for (auto &Tmp : *this)
			{
				if (Tmp->IsSuspend() && !Tmp->GetThreadDelegate().IsBound())
				{
					Tmp->GetThreadDelegate() = ThreadProxy;
					ThreadHandle = Tmp->GetThreadHandle();
					Tmp->WakeupThread();

					break;
				}
			}
		}

		if (!ThreadHandle.IsValid())
		{
			TSharedPtr<IThreadProxy> Proxy = MakeShareable(new FThreadRunnable(true));
			Proxy->GetThreadDelegate() = ThreadProxy;
			*this << Proxy;

			ThreadHandle = Proxy->GetThreadHandle();
		}

		return ThreadHandle;
	}

	FThreadHandle operator<<(const FSimpleDelegate &ThreadProxy)
	{
		FThreadHandle ThreadHandle = nullptr;
		{
			MUTEX_LOCL;
			for (auto &Tmp : *this)
			{
				if (Tmp->IsSuspend() && !Tmp->GetThreadDelegate().IsBound())
				{
					Tmp->GetThreadDelegate() = ThreadProxy;
					ThreadHandle = Tmp->GetThreadHandle();

					break;
				}
			}
		}

		if (!ThreadHandle.IsValid())
		{
			ThreadHandle = *this << MakeShareable(new FThreadRunnable) << ThreadProxy;
		}

		return ThreadHandle;
	}

	TSharedPtr<IThreadProxy> operator>>(const FThreadHandle &Handle)
	{
		MUTEX_LOCL;
		for (auto &Tmp : *this)
		{
			if (Tmp->GetThreadHandle() == Handle)
			{
				return Tmp;
			}
		}

		return NULL;
	}
};

//线程的任务管理，可以自动管理任务，自动配闲置的线程池，实现高效的利用线程池特点
class IThreadTaskContainer :public TQueue<FSimpleDelegate>, public TArray<TSharedPtr<IThreadProxy>>, public IThreadContainer
{
protected:
	typedef TArray<TSharedPtr<IThreadProxy>>	TProxyArray;
	typedef TQueue<FSimpleDelegate>				TEventQueue;

public:
	void operator<<(const FSimpleDelegate &ThreadProxy)
	{
		MUTEX_LOCL;

		this->Enqueue(ThreadProxy);
	}

	bool operator<<=(FSimpleDelegate &ThreadProxy)
	{
		MUTEX_LOCL;

		return this->Dequeue(ThreadProxy);
	}

	IThreadTaskContainer& operator<<(const TSharedPtr<IThreadProxy> &ThreadProxy)
	{
		MUTEX_LOCL;

		this->Add(ThreadProxy);
		ThreadProxy->CreateSafeThread();

		return *this;
	}

	void operator>>(const FSimpleDelegate &ThreadProxy)
	{
		bool bSuccessful = false;
		{
			MUTEX_LOCL;
			for (auto &Tmp : *this)
			{
				if (Tmp->IsSuspend() && !Tmp->GetThreadDelegate().IsBound())
				{
					Tmp->GetThreadDelegate() = ThreadProxy;
					Tmp->WakeupThread();

					bSuccessful = true;
					break;
				}
			}
		}

		if (!bSuccessful)
		{
			*this << ThreadProxy;
		}
	}
};

//同步异步线程接口
class IAbandonableContainer :public IThreadContainer
{
protected:
	//同步绑定
	void operator<<(const FSimpleDelegate& ThreadDelegate)
	{
		FAsyncTask<FSimpleAbandonable> *SimpleAbandonable = new FAsyncTask<FSimpleAbandonable>(ThreadDelegate);
		SimpleAbandonable->StartBackgroundTask();
		SimpleAbandonable->EnsureCompletion();
		delete SimpleAbandonable;
	}

	//异步绑定
	void operator>>(const FSimpleDelegate& ThreadDelegate)
	{
		(new FAutoDeleteAsyncTask<FSimpleAbandonable>(ThreadDelegate))->StartBackgroundTask();
	}
};

//协程接口
class ICoroutinesContainer 
{
public:
	ICoroutinesContainer()
	 :TmpTotalTime(0.f)
	{

	}
	virtual ~ICoroutinesContainer()
	{
		ICoroutinesObject::Array.Empty();
	}

	ICoroutinesContainer &operator<<(float TotalTime)
	{
		TmpTotalTime = TotalTime;

		return *this;
	}

	ICoroutinesContainer &operator<<(const FSimpleDelegate& ThreadDelegate)
	{
		ICoroutinesObject::Array.Add(MakeShareable(new FCoroutinesObject(TmpTotalTime, ThreadDelegate)));

		return *this;
	}

	void operator<<=(float Time)
	{
		TArray<TSharedPtr<ICoroutinesObject>> RemoveObject;
		for (int32 i = 0 ;i < ICoroutinesObject::Array.Num() ;i++)
		{
			FCoroutinesRequest Request(Time);

			ICoroutinesObject::Array[i]->Update(Request);
			if (Request.bCompleteRequest)
			{
				RemoveObject.Add(ICoroutinesObject::Array[i]);
			}
		}

		for (auto &RemoveInstance : RemoveObject)
		{
			ICoroutinesObject::Array.Remove(RemoveInstance);
		}
	}

	FCoroutinesHandle operator>>(const FSimpleDelegate& ThreadDelegate)
	{
		ICoroutinesObject::Array.Add(MakeShareable(new FCoroutinesObject(ThreadDelegate)));

		return ICoroutinesObject::Array[ICoroutinesObject::Array.Num() - 1];
	}
private:
	float TmpTotalTime;
};

//图表线程接口
class IGraphContainer :public IThreadContainer
{
protected:

	//呼叫主线程
	FGraphEventRef operator<<(const FSimpleDelegate& ThreadDelegate)
	{
		MUTEX_LOCL;
		return FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
			ThreadDelegate,
			TStatId(),
			nullptr,
			ENamedThreads::GameThread);
	}

	//绑定任意线程
	FGraphEventRef operator>>(const FSimpleDelegate& ThreadDelegate)
	{
		MUTEX_LOCL;
		return FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
			ThreadDelegate,
			TStatId(),
			nullptr,
			ENamedThreads::AnyThread);
	}
};

//资源管理接口
class IStreamableContainer
{
public:
	virtual ~IStreamableContainer() {}

	//存储路径
	IStreamableContainer &operator>>(const TArray<FSoftObjectPath> &InObjectPath)
	{
		SetObjectPath(InObjectPath);

		return *this;
	}
	//异步资源
	TSharedPtr<struct FStreamableHandle> operator>>(const FSimpleDelegate& ThreadDelegate)
	{
		return GetStreamableManager()->RequestAsyncLoad(GetObjectPath(), ThreadDelegate);
	}

	//同步读取
	TSharedPtr<struct FStreamableHandle> operator<<(const TArray<FSoftObjectPath> &InObjectPath)
	{
		return GetStreamableManager()->RequestSyncLoad(InObjectPath);
	}

protected:
	virtual void SetObjectPath(const TArray<FSoftObjectPath> &InObjectPath) = 0;
	virtual TArray<FSoftObjectPath> &GetObjectPath() = 0;
	virtual FStreamableManager *GetStreamableManager() = 0;
};