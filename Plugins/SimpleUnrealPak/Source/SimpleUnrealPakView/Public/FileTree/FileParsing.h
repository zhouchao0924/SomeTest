// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

namespace SimpleUnrealPakView
{
	enum class EFileType :uint8
	{
		FILE,
		FOLDER,
	};

	struct FFileList
	{
		//如果Filename是文件那么它保留的是一个Packagename = /Game/SS/A
		//如果Filename是文件夹那么它保留的是一个文件夹名称 比如 Game
		FString Filename;
		TArray<FFileList> Children;

		FORCEINLINE friend bool operator==(const FFileList& A, const FFileList& B)
		{
			return A.Filename == B.Filename;
		}

		EFileType GetFileType()const;
	};

	void FilesParsing(const TArray<FString> &Filenames, FFileList &List);
}