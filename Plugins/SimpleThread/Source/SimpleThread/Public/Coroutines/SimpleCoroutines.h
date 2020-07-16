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

//协程的请求
struct SIMPLETHREAD_API FCoroutinesRequest
{
	FCoroutinesRequest(float InIntervalTime);

	//是否完成请求
	bool bCompleteRequest;

	//每一帧的时间间隔
	float IntervalTime;
};

//协程接口对象
class SIMPLETHREAD_API ICoroutinesObject :public TSharedFromThis<ICoroutinesObject>
{
	friend class ICoroutinesContainer;
public:
	ICoroutinesObject();
	virtual ~ICoroutinesObject(){}

	bool operator==(const ICoroutinesObject& SimpleThreadHandle)
	{
		return this->Handle == SimpleThreadHandle.Handle;
	}

	//唤醒协程
	FORCEINLINE void Awaken() { bAwaken = true; }
protected:

	virtual void Update(FCoroutinesRequest &CoroutinesRequest) = 0;

protected:

	//协程对象容器，里面存储着协程对象
	static TArray<TSharedPtr<ICoroutinesObject>> Array;
	uint8 bAwaken : 1;
	FSimpleThreadHandle Handle;
};
//协程句柄
typedef TWeakPtr<ICoroutinesObject> FCoroutinesHandle;

//协程对象
class SIMPLETHREAD_API FCoroutinesObject :public ICoroutinesObject
{
public:
	FCoroutinesObject(const FSimpleDelegate &InSimpleDelegate);
	FCoroutinesObject(float InTotalTime,const FSimpleDelegate &InSimpleDelegate);

	//一旦注册 将会每帧更新
	virtual void Update(FCoroutinesRequest &CoroutinesRequest) final;
private:

	//注册需要的代理
	FSimpleDelegate SimpleDelegate;

	//总共需要等待的时间
	const float TotalTime;

	//当前时间，主要用于记录时间
	float RuningTime;
};