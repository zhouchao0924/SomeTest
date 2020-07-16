// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*
非常感谢您使用本套插件，这套插件您可以使用在您的项目当中，我们已经为您做过严格的测试
该线程完全支持您的多线程项目方案。
如果您还需要更多功能欢迎加入我们的QQ群 ：846023597  密码 helloworld  如果遇到什么不会的问题可以直接@人宅

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

#pragma once

//如果想了解详细教程可以打开
#pragma region Course

//教程文档，防止以上链接失效
//哈喽，大家好，我叫人宅，这里我们为大家介绍一下关于UE4插件SimpleThread的使用技巧。
//这是一款融合着多种线程模式的插件，SimpleThread，英译过来为简单的线程，并不是它的代码简单，
//而是使用者可以完全不用关心底层线程是如何运行，只管使用就好
//只需要把事件绑定到该插件上即可完成各种多线程方案。
//
//目录
//前置环境测试准备
//代理线程(FThreadProxyManage)
//任务线程(FThreadTaskManagement)
//同步异步线程(FThreadAbandonableManage)
//协程(FCoroutinesManage)
//Windows原生线程(FWindowsPlatformThread)
//异步资源读取(FResourceLoadingManage)
//图表线程(ThreadGraphManage)
//其他图表线程
//1.前置环境测试准备
//我们先布置好要测试SimpleThread线程的环境
//
////锁
//FCriticalSection					Mutex;
//
////打印
//void ThreadP(const FString Mes)
//{
//	{
//		FScopeLock ScopeLock(&Mutex);
//		if (GEngine)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, *Mes);
//		}
//	}
//}
//
////结构体
//struct FMyStruct
//{
//	void Hello(FString Mes)
//	{
//		ThreadP(FString::Printf(TEXT("FMyStruct::Hello : %s"), *Mes));
//	}
//};
//
////智能指针
//struct FMyStructSP :public TSharedFromThis<FMyStructSP>
//{
//	void HelloSP(FString Mes)
//	{
//		ThreadP(FString::Printf(TEXT("FMyStructSP::Hello : %s"), *Mes));
//	}
//};
//
////类
//UCLASS(config = Game)
//class AThreadTestCharacter : public ACharacter
//{
//
//	GENERATED_BODY()
//public:
//	//测试 UObject
//	UFUNCTION()
//		void T1(int32 i);
//	//测试 UFunction
//	UFUNCTION()
//		void T2(int32 i, FString Mes);
//	UFUNCTION()
//		void Do();
//	UFUNCTION()
//		void Run();
//	UFUNCTION()
//		void OK();
//protected:
//	virtual void BeginPlay();
//};
//
//void AThreadTestCharacter::BeginPlay()
//{
//	Super::BeginPlay();
//}
//void AThreadTestCharacter::T1(int32 i)
//{
//	ThreadP(FString::Printf(TEXT("T1 : %i"), i));
//}
//
//void AThreadTestCharacter::T2(int32 i, FString Mes)
//{
//	ThreadP(FString::Printf(TEXT("T2 : %i ,Mes = %s"), i, *Mes));
//}
//
//void AThreadTestCharacter::OK()
//{
//	ThreadP(TEXT("Windows Run"));
//}
//
//void AThreadTestCharacter::Run()
//{
//
//}
//
//void AThreadTestCharacter::Do()
//{
//
//}
//首先创建一个类AThreadTestCharacter 它是继承自 ACharacter。
//
//然后我们来测试SimpleThread提供的各种线程。
//
//2.代理线程(FThreadProxyManage)
//代理线程有着自己的线程池方案，当你通过代理线程创建一个线程，那么这个线程会加入到线程池内，不会被销毁掉而是挂起，已等待下一个任务。
//
//我们来演示一下它具体的使用方式：
//
//CreateXXX：创建线程并且直接执行任务, 是最快捷的异步方式
//
//void AThreadTestCharacter::BeginPlay()
//{
//	Super::BeginPlay();
//
//	FMyStruct MyStruct;
//	TSharedPtr<FMyStructSP> MyStructSP = MakeShareable(new FMyStructSP);
//
//	GThread::GetProxy().CreateUObject(this, &AThreadTestCharacter::T1, 777);
//	GThread::GetProxy().CreateRaw(&MyStruct, &FMyStruct::Hello, FString("Hello~"));
//	GThread::GetProxy().CreateSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
//	GThread::GetProxy().CreateUFunction(this, TEXT("T2"), 123, "T22222");
//	GThread::GetProxy().CreateLambda([](FString Mes)
//	{
//		ThreadP(Mes);
//	}, "Lambda");
//
//}
//除此之外我们还有Bind
//
//BindXXX : 创建线程并且绑定任务，但不执行 通过 Join和Detach来决定是异步执行还是同步执行;
//
//TArray<FThreadHandle> ThreadHandle;
//
//void AThreadTestCharacter::BeginPlay()
//{
//	Super::BeginPlay();
//
//	ThreadHandle.SetNum(5);
//	FMyStruct MyStruct;
//	TSharedPtr<FMyStructSP> MyStructSP = MakeShareable(new FMyStructSP);
//
//	ThreadHandle[0] = GThread::GetProxy().BindUObject(this, &AThreadTestCharacter::T1, 777);
//	ThreadHandle[1] = GThread::GetProxy().BindRaw(&MyStruct, &FMyStruct::Hello, FString("Hello~"));
//	ThreadHandle[2] = GThread::GetProxy().BindSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
//	ThreadHandle[3] = GThread::GetProxy().BindUFunction(this, TEXT("T2"), 123, "T22222");
//	ThreadHandle[4] = GThread::GetProxy().BindLambda([](FString Mes)
//	{
//		ThreadP(Mes);
//	}, "Lambda");
//
//
//	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AThreadTestCharacter::Do, 3.f);
//
//}
//
////执行我们的线程与Std用法类似
//void AThreadTestCharacter::Do()
//{
//	if (Handle.IsValid())
//	{
//		GetWorld()->GetTimerManager().ClearTimer(Handle);
//	}
//	//同步执行
//	//for (auto &Tmp : ThreadHandle)
//	//{
//	//	GThread::GetProxy().Join(Tmp);
//	//}
//
//	//异步执行
//	//for (auto &Tmp : ThreadHandle)
//	//{
//	//	GThread::GetProxy().Detach(Tmp);
//	//}
//}
//3.任务线程(FThreadTaskManagement)
//使用任务线程，可以往线程里面不断的去放任务
//
//BindXXX 添加到任务队列中 如果有空置的线程可以直接执行该任务；
//
//我们来看看它的实际运用：
//
//void AThreadTestCharacter::BeginPlay()
//{
//	Super::BeginPlay();
//
//	GThread::GetTask().BindUObject(this, &AThreadTestCharacter::T1, 777);
//	GThread::GetTask().BindRaw(&MyStruct1, &FMyStruct::Hello, FString("Hello~"));
//	GThread::GetTask().BindSP(MyStructSP1.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
//	GThread::GetTask().BindUFunction(this, TEXT("T2"), 123, FString("T22222"));
//	GThread::GetTask().BindLambda([](FString Mes)
//	{
//		ThreadP(Mes);
//	}, "Lambda");
//
//}
//用法和代理线程差不多，我们来看看CreateXXX
//
//CreateXXX 直接在线程池里面找 如果有闲置的线程 直接运行当前任务;
//
//void AThreadTestCharacter::BeginPlay()
//{
//	Super::BeginPlay();
//
//	GThread::GetTask().CreateUObject(this, &AThreadTestCharacter::T1, 777);
//	GThread::GetTask().CreateRaw(&MyStruct1, &FMyStruct::Hello, FString("Hello~"));
//	GThread::GetTask().CreateSP(MyStructSP1.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
//	GThread::GetTask().CreateUFunction(this, TEXT("T2"), 123, FString("T22222"));
//	GThread::GetTask().CreateLambda([](FString Mes)
//	{
//		ThreadP(Mes);
//	}, "Lambda");
//}
//根据官方:任务线程的以后会添加优先级任务设置的扩展；
//
//	4.同步异步线程(FThreadAbandonableManage)
//	该线程是UE4线程池内的线程，轻巧便捷，要么执行同步，要么执行异步，同样我们的同步异步线程也有Bind和Create
//
//	BindXXX 同步绑定 会阻塞启动线程 完成任务后激活启动线程
//
//	void AThreadTestCharacter::BeginPlay()
//{
//	Super::BeginPlay();
//
//	GThread::GetAbandonable().BindUObject(this, &AThreadTestCharacter::T1, 777);
//	GThread::GetAbandonable().BindRaw(&MyStruct1, &FMyStruct::Hello, FString("Hello~"));
//	GThread::GetAbandonable().BindSP(MyStructSP1.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
//	GThread::GetAbandonable().BindUFunction(this, TEXT("T2"), 123, FString("T22222"));
//	GThread::GetAbandonable().BindLambda([](FString Mes)
//	{
//		ThreadP(Mes);
//	}, "Lambda");
//}
//	 CreateXXX 异步绑定 直接启动，任务完成后自动销毁;
//
//	 void AThreadTestCharacter::BeginPlay()
//	 {
//		 Super::BeginPlay();
//
//		 GThread::GetAbandonable().CreateUObject(this, &AThreadTestCharacter::T1, 777);
//		 GThread::GetAbandonable().CreateRaw(&MyStruct1, &FMyStruct::Hello, FString("Hello~"));
//		 GThread::GetAbandonable().CreateSP(MyStructSP1.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
//		 GThread::GetAbandonable().CreateUFunction(this, TEXT("T2"), 123, FString("T22222"));
//		 GThread::GetAbandonable().CreateLambda([](FString Mes)
//		 {
//			 ThreadP(Mes);
//		 }, "Lambda");
//	 }
//	 除此之外还添加了便捷的宏启动线程，该线程也是可以执行同步异步
//
//		 同步宏
//
//		 void AThreadTestCharacter::BeginPlay()
//	 {
//		 Super::BeginPlay();
//
//		 SYNCTASK_UOBJECT(this, &AThreadTestCharacter::T1, 777);
//		 SYNCTASK_Raw(&MyStruct1, &FMyStruct::Hello, FString("Hello~"));
//		 SYNCTASK_SP(MyStructSP1.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
//		 SYNCTASK_UFunction(this, TEXT("T2"), 123, FString("T22222"));
//		 SYNCTASK_Lambda([](FString Mes)
//		 {
//			 ThreadP(Mes);
//		 }, "Lambda");
//	 }
//	 异步宏
//
//		 void AThreadTestCharacter::BeginPlay()
//	 {
//		 Super::BeginPlay();
//
//		 ASYNCTASK_UOBJECT(this, &AThreadTestCharacter::T1, 777);
//		 ASYNCTASK_Raw(&MyStruct1, &FMyStruct::Hello, FString("Hello~"));
//		 ASYNCTASK_SP(MyStructSP1.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
//		 ASYNCTASK_UFunction(this, TEXT("T2"), 123, FString("T22222"));
//		 ASYNCTASK_Lambda([](FString Mes)
//		 {
//			 ThreadP(Mes);
//		 }, "Lambda");
//	 }
//	 5.协程(FCoroutinesManage)
//		 该插件也加入了协程功能，同样也有Bind和Create的区别；
//
//		 Bind	XXX	绑定后可以设置时间，多久执行
//
//		 void AThreadTestCharacter::BeginPlay()
//	 {
//		 Super::BeginPlay();
//
//		 //设置1s后执行函数T1
//		 GThread::GetCoroutines().BindUObject(1.f, this, &AThreadTestCharacter::T1, 777);
//		 GThread::GetCoroutines().BindRaw(2.f, &MyStruct1, &FMyStruct::Hello, FString("Hello~"));
//		 GThread::GetCoroutines().BindSP(2.4f, MyStructSP1.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
//		 GThread::GetCoroutines().BindUFunction(4.f, this, TEXT("T2"), 123, FString("T22222"));
//		 GThread::GetCoroutines().BindLambda(7.f, [](FString Mes)
//		 {
//			 ThreadP(Mes);
//		 }, "Lambda");
//	 }
//	 CreateXXX	绑定完毕返回一个Handle, 由程序员来决定什么时候执行;
//
//	 TArray<FCoroutinesHandle> CoroutinesHandle;
//	 void AThreadTestCharacter::BeginPlay()
//	 {
//		 Super::BeginPlay();
//
//		 CoroutinesHandle.SetNum(5);
//		 CoroutinesHandle[0] = GThread::GetCoroutines().CreateUObject(this, &AThreadTestCharacter::T1, 777);
//		 CoroutinesHandle[1] = GThread::GetCoroutines().CreateRaw(&MyStruct1, &FMyStruct::Hello, FString("Hello~"));
//		 CoroutinesHandle[2] = GThread::GetCoroutines().CreateSP(MyStructSP1.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
//		 CoroutinesHandle[3] = GThread::GetCoroutines().CreateUFunction(this, TEXT("T2"), 123, FString("T22222"));
//		 CoroutinesHandle[4] = GThread::GetCoroutines().CreateLambda([](FString Mes)
//		 {
//			 ThreadP(Mes);
//		 }, "Lambda");
//
//		 GetWorld()->GetTimerManager().SetTimer(Handle, this, &AThreadTestCharacter::Do, 3.f);
//	 }
//
//	 //执行
//	 void AThreadTestCharacter::Do()
//	 {
//		 if (Handle.IsValid())
//		 {
//			 GetWorld()->GetTimerManager().ClearTimer(Handle);
//		 }
//
//		 for (auto &Tmp : CoroutinesHandle)
//		 {
//			 if (Tmp.IsValid())
//			 {
//				 //唤醒该协程下的事件
//				 Tmp.Pin()->Awaken();
//			 }
//		 }
//	 }
//	 6.Windows原生线程(FWindowsPlatformThread)
//		 void AThreadTestCharacter::BeginPlay()
//	 {
//		 Super::BeginPlay();
//
//		 FWindowsPlatformThread::RunDelegate.BindUObject(this, &AThreadTestCharacter::Run);
//		 FWindowsPlatformThread::CompletedDelegate.BindUObject(this, &AThreadTestCharacter::OK);
//		 FWindowsPlatformThread::Show();//执行线程
//	 }
//
//	 //正在执行
//	 void AThreadTestCharacter::Run()
//	 {
//		 ...
//	 }
//
//	 //执行完成
//	 void AThreadTestCharacter::OK()
//	 {
//		 ThreadP(TEXT("Windows Run"));
//	 }
//
//
//	 7.异步资源读取(FResourceLoadingManage)
//		 异步资源读取我们分为同步读取和异步读取，我们来看看它的使用方法
//
//		 异步
//
//		 UCLASS(config = Game)
//		 class AThreadTestCharacter : public ACharacter
//	 {
//		 GENERATED_BODY()
//
//	 public:
//		 //从蓝图加载资源路径
//		 UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
//			 TArray<FSoftObjectPath> ObjectPath;
//	 }
//
//	 void AThreadTestCharacter::BeginPlay()
//	 {
//		 Super::BeginPlay();
//
//		 auto La = [](TSharedPtr<FStreamableHandle> *InHandle)
//		 {
//			 TArray<UObject *> ExampleObject;
//			 (*InHandle)->GetLoadedAssets(ExampleObject);
//
//			 for (UObject *Tmp : ExampleObject)
//			 {
//				 ThreadP(Tmp->GetName());
//			 }
//		 };
//
//		 //异步使用方法
//		 GThread::GetResourceLoading() >> ObjectPath;
//		 StreamableHandle = GThread::GetResourceLoading().CreateLambda(La, &StreamableHandle);
//	 }
//	 同步
//
//		 UCLASS(config = Game)
//		 class AThreadTestCharacter : public ACharacter
//	 {
//		 GENERATED_BODY()
//	 public:
//		 //从蓝图加载资源路径
//		 UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
//			 TArray<FSoftObjectPath> ObjectPath;
//	 }
//
//	 void AThreadTestCharacter::BeginPlay()
//	 {
//		 Super::BeginPlay();
//
//		 auto La = [](TSharedPtr<FStreamableHandle> *InHandle)
//		 {
//			 TArray<UObject *> ExampleObject;
//			 (*InHandle)->GetLoadedAssets(ExampleObject);
//
//			 for (UObject *Tmp : ExampleObject)
//			 {
//				 ThreadP(Tmp->GetName());
//			 }
//		 };
//
//		 //同步
//		//////////////////////////////////////////////////////////////////////////
//		 StreamableHandle = GThread::GetResourceLoading() << ObjectPath;
//		 La(&StreamableHandle);
//	 }
//
//
//	 8.图表线程(ThreadGraphManage)
//		 图表线程是UE4使用频率最高的线程，它可以实现线程的前置任务，备受UE4线程喜爱，我们来演示一下该线程的使用技巧.
//
//		 同样该线程也是包含Bind和Create
//
//		 BindXXX : 只呼叫主线程
//
//		 TArray<FGraphEventRef > ArrayEventRef;
//	 void AThreadTestCharacter::BeginPlay()
//	 {
//		 Super::BeginPlay();
//
//		 ArrayEventRef.SetNum(5);
//		 ArrayEventRef[0] = GThread::GetGraph().BindUObject(this, &AThreadTestCharacter::T1, 777);
//		 ArrayEventRef[1] = GThread::GetGraph().BindRaw(&MyStruct1, &FMyStruct::Hello, FString("Hello~"));
//		 ArrayEventRef[2] = GThread::GetGraph().BindSP(MyStructSP1.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
//		 ArrayEventRef[3] = GThread::GetGraph().BindUFunction(this, TEXT("T2"), 123, FString("T22222"));
//		 ArrayEventRef[4] = GThread::GetGraph().BindLambda([](FString Mes)
//		 {
//			 ThreadP(Mes);
//		 }, "Lambda");
//
//		 //GThread::GetGraph().Wait(ArrayEventRef[0]);
//		 FGraphEventArray ArrayEvent;
//		 for (auto &Tmp : ArrayEventRef)
//		 {
//			 ArrayEvent.Add(Tmp);
//		 }
//		 //可以设置等待这些线程完成任务后再执行自己
//		 GThread::GetGraph().Wait(ArrayEvent);
//		 ThreadP("Wait-oK");
//	 }
//	 CreateXXX 绑定任意线程
//
//		 void AThreadTestCharacter::BeginPlay()
//	 {
//		 Super::BeginPlay();
//
//		 GThread::GetGraph().CreateUObject(this, &AThreadTestCharacter::T1, 777);
//		 GThread::GetGraph().CreateRaw(&MyStruct1, &FMyStruct::Hello, FString("Hello~"));
//		 GThread::GetGraph().CreateSP(MyStructSP1.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
//		 GThread::GetGraph().CreateUFunction(this, TEXT("T2"), 123, FString("T22222"));
//		 GThread::GetGraph().CreateLambda([](FString Mes)
//		 {
//			 ThreadP(Mes);
//		 }, "Lambda");
//	 }
//	 同样它也可以返回事件引用，方便其他图表线程等待该任务
//
//
//
//		 9.其他图表线程
//		 除此之外还有宏类型的图表线程，它的使用更加强大，可以对单个事件进行等待，我们来演示一下
//
//		 TArray<FGraphEventRef > ArrayEventRef;
//	 void AThreadTestCharacter::BeginPlay()
//	 {
//		 Super::BeginPlay();
//
//		auto A =  CALL_THREAD_UOBJECT(NULL, ENamedThreads::AnyThread, this, &AThreadTestCharacter::T1, 777);
//       //等待A
//		auto B =  CALL_THREAD_Raw(A, ENamedThreads::AnyThread, &MyStruct1, &FMyStruct::Hello, FString("Hello~"));
//		auto C =  CALL_THREAD_SP(NULL, ENamedThreads::AnyThread, MyStructSP1.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
//		auto D =  CALL_THREAD_UFunction(NULL, ENamedThreads::AnyThread, this, TEXT("T2"), 123, FString("T22222"));
//		auto E =  CALL_THREAD_Lambda(, NULL, ENamedThreads::AnyThread, [](FString Mes)
//		 {
//			 ThreadP(Mes);
//		 }, "Lambda");
//	 }
//
//
//	 好，以上是UE4SimpleThread插件的详细教程，希望该教程能对您有帮助
#pragma endregion Course

