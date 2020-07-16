// Fill out your copyright notice in the Description page of Project Settings.


#include "TestThreadCharacter.h"
#include "ThreadManage.h"
#include "Windows/WindowsPlatformThread.h"
#include "Core/ThreadCoreMacro.h"
#include "Engine/StreamableManager.h"

// Sets default values
ATestThreadCharacter::ATestThreadCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FCriticalSection Mutex;
TArray<FThreadHandle> ThreadHandle;

void ThreadP(const FString Mes)
{
	{
		FScopeLock ScopeLock(&Mutex);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Red, *Mes);
		}
	}

	FPlatformProcess::Sleep(1.f);
}

void ATestThreadCharacter::T1(int32 i)
{
	ThreadP(FString::Printf(TEXT("T1 : %i"), i));
}

void ATestThreadCharacter::T2(int32 i, FString Mes)
{
	ThreadP(FString::Printf(TEXT("T2 : %i ,Mes = %s"), i, *Mes));
}

void ATestThreadCharacter::Do()
{
	if (Handle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(Handle);
	}
	////同步执行
	//for (auto &Tmp : ThreadHandle)
	//{
	//	GThread::GetProxy().Join(Tmp);
	//}
	//
	////异步执行
	//for (auto &Tmp : ThreadHandle)
	//{
	//	GThread::GetProxy().Detach(Tmp);
	//}
	//for (auto &Tmp : CoroutinesHandle)
	//{
	//	if (Tmp.IsValid())
	//	{
	//		//唤醒该协程下的事件
	//		Tmp.Pin()->Awaken();
	//	}
	//}
}

//结构体
struct FMyStruct
{
	void Hello(FString Mes)
	{
		ThreadP(FString::Printf(TEXT("FMyStruct::Hello : %s"), *Mes));
	}
};

//智能指针
struct FMyStructSP :public TSharedFromThis<FMyStructSP>
{
	void HelloSP(FString Mes)
	{
		ThreadP(FString::Printf(TEXT("FMyStructSP::Hello : %s"), *Mes));
	}
};

FMyStruct MyStruct;
TSharedPtr<FMyStructSP> MyStructSP = MakeShareable(new FMyStructSP);
TArray<FGraphEventRef> ArrayEventRef;

