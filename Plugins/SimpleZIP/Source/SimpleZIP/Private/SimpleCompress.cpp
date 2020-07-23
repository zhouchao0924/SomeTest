#include "SimpleCompress.h"
#include "Misc/Compression.h"
#include "Serialization/MemoryWriter.h"

SimpleZIP::FCompressAssist::FCompressAssist(TArray<uint8> &InData)
	:Data(InData)
{

}

bool SimpleZIP::FCompressAssist::Compress(const TArray<uint8> &UncompressedData)
{
	int32 UncompressedSize = UncompressedData.Num() * UncompressedData.GetTypeSize();

	TArray<uint8> TempCompressedMemeory;
	TempCompressedMemeory.Empty(UncompressedSize * 4 / 3);
	TempCompressedMemeory.AddUninitialized(UncompressedSize * 4 / 3);

	int32 CompressSize = TempCompressedMemeory.Num() * TempCompressedMemeory.GetTypeSize();

	bool bSucceeded = FCompression::CompressMemory(
		NAME_Zlib,
		TempCompressedMemeory.GetData(),
		CompressSize,
		UncompressedData.GetData(),
		UncompressedSize,
		COMPRESS_BiasMemory);

	if (bSucceeded)
	{
		FMemoryWriter FinalArchive(Data, true);
		FinalArchive << UncompressedSize;
		FinalArchive << CompressSize;
		FinalArchive.Serialize(TempCompressedMemeory.GetData(), CompressSize);
	}

	return bSucceeded;
}

bool SimpleZIP::FCompressAssist::Decompress(const TArray<uint8> &CompressedData)
{
	const int32 BufferHeaderSize = (int32)(sizeof(int32) * 2);

	int32 UncompressedSize = ((int32*)CompressedData.GetData())[0];
	Data.SetNum(UncompressedSize);

	return FCompression::UncompressMemory(NAME_Zlib, Data.GetData(), Data.Num(), CompressedData.GetData() + BufferHeaderSize, CompressedData.Num() - BufferHeaderSize);
}

