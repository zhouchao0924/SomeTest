#include "CompressedByte.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "PakInfo.h"
#include "Math/UnrealMathSSE.h"
#include "XmlParser/Public/XmlFile.h"
#include "SimpleXML.h"

typedef TMap<SimplePak::FPakHash,
	TArray<uint8>,
	FDefaultSetAllocator,
	SimplePak::TFPakHashMapKeyFuncs<TArray<uint8>>> TSimpleHashMapPakData;

void RecursionAddPakInfo(
	TArray<uint8> &InByteData, 
	TSimpleHashMapPakData &PakData, 
	SimplePak::TSimpleHashMapPakInfo &PakInfo,
	uint64 FragmentSize)
{
	SimplePak::FPakHash *HashPoint = nullptr;
	SimplePak::FByteInfo ByteInfo;

	if (InByteData.Num() <= FragmentSize)
	{
		for (auto &Tmp : PakData)
		{
			//判断有没有空间可以储存我们的数据
			uint64 SurplusNum = FragmentSize - Tmp.Value.Num();
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
			PakData.Add(SimplePak::FPakHash(), InByteData);
			HashPoint = const_cast<SimplePak::FPakHash *>(PakData.FindKey(InByteData));
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
		PakData.Add(SimplePak::FPakHash(), HalfData);
		HashPoint = const_cast<SimplePak::FPakHash *>(PakData.FindKey(HalfData));
		PakInfo.Add(*HashPoint, ByteInfo);

		//需要释放掉数据
		InByteData.Empty();
		RecursionAddPakInfo(SurplusData, PakData, PakInfo, FragmentSize);
	}
}

bool SavePakInfoToXMLFile(const FString &Path, const TArray<SimplePak::FPakInfo> &PakInfo)
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
				TEXT("Hash=\"" + BagTmp.Key.UniqueID + TEXT("\" ")) +
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
	SimpleXML::Write(TEXT("DependencyPak"), TEXT("BaseURL=\"smartuil.top\""), XMLArray, DependencyXML, true);
	FXmlFile *DependencyXMLFile = new FXmlFile(DependencyXML, EConstructMethod::ConstructFromBuffer);
	bool SavaSuccess = DependencyXMLFile->Save(Path/TEXT("DependencyPakFile.xml"));
	delete DependencyXMLFile;

	return SavaSuccess;
}

bool SimplePak::FCompressedByte::CompressedData(
	const FString & ReadPath, 
	const FString & SavaPath, 
	int64 FragmentSize /*= BYTE_30M*/, 
	uint8 Tmp /*= 1*/)
{
	TArray<FString> AllPaths;

	IFileManager::Get().FindFilesRecursive(AllPaths, *ReadPath, TEXT("*"), true, true);

	//保存真正被切割好的数据
	TSimpleHashMapPakData PakData;

	TArray<SimplePak::FPakInfo> PakInfo;
	for (auto &Tmp : AllPaths)
	{
		TArray<uint8> ByteData;
		FFileHelper::LoadFileToArray(ByteData, *Tmp);

		PakInfo.Add(SimplePak::FPakInfo());
		Tmp.RemoveFromStart(ReadPath / TEXT(""));
		PakInfo[PakInfo.Num() - 1].RelativePath = Tmp;
		PakInfo[PakInfo.Num() - 1].ByteTotalSize = ByteData.Num();

		RecursionAddPakInfo(ByteData, PakData, PakInfo[PakInfo.Num() - 1].StickyByte, FragmentSize);

		//释放资源  主要是为了维持动态平衡
		TArray<SimplePak::FPakHash*> RemovePakHash;
		for (auto &TmpPak : PakData)
		{
			if (FMath::IsNearlyEqual(TmpPak.Value.Num(), FragmentSize, 4))
			{
				RemovePakHash.Add(&TmpPak.Key);
			}
		}

		for (auto &PakID : RemovePakHash)
		{
			PakData.Remove(*PakID);
		}
	}
}

