// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

DECLARE_DELEGATE_OneParam(FSimpleProcessDelegate,int32);

enum class ESimpleSuMenuType :uint8
{
	CONTENT,
	ASSET
};
enum class EQueueTaskType :uint8
{
	PAK,
	COOK
};

struct FQueueTaskInterface
{
	FQueueTaskInterface()
		:bMissionAccomplished(false)
		,ID(FGuid::NewGuid())
	{

	}
	virtual ~FQueueTaskInterface(){}

	virtual void Tick() = 0;

	virtual EQueueTaskType GetQueueTaskType() const  = 0;

	virtual void Add(const FProcHandle &Handle) = 0;

	FSimpleProcessDelegate MissionAccomplished;

	bool bMissionAccomplished;

	bool operator==(const FQueueTaskInterface &QueueTask)
	{
		return ID == QueueTask.ID;
	}

private:
	FGuid ID;
};

struct FPakQueueTask :public FQueueTaskInterface
{
	FPakQueueTask()
		:FQueueTaskInterface()
	{}
	virtual void Tick();
	virtual EQueueTaskType GetQueueTaskType()const {return EQueueTaskType::PAK;}
	virtual void Add(const FProcHandle &Handle){PakHandles.Add(Handle);}

	TArray<FProcHandle> PakHandles;
};

struct FCookQueueTask :public FQueueTaskInterface
{
	FCookQueueTask()
		:FQueueTaskInterface()
	{}

	virtual void Tick();
	virtual EQueueTaskType GetQueueTaskType()const { return EQueueTaskType::COOK; }
	virtual void Add(const FProcHandle &Handle) { CookHandles = Handle; }

	FProcHandle CookHandles;
};
