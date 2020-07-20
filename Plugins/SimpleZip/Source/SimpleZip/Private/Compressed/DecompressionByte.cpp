#include "Compressed/DecompressionByte.h"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"
#include "XmlFile.h"
#include "PakInfo.h"
#include "Misc/FileHelper.h"
#include "Misc/AES.h"

#if PLATFORM_WINDOWS
#pragma optimize("", off)
#endif


bool SimpleZip::FDecompressedByte::DecompressedData(
	const FString & ReadPath, 
	const FString & SavaPath, 
	EEncryptionMode EncryptionMode /*= EEncryptionMode::NONE*/, 
	const FString &Keys /*= ""*/)
{
	TMap<FString, FString> PakFileData;
	TArray<FString> ALlPakPaths;

	IFileManager::Get().FindFilesRecursive(ALlPakPaths, *ReadPath, TEXT("*"), true, true);

	for (const auto &Tmp : ALlPakPaths)
	{
		if (!Tmp.Contains("."))
		{
			FString HashName = FPaths::GetCleanFilename(Tmp);
			PakFileData.Add(HashName, Tmp);
		}
	}

	//解析数据
	FXmlFile *XMLFile = new FXmlFile(ReadPath/TEXT("DependencyPakFile.xml"));
	FXmlNode *RootNode = XMLFile->GetRootNode();
	if (RootNode)
	{
		const TArray<FXmlNode*> NodeFiles = RootNode->GetChildrenNodes();
		for (const FXmlNode *NodeFile : NodeFiles)
		{
			FString PakPaths = NodeFile->GetAttribute(TEXT("Name"));
			int32 ByteTotalSize = FCString::Atoi(*(NodeFile->GetAttribute(TEXT("ByteTotalSize"))));
			const TArray<FXmlNode*> Blobs = NodeFile->GetChildrenNodes();
			TMap<FString, SimpleZip::FByteInfo> ByteMap;
			for (const FXmlNode *Blob : Blobs)
			{
				FString Hash = Blob->GetAttribute(TEXT("Hash"));
				int32 ByteBegin = FCString::Atoi(*(Blob->GetAttribute(TEXT("ByteBegin"))));
				int32 ByteEnd = FCString::Atoi(*(Blob->GetAttribute(TEXT("ByteEnd"))));
				ByteMap.Add(Hash, SimpleZip::FByteInfo(ByteBegin, ByteEnd));
			}
			//读取字节码 并且整合成相应文件

			TArray<uint8> ByteData;
			for (const auto &Tmp : ByteMap)
			{
				TArray<uint8> TmpByteData;
				FFileHelper::LoadFileToArray(TmpByteData, *(ReadPath / Tmp.Key));
				if (EncryptionMode == EEncryptionMode::AES)
				{
					if (Keys != "")
					{
						AESDecrypData(TmpByteData, Keys);
					}
					else
					{
						AESDecrypData(TmpByteData, Tmp.Key);
					}					
				}
				

				for (uint32 i = Tmp.Value.ByteBegin; i <= Tmp.Value.ByteEnd; i++)
				{
					if (TmpByteData.IsValidIndex(i))
					{
						ByteData.Add(TmpByteData[i]);
					}
				}
			}

			FFileHelper::SaveArrayToFile(ByteData, *(SavaPath / PakPaths));
		}
	}

	delete XMLFile;
	return 0;
}

bool SimpleZip::FDecompressedByte::AESDecrypData(TArray<uint8> &RawData, const FString &Keys)
{
	if (RawData.Num() % FAES::AESBlockSize == 0)
	{
		FAES::DecryptData(RawData.GetData(), RawData.Num(), TCHAR_TO_ANSI(*Keys));

		return true;
	}

	return false;
}

#if PLATFORM_WINDOWS
#pragma optimize("", on)
#endif