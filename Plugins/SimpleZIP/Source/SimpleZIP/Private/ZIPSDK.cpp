#include "ZIPSDK.h"
#include "SimpleZIPLibrary/ZIP_SDK_Library/ZIP_SDK_Library.h"
#include "ZIPType.h"

bool SimpleZIP::ZipByLoacl(const TCHAR *SrcPath, const TCHAR *DestFilename, const TCHAR *Password)
{
	auto Result = zip_file_by_loacl(TCHAR_TO_ANSI(SrcPath), TCHAR_TO_ANSI(DestFilename), TCHAR_TO_ANSI(Password));
	
	SimpleZIP::CollectErrorInformation(Result);
	return Result == SIMPLE_ZIP_OK;
}

bool SimpleZIP::UnzipByLoacl(const TCHAR *SrcPath, const TCHAR *DestFilename, const TCHAR *Password, bool bOverwrite /*= true*/)
{
	auto Result = unzip_file_by_loacl(TCHAR_TO_ANSI(SrcPath), TCHAR_TO_ANSI(DestFilename), TCHAR_TO_ANSI(Password), bOverwrite);

	SimpleZIP::CollectErrorInformation(Result);
	return Result == SIMPLE_ZIP_OK;
}

