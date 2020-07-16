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
#include "Interface/ThreadManageInterface.h"
#include "ManageBase/ThreadManageBase.h"

//Bind   添加到任务队列中 如果有空置的线程可以直接执行该任务
//Create 直接在线程池里面找 如果有闲置的线程 直接运行当前任务
class SIMPLETHREAD_API FThreadTaskManagement : public FTreadTemplate<IThreadTaskContainer>
{
public:
	FThreadTaskManagement();
	~FThreadTaskManagement();

	//初始化 主要是为了初始化线程池
	void Init(int32 ThreadNum);

	//检测有没有闲置的线程
	void Tick(float DeltaTime);
};

