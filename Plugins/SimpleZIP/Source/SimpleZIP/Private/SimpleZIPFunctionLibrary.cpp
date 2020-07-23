#include "SimpleZIPFunctionLibrary.h"
#include "SimpleCompress.h"
#include "ZIPSDK.h"

bool USimpleZIPFunctionLibrary::Compress(const TArray<uint8> &UncompressedData, TArray<uint8> &OutData)
{
	SimpleZIP::FCompressAssist CompressAssist(OutData);
	return CompressAssist.Compress(UncompressedData);
}

bool USimpleZIPFunctionLibrary::Decompress(const TArray<uint8> &CompressedData, TArray<uint8> &OutData)
{
	SimpleZIP::FCompressAssist CompressAssist(OutData);
	return CompressAssist.Decompress(CompressedData);
}

bool USimpleZIPFunctionLibrary::ZipByLoacl(const FString &SrcPath, const FString &DestFilename, const FString &Password)
{
	return SimpleZIP::ZipByLoacl(*SrcPath, *DestFilename, *Password);
}

bool USimpleZIPFunctionLibrary::UnzipByLoacl(const FString &SrcPath, const FString &DestFilename, const FString &Password, bool bOverwrite /*= true*/)
{
	return SimpleZIP::UnzipByLoacl(*SrcPath, *DestFilename, *Password,bOverwrite);
}
