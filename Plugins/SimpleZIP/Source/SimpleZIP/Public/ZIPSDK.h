// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace SimpleZIP
{
	bool ZipByLoacl(const TCHAR *SrcPath, const TCHAR *DestFilename, const TCHAR *Password);
	bool UnzipByLoacl(const TCHAR *SrcPath, const TCHAR *DestFilename, const TCHAR *Password, bool bOverwrite = true);
}