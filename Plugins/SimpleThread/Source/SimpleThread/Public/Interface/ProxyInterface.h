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
#include "Core/SimpleThreadType.h"

//代理线程的接口类，提供了基础的方法
class SIMPLETHREAD_API IThreadProxy : public TSharedFromThis<IThreadProxy>
{
public:
	IThreadProxy();
	virtual ~IThreadProxy(){}

	//创建安全线程
	virtual void CreateSafeThread() = 0;

	//唤醒线程
	virtual void WakeupThread() = 0;

	//线程是否被挂起
	virtual bool IsSuspend() = 0;

	//让启动线程等待知道当前任务完成
	virtual void WaitAndCompleted() = 0;

	//阻塞启动线程
	virtual void BlockingAndCompletion() = 0;

	//主要任务执行代理
	FORCEINLINE FSimpleDelegate &GetThreadDelegate() { return ThreadDelegate; }

	//一个用于监视的句柄
	FORCEINLINE FThreadHandle GetThreadHandle() { return SimpleThreadHandle; }
protected:

	//代理实例
	FSimpleDelegate ThreadDelegate;

private:
	//句柄实例，唯一，其他地方都是弱引用
	TSharedPtr<FSimpleThreadHandle> SimpleThreadHandle;
};