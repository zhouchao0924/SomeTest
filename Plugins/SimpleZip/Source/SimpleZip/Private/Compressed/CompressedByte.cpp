#include "Compressed/CompressedByte.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "PakInfo.h"
#include "Math/UnrealMathSSE.h"
#include "XmlParser/Public/XmlFile.h"
#include "Core/SimpleXML.h"
#include "Misc/AES.h"

typedef TMap<SimpleZip::FPakHash, TArray<uint8>> TSimpleHashMapPakData;

void RecursionAddPakInfo(
	TArray<uint8> &InByteData, 
	TSimpleHashMapPakData &PakData, 
	SimpleZip::TSimpleHashMapPakInfo &PakInfo,
	int64 FragmentSize)
{
	SimpleZip::FPakHash *HashPoint = nullptr;
	SimpleZip::FByteInfo ByteInfo;

	if (InByteData.Num() <= FragmentSize)
	{
		for (auto &Tmp : PakData)
		{
			//判断有没有空间可以储存我们的数据
			int64 SurplusNum = FragmentSize - Tmp.Value.Num();
			if (InByteData.Num() <= SurplusNum)
			{
				ByteInfo.ByteBegin = Tmp.Value.Num();
				Tmp.Value.Append(InByteData);
				ByteInfo.ByteEnd = Tmp.Value.Num() - 1;
				HashPoint = &Tmp.Key;
				break;
			}
		}
		//第一次添加
		if (HashPoint == nullptr)
		{
			ByteInfo.ByteEnd = InByteData.Num() - 1;
			SimpleZip::FPakHash HashObject = FGuid::NewGuid();
			PakData.Add(HashObject, InByteData);
			HashPoint = const_cast<SimpleZip::FPakHash *>(PakData.FindKey(InByteData));
		}
		PakInfo.Add(*HashPoint, ByteInfo);
	}
	else
	{
		TArray<uint8> HalfData;
		TArray<uint8> SurplusData;
		for (int32 i = 0; i < InByteData.Num(); i++)
		{
			if (i <= FragmentSize)
			{
				HalfData.Add(InByteData[i]);
			}
			else
			{
				SurplusData.Add(InByteData[i]);
			}
		}
		ByteInfo.ByteEnd = FragmentSize;
		SimpleZip::FPakHash HashObject = FGuid::NewGuid();
		PakData.Add(HashObject, HalfData);
		HashPoint = const_cast<SimpleZip::FPakHash *>(PakData.FindKey(HalfData));
		PakInfo.Add(*HashPoint, ByteInfo);

		//需要释放掉数据
		InByteData.Empty();
		RecursionAddPakInfo(SurplusData, PakData, PakInfo, FragmentSize);
	}
}

bool SavePakInfoToXMLFile(const FString &Path, const TArray<SimpleZip::FPakInfo> &PakInfo, const FString &BaseURL)
{
	FString XMLArray;
	for (auto &Tmp : PakInfo)
	{
		FString BagMap;
		for (const auto &BagTmp : Tmp.StickyByte)
		{
			FString BagElement;
			//拿到属性
			FString BagAttribute;
			BagAttribute.Append(
				TEXT("Hash=\"" + BagTmp.Key.ToString()+ TEXT("\" ")) +
				TEXT("ByteBegin=\"" + FString::FromInt(BagTmp.Value.ByteBegin) + TEXT("\" ")) +
				TEXT("ByteEnd=\"" + FString::FromInt(BagTmp.Value.ByteEnd) + TEXT("\""))
			);
			SimpleXML::Write(TEXT("Blob"), BagAttribute, "", BagElement);
			BagMap.Append(BagElement);
		}

		FString XMLElement;

		FString Attribute;
		Attribute.Append(
			TEXT("Name=\"" + Tmp.RelativePath + TEXT("\" ")) +
			TEXT("ByteTotalSize=\"" + FString::FromInt(Tmp.ByteTotalSize) + TEXT("\""))
		);
		SimpleXML::Write(TEXT("File"), Attribute, BagMap, XMLElement, true);

		XMLArray.Append(XMLElement);
	}
	
	FString DependencyXML;
	SimpleXML::Write(TEXT("DependencyPak"), FString::Printf(TEXT("BaseURL=\"%s\""), *BaseURL), XMLArray, DependencyXML, true);
	FXmlFile *DependencyXMLFile = new FXmlFile(DependencyXML, EConstructMethod::ConstructFromBuffer);
	bool SavaSuccess = DependencyXMLFile->Save(Path/TEXT("DependencyPakFile.xml"));
	delete DependencyXMLFile;

	return SavaSuccess;
}

bool SimpleZip::FCompressedByte::CompressedData(
	const FString & ReadPath, 
	const FString & SavaPath, 
	const FString &BaseURL /*= ""*/, 
	int64 FragmentSize /*= BYTE_30M*/, 
	EEncryptionMode EncryptionMode /*= EEncryptionMode::NONE*/, 
	const FString &Keys /*= ""*/)
{
	TArray<FString> AllPaths;

	IFileManager::Get().FindFilesRecursive(AllPaths, *ReadPath, TEXT("*"), true, true);

	//保存真正被切割好的数据
	TSimpleHashMapPakData PakData;

	TArray<SimpleZip::FPakInfo> PakInfo;
	for (auto &TmpPath : AllPaths)
	{
		TArray<uint8> ByteData;
		FFileHelper::LoadFileToArray(ByteData, *TmpPath);
		if (ByteData.Num())
		{
			PakInfo.Add(SimpleZip::FPakInfo());
			TmpPath.RemoveFromStart(ReadPath / TEXT(""));
			PakInfo[PakInfo.Num() - 1].RelativePath = TmpPath;
			PakInfo[PakInfo.Num() - 1].ByteTotalSize = ByteData.Num();

			RecursionAddPakInfo(ByteData, PakData, PakInfo[PakInfo.Num() - 1].StickyByte, FragmentSize);
		}
		//储存Hash所对应的数据
		for (auto &Tmp : PakData)
		{
			if (EncryptionMode == EEncryptionMode::AES)
			{
				if (Keys != "")
				{
					AESEncrypData(Tmp.Value, Keys);
				}
				else
				{
					AESEncrypData(Tmp.Value, Tmp.Key.ToString());
				}
			}

			FFileHelper::SaveArrayToFile(Tmp.Value, *(SavaPath / Tmp.Key.ToString()));
		}

		//释放资源  主要是为了维持动态平衡
		TArray<SimpleZip::FPakHash*> RemovePakHash;
		for (auto &TmpPak : PakData)
		{
			if (FMath::Abs<int32>(TmpPak.Value.Num() - FragmentSize) < 4)
			{
				RemovePakHash.Add(&TmpPak.Key);
			}
		}

		for (auto &PakID : RemovePakHash)
		{
			PakData.Remove(*PakID);
		}
	}

	//储存XML
	return SavePakInfoToXMLFile(SavaPath, PakInfo, BaseURL);
}

void RescursionSupplementByte(TArray<uint8> &RawData)
{
	if (RawData.Num() % FAES::AESBlockSize)
	{
		uint8 NewChar = 0;
		RawData.Add(NewChar);
		RescursionSupplementByte(RawData);
	}
}

void SimpleZip::FCompressedByte::AESEncrypData(TArray<uint8> &RawData, const FString &Keys)
{
	RescursionSupplementByte(RawData);
	FAES::EncryptData(RawData.GetData(), RawData.Num(), TCHAR_TO_ANSI(*Keys));
}