#include "CoreMinimal.h"
#include "Core/Manage/ThreadProxyManage.h"
#include "Core/Manage/ThreadTaskManage.h"
#include "Core/Manage/ThreadAbandonableManage.h"
#include "Core/Manage/CoroutinesManage.h"
#include "Core/Manage/ResourceLoadingManage.h"
#include "Core/Manage/ThreadGraphManage.h"
#include "Tickable.h"

//线程的总管理
namespace TM
{
	//FThreadManagement 是线程安全的，不容易出现死锁
	class SIMPLETHREAD_API FThreadManagement : public TSharedFromThis<FThreadManagement>, public FTickableGameObject
	{
	public:
		static TSharedRef<FThreadManagement> Get();

		//需要手动删除该单例
		static void Destroy();

		//~Tick
	private:
		//由主线程作为监视
		virtual void Tick(float DeltaTime);
		virtual TStatId GetStatId() const;

	public:
		static FThreadProxyManage		&GetProxy() { return Get()->ThreadProxyManage; }
		static FThreadTaskManagement	&GetTask() { return Get()->ThreadTaskManagement; }
		static FThreadAbandonableManage &GetAbandonable() { return Get()->ThreadAbandonableManage; }
		static FCoroutinesManage		&GetCoroutines() { return Get()->CoroutinesManage; }
		static FThreadGraphManage		&GetGraph() { return Get()->ThreadGraphManage; }
		static FResourceLoadingManage	&GetResourceLoading() { return Get()->ResourceLoadingManage; }

	protected:
		//自定义线程创建，可以简单直接的创建线程
		FThreadProxyManage ThreadProxyManage;

		//自定义的线程池，可以往线程池内丢任务，令其执行
		FThreadTaskManagement ThreadTaskManagement;

		//从UE4线程池内直接取线程执行
		FThreadAbandonableManage ThreadAbandonableManage;

		//协程
		FCoroutinesManage CoroutinesManage;

		//线程图表
		FThreadGraphManage ThreadGraphManage;

		//资源读取
		FResourceLoadingManage ResourceLoadingManage;
	private:
		static TSharedPtr<FThreadManagement> ThreadManagement;
	};
}
using namespace TM;

typedef TM::FThreadManagement GThread;