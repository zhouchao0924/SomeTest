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

新浪博客				https://weibo.com/BZRZ/profile?s=6cm7D0  //这个博客从16年到现在 有三四年没怎么用过 以后说不定可以重新启用 先发个连接
*/

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "Interface/ProxyInterface.h"
#include "Core/SimpleSemaphore.h"

//UE4 Runnable线程，可以创建线程实例
class SIMPLETHREAD_API FThreadRunnable : public FRunnable, public IThreadProxy
{
public:
	FThreadRunnable(bool InSuspendAtFirst = false);
	virtual ~FThreadRunnable();

	//创建安全线程
	virtual void CreateSafeThread();

	//唤醒线程
	virtual void WakeupThread();

	//线程是否被挂起
	virtual bool IsSuspend();

	//让启动线程等待知道当前任务完成
	virtual void WaitAndCompleted();

	//阻塞启动线程
	virtual void BlockingAndCompletion();

private:

	//线程真正执行的地方
	virtual uint32 Run();

	virtual bool Init();
	virtual void Stop();
	virtual void Exit();

private:

	
	FThreadSafeCounter					StopTaskCounter; //线程安全计数
	bool								bSuspendAtFirst; //是否第一次阻塞线程
	class FRunnableThread*				Thread;			 //线程具体实例
	FName								RunnableName;	 //线程名字 以后会扩展
	FSimpleSemaphore					ThreadEvent;     //信号量 阻塞Thread
	FSimpleSemaphore					StartUpEvent;	 //信号量 阻塞启动线程
	FSimpleSemaphore					WaitExecuteEvent;//信号量 挂起启动线程
	FCriticalSection					Mutex;			 //锁

	static int32						ThreadCount;	 //线程序列计数
};