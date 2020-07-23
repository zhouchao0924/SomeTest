// Copyright (C) RenZhai.2020.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/FileHelper.h"
#include "FileHelperType.generated.h"


UENUM()
enum class ESimpleHashOptions :uint8
{
	None = 0,
	/** Enable the async task for verifying the hash for the file being loaded */
	EnableVerify,
	/** A missing hash entry should trigger an error */
	ErrorMissingHash,
};

UENUM()
enum class ESimpleEncodingOptions :uint8
{
	AutoDetect,
	ForceAnsi,
	ForceUnicode,
	ForceUTF8,
	ForceUTF8WithoutBOM
};

namespace SimpleFileHelper
{
	FFileHelper::EHashOptions ToHashOptions(ESimpleHashOptions HashOptions);
};