void ATestThreadCharacter::Run()
{
	//{
	//	GThread::GetGraph().CreateUObject(this, &ATestThreadCharacter::T1, 777);
	//	GThread::GetGraph().CreateRaw(&MyStruct, &FMyStruct::Hello, FString("Hello~"));
	//	GThread::GetGraph().CreateSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
	//	GThread::GetGraph().CreateUFunction(this, TEXT("T2"), 123, FString("T22222"));
	//	GThread::GetGraph().CreateLambda([](FString Mes)
	//	{
	//		ThreadP(Mes);
	//	}, "Lambda");
	//}

	//{
	//	ArrayEventRef.SetNum(5);
	//	ArrayEventRef[0] = GThread::GetGraph().BindUObject(this, &ATestThreadCharacter::T1, 777);
	//	ArrayEventRef[1] = GThread::GetGraph().BindRaw(&MyStruct, &FMyStruct::Hello, FString("Hello~"));
	//	ArrayEventRef[2] = GThread::GetGraph().BindSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
	//	ArrayEventRef[3] = GThread::GetGraph().BindUFunction(this, TEXT("T2"), 123, FString("T22222"));
	//	ArrayEventRef[4] = GThread::GetGraph().BindLambda([](FString Mes)
	//	{
	//		ThreadP(Mes);
	//	}, "Lambda");

	//	//GThread::GetGraph().Wait(ArrayEventRef[0]);

	//	FGraphEventArray ArrayEvent;
	//	for (auto &Tmp : ArrayEventRef)
	//	{
	//		ArrayEvent.Add(Tmp);
	//	}
	//	GThread::GetGraph().Wait(ArrayEvent);
	//	ThreadP(TEXT("Wait ok"));
	//}

	{
		ArrayEventRef.SetNum(5);
		CALL_THREAD_UOBJECT(NULL, ENamedThreads::AnyThread, this, &ATestThreadCharacter::T1, 777);
		CALL_THREAD_Raw(ArrayEventRef[0], ENamedThreads::AnyThread, &MyStruct, &FMyStruct::Hello, FString("Hello~"));
		CALL_THREAD_SP(NULL, ENamedThreads::AnyThread, MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
		//CALL_THREAD_UFunction(NULL, ENamedThreads::AnyThread, this, TEXT("T2"), 123, FString("T22222"));
		CALL_THREAD_Lambda(NULL, ENamedThreads::AnyThread, [](FString Mes)
		{
			ThreadP(Mes);
		}, "Lambda");

	}
}

void ATestThreadCharacter::OK()
{
	ThreadP(TEXT("Windows Run"));
}

TArray<FCoroutinesHandle> CoroutinesHandle;
TSharedPtr<FStreamableHandle> StreamableHandle;
// Called when the game starts or when spawned
void ATestThreadCharacter::BeginPlay()
{
	Super::BeginPlay();
	{
		auto La = [](TSharedPtr<FStreamableHandle> *InHandle)
		 {
			 TArray<UObject *> ExampleObject;
			 (*InHandle)->GetLoadedAssets(ExampleObject);

			 for (UObject *Tmp : ExampleObject)
			 {
				 ThreadP(Tmp->GetName());
			 }
		 };

		 //同步
		//////////////////////////////////////////////////////////////////////////
		 StreamableHandle = GThread::GetResourceLoading() << ObjectPath;
		 La(&StreamableHandle);
	}
	return;

	{
		auto La = [](TSharedPtr<FStreamableHandle> *InHandle)
		{
			TArray<UObject *> ExampleObject;
			(*InHandle)->GetLoadedAssets(ExampleObject);
			
			for (UObject *Tmp : ExampleObject)
			{
				ThreadP(Tmp->GetName());
			}
		};
			
		//异步使用方法
		GThread::GetResourceLoading() >> ObjectPath;
		//StreamableHandle = GThread::GetResourceLoading() >> ObjectPath >> FSimpleDelegate::CreateLambda(La, StreamableHandle);
		StreamableHandle = GThread::GetResourceLoading().CreateLambda(La, &StreamableHandle);
	}
	return;

	//Windows原生线程(FWindowsPlatformThread)
	{
		FWindowsPlatformThread::RunDelegate.BindUObject(this, &ATestThreadCharacter::Run);
		FWindowsPlatformThread::CompletedDelegate.BindUObject(this, &ATestThreadCharacter::OK);
		FWindowsPlatformThread::Show();//执行线程
	}
	return;

	//	 CreateXXX	绑定完毕返回一个Handle, 由程序员来决定什么时候执行;
	{
		CoroutinesHandle.SetNum(5);
		CoroutinesHandle[0] = GThread::GetCoroutines().CreateUObject(this, &ATestThreadCharacter::T1, 777);
		CoroutinesHandle[1] = GThread::GetCoroutines().CreateRaw(&MyStruct, &FMyStruct::Hello, FString("Hello~"));
		CoroutinesHandle[2] = GThread::GetCoroutines().CreateSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
		CoroutinesHandle[3] = GThread::GetCoroutines().CreateUFunction(this, TEXT("T2"), 123, FString("T22222"));
		CoroutinesHandle[4] = GThread::GetCoroutines().CreateLambda([](FString Mes)
		{
			ThreadP(Mes);
		}, "Lambda");

		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ATestThreadCharacter::Do, 3.f);
	}

	//		 Bind	XXX	绑定后可以设置时间，多久执行
	{
		//设置1s后执行函数T1
		GThread::GetCoroutines().BindUObject(1.f, this, &ATestThreadCharacter::T1, 777);
		GThread::GetCoroutines().BindRaw(2.f, &MyStruct, &FMyStruct::Hello, FString("Hello~"));
		GThread::GetCoroutines().BindSP(2.4f, MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
		GThread::GetCoroutines().BindUFunction(4.f, this, TEXT("T2"), 123, FString("T22222"));
		GThread::GetCoroutines().BindLambda(7.f, [](FString Mes)
		{
			ThreadP(Mes);
		}, "Lambda");
	}

	//同步宏
	{
		SYNCTASK_UOBJECT(this, &ATestThreadCharacter::T1, 777);
		SYNCTASK_Raw(&MyStruct, &FMyStruct::Hello, FString("Hello~"));
		SYNCTASK_SP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
		SYNCTASK_UFunction(this, TEXT("T2"), 123, FString("T22222"));
		SYNCTASK_Lambda([](FString Mes)
		{
			ThreadP(Mes);
		}, "Lambda");
	}

	//异步宏
	{
		ASYNCTASK_UOBJECT(this, &ATestThreadCharacter::T1, 777);
		ASYNCTASK_Raw(&MyStruct, &FMyStruct::Hello, FString("Hello~"));
		ASYNCTASK_SP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
		ASYNCTASK_UFunction(this, TEXT("T2"), 123, FString("T22222"));
		ASYNCTASK_Lambda([](FString Mes)
		{
			ThreadP(Mes);
		}, "Lambda");
	}

	{
		GThread::GetAbandonable().CreateUObject(this, &ATestThreadCharacter::T1, 777);
		GThread::GetAbandonable().CreateRaw(&MyStruct, &FMyStruct::Hello, FString("Hello~"));
		GThread::GetAbandonable().CreateSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
		GThread::GetAbandonable().CreateUFunction(this, TEXT("T2"), 123, FString("T22222"));
		GThread::GetAbandonable().CreateLambda([](FString Mes)
		{
			ThreadP(Mes);
		}, "Lambda");
	}

	{
		GThread::GetAbandonable().BindUObject(this, &ATestThreadCharacter::T1, 777);
		GThread::GetAbandonable().BindRaw(&MyStruct, &FMyStruct::Hello, FString("Hello~"));
		GThread::GetAbandonable().BindSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
		GThread::GetAbandonable().BindUFunction(this, TEXT("T2"), 123, FString("T22222"));
		GThread::GetAbandonable().BindLambda([](FString Mes)
		{
			ThreadP(Mes);
		}, "Lambda");
	}

	{
		GThread::GetTask().CreateUObject(this, &ATestThreadCharacter::T1, 777);
		GThread::GetTask().CreateRaw(&MyStruct, &FMyStruct::Hello, FString("Hello~"));
		GThread::GetTask().CreateSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
		GThread::GetTask().CreateUFunction(this, TEXT("T2"), 123, FString("T22222"));
		GThread::GetTask().CreateLambda([](FString Mes)
		{
			ThreadP(Mes);
		}, "Lambda");
	}

	{
		GThread::GetTask().BindUObject(this, &ATestThreadCharacter::T1, 777);
		GThread::GetTask().BindRaw(&MyStruct, &FMyStruct::Hello, FString("Hello~"));
		GThread::GetTask().BindSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
		GThread::GetTask().BindUFunction(this, TEXT("T2"), 123, FString("T22222"));
		GThread::GetTask().BindLambda([](FString Mes)
		{
			ThreadP(Mes);
		}, "Lambda");
	}

	{
		ThreadHandle.SetNum(5);
		//FMyStruct MyStruct;
		//TSharedPtr<FMyStructSP> MyStructSP = MakeShareable(new FMyStructSP);

		ThreadHandle[0] = GThread::GetProxy().BindUObject(this, &ATestThreadCharacter::T1, 777);
		ThreadHandle[1] = GThread::GetProxy().BindRaw(&MyStruct, &FMyStruct::Hello, FString("Hello~"));
		ThreadHandle[2] = GThread::GetProxy().BindSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
		ThreadHandle[3] = GThread::GetProxy().BindUFunction(this, TEXT("T2"), 123, FString("T22222"));
		ThreadHandle[4] = GThread::GetProxy().BindLambda([](FString Mes)
		{
			ThreadP(Mes);
		}, "Lambda");


		GetWorld()->GetTimerManager().SetTimer(Handle, this, &ATestThreadCharacter::Do, 3.f);
	}

	{
		//FMyStruct MyStruct;
		//TSharedPtr<FMyStructSP> MyStructSP = MakeShareable(new FMyStructSP);

		GThread::GetProxy().CreateUObject(this, &ATestThreadCharacter::T1, 777);
		GThread::GetProxy().CreateRaw(&MyStruct, &FMyStruct::Hello, FString("Hello~"));
		GThread::GetProxy().CreateSP(MyStructSP.ToSharedRef(), &FMyStructSP::HelloSP, FString("HelloSP~"));
		GThread::GetProxy().CreateUFunction(this, TEXT("T2"), 123, FString("T22222"));
		GThread::GetProxy().CreateLambda([](FString Mes)
		{
			ThreadP(Mes);
		}, "Lambda");

	}
}

void ATestThreadCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GThread::Destroy();
}

// Called every frame
void ATestThreadCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestThreadCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

