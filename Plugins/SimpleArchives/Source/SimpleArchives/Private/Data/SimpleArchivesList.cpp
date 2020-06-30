#include "Data/SimpleArchivesList.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Modules/ModuleManager.h"
#include "Engine/Texture2D.h"

#define LOCTEXT_NAMESPACE "SaveSlot"

FSaveSlot::FSaveSlot()
{
	Init();
}

void FSaveSlot::Init()
{
	GameThumbnail.ReleaseResources();
	DateText = LOCTEXT("SaveSlot", " ");
	LevelName = LOCTEXT("LevelName", " ");
	ChapterName = LOCTEXT("ChapterName", " ");
	bSave = false;
}

FSaveSlotList::FSaveSlotList()
{

}

void FSaveSlotList::InitSlot()
{
	for (int32 i = 0; i < 20; i++)
	{
		Slots.Add(i, FSaveSlot());
	}

	for (int32 i = 1; i < 6; i++)
	{
		DegreeOfCompletion.Add(i, 0.0f);
	}
}

int32 FSaveSlotList::GetSerialNumber()
{
	for (auto &Tmp : Slots)
	{
		if (!Tmp.Value.bSave)
		{
			return Tmp.Key;
		}
	}

	return INDEX_NONE;
}

bool FSaveSlotList::RemoveAtGameData(int32 SlotNumber)
{
	if (Slots.Contains(SlotNumber))
	{
		Slots[SlotNumber].bSave = false;
	}

	return false;
}

bool FSaveSlotList::AddGameDataByNumber(int32 SlotNumber)
{
	if (Slots.Contains(SlotNumber))
	{
		Slots[SlotNumber].bSave = true;
	}

	return false;
}

FGameArchivesThumbnail::FGameArchivesThumbnail()
	:GameThumbnail(nullptr)
{

}


void FGameArchivesThumbnail::InitResources()
{
	if (!ScrPath.IsEmpty())
	{
		if (!GameThumbnail)
		{
			LoadTexture2D(ScrPath);
		}
	}
}

void FGameArchivesThumbnail::ReleaseResources()
{
	//删除我们的缩略图 然后清除我们的路径
	if (!ScrPath.IsEmpty())
	{
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*ScrPath))
		{
			FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*ScrPath);
		}

		GameThumbnail = nullptr;
		ScrPath.Empty();
	}
}

void FGameArchivesThumbnail::LoadTexture2D(const FString& ImagePath)
{
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*ImagePath))
	{
		TArray<uint8> CompressedData;
		if (FFileHelper::LoadFileToArray(CompressedData, *ImagePath))
		{
			IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));

			TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
			if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(CompressedData.GetData(), CompressedData.Num()))
			{
				const TArray<uint8>* UncompressedRGBA = nullptr;
				if (ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, UncompressedRGBA))
				{
					GameThumbnail = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_R8G8B8A8);
					if (GameThumbnail != nullptr)
					{
						GameThumbnail->UpdateResource();
					}
				}
			}

			ImageWrapper.Reset();
		}
	}
}

#undef LOCTEXT_NAMESPACE

