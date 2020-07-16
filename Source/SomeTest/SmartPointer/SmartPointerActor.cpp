// Fill out your copyright notice in the Description page of Project Settings.


#include "SmartPointerActor.h"

TSharedPtr<Task> Task::m_Task;

// Sets default values
ASmartPointerActor::ASmartPointerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASmartPointerActor::BeginPlay()
{
	Super::BeginPlay();
	
	TSharedPtr<Base> m_base = MakeShareable(new Child());
	TSharedPtr<Child> m_child = StaticCastSharedPtr<Child>(m_base);
	//StaticCastSharedRef
	if (m_child.IsValid())
	{
		m_child->print();
	}

	const TSharedPtr<Base> m_base_2 = MakeShareable(new Child());;
	TSharedPtr<Base> m_base_3 = ConstCastSharedPtr<Base>(m_base_2);//不能直接转换为基类
	TSharedPtr<Child> m_child_2 = StaticCastSharedPtr<Child>(m_base_3);
	if (m_child_2.IsValid())
	{
		m_child_2->print();
	}


}

void ASmartPointerActor::TSharedPtrTask()
{
	Task_a = MakeShareable(new Task());

	if (Task_a.IsValid() || Task_a.Get())
	{
		Task_a.Get()->a;
		Task_a.Reset();
	}
}

void ASmartPointerActor::TSharedRefTask()
{
	TSharedRef<Task> Task_b(new Task());
	Task_b->a;
	(*Task_b).a;
}

void ASmartPointerActor::TSharedRef2Ptr()
{
	TSharedRef<Task> Task_b(new Task());
	//Task_b = nullptr;//错误的！！！
	Task_a = Task_b;

	//普通转换为智能指针
	Task *NewTask = new Task();
	Task_a = MakeShareable(NewTask);

	//智能指针转换为共享引用
	Task_b = Task_a.ToSharedRef();//有断言
	
	//普通转换为共享引用
	Task *NewTask2 = new Task();
	TSharedRef<Task> Task_d = NewTask2->AsShared();

	Task::Get()->a;
}

void ASmartPointerActor::TWeakPtrTask()
{
	TSharedPtr<Task> ptr_Task = MakeShareable(new Task());
	TSharedRef<Task> ref_Task(new Task());

	TWeakPtr<Task> Task_d(ptr_Task);
	TWeakPtr<Task> Task_e(ref_Task);
	Task_c = Task_d;
	Task_c = Task_e;

	Task_c = nullptr;

	TSharedPtr<Task> NewTask(Task_c.Pin());
	if (NewTask.IsValid())
	{
		NewTask->a;
	}
}

// Called every frame
void ASmartPointerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void Child::print()
{

}

void FProjectSettings::print()
{

}

void NewMain()
{
	TSharedPtr<FProjectSettings> ProjectSettings = MakeShareable(new FProjectSettings());
	ProjectSettings->print();

	FProjectSettings *m_ProjectSettings = ProjectSettings.Get();//智能指针变为原生
	if (m_ProjectSettings)
	{
		m_ProjectSettings->AsShared();
	}
}