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
#include "Core/Manage/ManageBase/ThreadManageBase.h"

//Create 可以绑定异步结束后通知的事件
struct SIMPLETHREAD_API FResourceLoadingManage :FTreadTemplateBase<IStreamableContainer,TSharedPtr<struct FStreamableHandle>>
{
	typedef FTreadTemplateBase<
		IStreamableContainer,
		TSharedPtr<struct FStreamableHandle>> Super;

	FResourceLoadingManage();
	~FResourceLoadingManage();

protected:

	//设置资源路径
	virtual void SetObjectPath(const TArray<FSoftObjectPath> &InObjectPath) final;

	//获取资源路径
	virtual TArray<FSoftObjectPath> &GetObjectPath() final;

	//获取流管理
	virtual FStreamableManager *GetStreamableManager()final;

private:
	TArray<FSoftObjectPath> SoftObjectPath;
	FStreamableManager *StreamableManager;
};
