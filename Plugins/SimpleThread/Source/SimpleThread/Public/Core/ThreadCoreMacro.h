#pragma once
#include "Misc/ScopeLock.h"
#include "Abandonable/SimpleAbandonable.h"
#include "Async/TaskGraphInterfaces.h"

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

#define DDD(...) __VA_ARGS__

//以下是宏封装
#if PLATFORM_WINDOWS
	#include <iostream>
	#include <thread>
	#define CPUThreadNumber std::thread::hardware_concurrency()
#else
#if PLATFORM_LINUX
	#define CPUThreadNumber 12
#else
#if PLATFORM_MAC
	#define CPUThreadNumber 12
#endif
#endif
#endif

#define MUTEX_LOCL FScopeLock ScopeLock(&Mutex) 

//异步
#define USE_UE_THREAD_POOL_ASYNCTASK(ThreadDelegate) \
(new FAutoDeleteAsyncTask<FSimpleAbandonable>(ThreadDelegate))->StartBackgroundTask()

#define ASYNCTASK_UOBJECT(Object,...) \
USE_UE_THREAD_POOL_ASYNCTASK(FSimpleDelegate::CreateUObject(Object,__VA_ARGS__))

#define ASYNCTASK_Raw(Object,...) \
USE_UE_THREAD_POOL_ASYNCTASK(FSimpleDelegate::CreateRaw(Object,__VA_ARGS__))

#define ASYNCTASK_SP(Object,...) \
USE_UE_THREAD_POOL_ASYNCTASK(FSimpleDelegate::CreateSP(Object,__VA_ARGS__))

#define ASYNCTASK_Lambda(...) \
USE_UE_THREAD_POOL_ASYNCTASK(FSimpleDelegate::CreateLambda(__VA_ARGS__))

#define ASYNCTASK_UFunction(Object,...) \
USE_UE_THREAD_POOL_ASYNCTASK(FSimpleDelegate::CreateUFunction(Object,__VA_ARGS__))

//同步
#define USE_UE_THREAD_POOL_SYNCTASK(ThreadDelegate) \
{FAsyncTask<FSimpleAbandonable> *SimpleAbandonable = new FAsyncTask<FSimpleAbandonable>(ThreadDelegate); \
SimpleAbandonable->StartBackgroundTask(); \
SimpleAbandonable->EnsureCompletion(); \
delete SimpleAbandonable; }

#define SYNCTASK_UOBJECT(Object,...) \
USE_UE_THREAD_POOL_SYNCTASK(FSimpleDelegate::CreateUObject(Object,__VA_ARGS__))

#define SYNCTASK_Raw(Object,...) \
USE_UE_THREAD_POOL_SYNCTASK(FSimpleDelegate::CreateRaw(Object,__VA_ARGS__))

#define SYNCTASK_SP(Object,...) \
USE_UE_THREAD_POOL_SYNCTASK(FSimpleDelegate::CreateSP(Object,__VA_ARGS__))

#define SYNCTASK_Lambda(...) \
USE_UE_THREAD_POOL_SYNCTASK(FSimpleDelegate::CreateLambda(__VA_ARGS__))

#define SYNCTASK_UFunction(Object,...) \
USE_UE_THREAD_POOL_SYNCTASK(FSimpleDelegate::CreateUFunction(Object,__VA_ARGS__))

//呼叫线程 可以设置前置任务
#define CALL_THREAD(OtherTask,CallThreadName,InTaskDeletegate) \
FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(InTaskDeletegate,TStatId(),OtherTask,CallThreadName);

#define CALL_THREAD_UOBJECT(OtherTask,CallThreadName,Object,...) \
CALL_THREAD(OtherTask,CallThreadName,FSimpleDelegate::CreateUObject(Object,__VA_ARGS__))

#define CALL_THREAD_Raw(OtherTask,CallThreadName,Object,...) \
CALL_THREAD(OtherTask,CallThreadName,FSimpleDelegate::CreateRaw(Object,__VA_ARGS__))

#define CALL_THREAD_SP(OtherTask,CallThreadName,Object,...) \
CALL_THREAD(OtherTask,CallThreadName,FSimpleDelegate::CreateSP(Object,__VA_ARGS__))

#define CALL_THREAD_Lambda(OtherTask,CallThreadName,...) \
CALL_THREAD(OtherTask,CallThreadName,FSimpleDelegate::CreateLambda(__VA_ARGS__))

#define CALL_THREAD_UFunction(OtherTask,CallThreadName,Object,...) \
CALL_THREAD(CallThreadName,FSimpleDelegate::CreateUFunction(Object,__VA_ARGS__))


#define WAITING_OTHER_THREADS_COMPLETED(EventRef) \
FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);

#define WAITING_OTHER_THREADS_COMPLETED_MULTI(EventRef) \
FTaskGraphInterface::Get().WaitUntilTasksComplete(EventRef);
