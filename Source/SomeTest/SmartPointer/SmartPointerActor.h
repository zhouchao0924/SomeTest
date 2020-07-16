// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmartPointerActor.generated.h"

//接口
class IMyID
{
	IMyID()
	{
		ID = FMath::RandRange(100, 100000);
	}

private:
	uint64 ID;
};

//数据
class FData : public IMyID
{
public:
	float Health;
	uint8 bDeath : 1;

	FName PlayerName;
};

//数据管理
class DataManage : public TSharedFromThis<DataManage>
{
public:
	static TSharedRef<DataManage> Get()
	{
		if (m_Manage.IsValid())
		{
			m_Manage = MakeShareable(new DataManage());
		}
		return m_Manage->AsShared();
	}
private:
	static TSharedPtr<DataManage> m_Manage;
	TMap<uint64, TSharedPtr<FData>> MyData;
};

//角色实例
class FCharacter
{
private:
	TWeakPtr<FData> NewData;
};

class FProjectSettings : public TSharedFromThis<FProjectSettings>
{
public:
	void print();
};

class Base
{

};

class Child :public Base
{
public:
	void print();
};

class Task : public TSharedFromThis<Task>//与UObject不兼容
{
public:
	int a;
	static TSharedRef<Task> Get()
	{
		if (m_Task.IsValid())
		{
			m_Task = MakeShareable(new Task());
		}
		return m_Task->AsShared();
	}
private:
	static TSharedPtr<Task> m_Task;
};

class TaskB
{
public:

private:
	static TSharedPtr<TaskB> m_Task;
};

class TreeNode
{
	TArray<TSharedPtr<TreeNode>> Children;
	TWeakPtr<TreeNode> Parent;
};

UCLASS()
class SOMETEST_API ASmartPointerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmartPointerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TSharedPtrTask();

	void TSharedRefTask();

	void TSharedRef2Ptr();

	void TWeakPtrTask();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TSharedPtr<Task> Task_a;
	
	TWeakPtr<Task> Task_c;
};
