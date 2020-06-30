// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleArchivesList.generated.h"

USTRUCT()
struct SIMPLEARCHIVES_API FGameArchivesThumbnail
{
	GENERATED_USTRUCT_BODY()

		FGameArchivesThumbnail();

	UPROPERTY(Transient)
		UTexture2D* GameThumbnail;

	UPROPERTY(SaveGame)
		FString ScrPath;

	void InitResources();
	void ReleaseResources();
protected:
	void LoadTexture2D(const FString& ImagePath);
};

USTRUCT()
struct SIMPLEARCHIVES_API FSaveSlot
{
	GENERATED_USTRUCT_BODY()

	FSaveSlot();

	UPROPERTY(SaveGame)
		FGameArchivesThumbnail GameThumbnail;

	//存储的内容和日期
	UPROPERTY(SaveGame)
		FText DateText;

	//关卡名称
	UPROPERTY(SaveGame)
		FText LevelName;

	//章节名称
	UPROPERTY(SaveGame)
		FText ChapterName;

	//是否存档
	UPROPERTY(SaveGame)
		bool bSave;

	void Init();
};

USTRUCT()
struct SIMPLEARCHIVES_API FSaveSlotList
{
	GENERATED_USTRUCT_BODY()

		FSaveSlotList();

	//由外部调用
	void InitSlot();

	UPROPERTY(SaveGame)
		TMap<int32, FSaveSlot> Slots;

	UPROPERTY(SaveGame)
		TMap<int32, float> DegreeOfCompletion;

	int32 GetSerialNumber();

	bool RemoveAtGameData(int32 SlotNumber);
	bool AddGameDataByNumber(int32 SlotNumber);
};
