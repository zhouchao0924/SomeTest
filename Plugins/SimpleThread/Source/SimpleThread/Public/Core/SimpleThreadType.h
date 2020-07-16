#pragma once
#include "CoreMinimal.h"
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

//主要作为GUID使用
struct SIMPLETHREAD_API FSimpleThreadHandle : public TSharedFromThis<FSimpleThreadHandle>
{
	FSimpleThreadHandle();

	bool operator==(const FSimpleThreadHandle& SimpleThreadHandle)
	{
		return
			this->GUIDTypeA == SimpleThreadHandle.GUIDTypeA &&
			this->GUIDTypeB == SimpleThreadHandle.GUIDTypeB &&
			this->GUIDTypeC == SimpleThreadHandle.GUIDTypeC &&
			this->GUIDTypeD == SimpleThreadHandle.GUIDTypeD;
	}

protected:
	uint64 GUIDTypeA;
	uint64 GUIDTypeB;
	uint64 GUIDTypeC;
	uint64 GUIDTypeD;
};

//线程状态
enum class EThreadState
{
	LEISURELY,	//闲置中
	WORKING,	//工作中
	ERROR		//出错
};

typedef TWeakPtr<FSimpleThreadHandle> FThreadHandle;
typedef TFunction<void()> FThreadLambda;

