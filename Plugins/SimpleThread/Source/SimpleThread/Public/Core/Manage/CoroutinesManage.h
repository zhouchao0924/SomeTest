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

//Bind		绑定后可以设置时间，多久执行
//Create	绑定完毕返回一个Handle ,由程序员来决定什么时候执行
struct SIMPLETHREAD_API FCoroutinesManage :FTreadTemplateBase<ICoroutinesContainer, FCoroutinesHandle>
{
public:
	FCoroutinesManage();

	//用于监视任务情况
	void Tick(float );
public:
	template<class UserClass, typename... VarTypes>
	void BindRaw(float InTotalTime, UserClass *TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		*this << InTotalTime << FSimpleDelegate::CreateRaw(TargetClass, InMethod, Vars...);
	}

	template<class UserClass, typename... VarTypes>
	void BindUObject(float InTotalTime, UserClass *TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		*this << InTotalTime << FSimpleDelegate::CreateUObject(TargetClass, InMethod, Vars...);
	}

	template<class UserClass, typename... VarTypes>
	void BindSP(float InTotalTime, const TSharedRef<UserClass>& TargetClass, typename TMemFunPtrType<false, UserClass, void(VarTypes...)>::Type InMethod, VarTypes... Vars)
	{
		*this << InTotalTime << FSimpleDelegate::CreateSP(TargetClass, InMethod, Vars...);
	}

	template<typename FunctorType, typename... VarTypes>
	void BindLambda(float InTotalTime, FunctorType &&InMethod, VarTypes... Vars)
	{
		*this << InTotalTime << FSimpleDelegate::CreateLambda(InMethod, Vars...);
	}

	template<class UserClass, typename... VarTypes>
	void BindUFunction(float InTotalTime, UserClass *TargetClass, const FName& InMethod, VarTypes... Vars)
	{
		*this << InTotalTime << FSimpleDelegate::CreateUFunction(TargetClass, InMethod, Vars...);
	}
};