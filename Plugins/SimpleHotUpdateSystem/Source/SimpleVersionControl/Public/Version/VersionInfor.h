
#pragma once

#include "CoreMinimal.h"
#include "VersionInfor.generated.h"

UENUM(BlueprintType)
enum class EPakVersionType :uint8
{
	PATCH_VERSION = 0,
	MAIN_VERSION,
	MAX_VERSION,
};

USTRUCT(BlueprintType)
struct SIMPLEVERSIONCONTROL_API FRemoteDataDescribe
{
	GENERATED_USTRUCT_BODY()

	FRemoteDataDescribe()
	:Len(0)
	, Crc(FGuid::NewGuid().ToString())
	, bDiscard(false)
	, PakVersionType(EPakVersionType::PATCH_VERSION)
	{}

	FRemoteDataDescribe(int64 InLen, const FString &InLink, const FString &InCrc, EPakVersionType VersionType)
		:Len(InLen)
		, Link(InLink)
		, Crc(InCrc)
		, bDiscard(false)
		, PakVersionType(VersionType)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remote Data")
	int64 Len;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remote Data")
	FString Link;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remote Data")
	FString Crc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remote Data")
	bool bDiscard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Remote Data")
	EPakVersionType PakVersionType;
};

USTRUCT(BlueprintType)
struct SIMPLEVERSIONCONTROL_API FSimpleVersionBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Version")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Version")
		FString Crc;

	FORCEINLINE friend bool operator== (const FSimpleVersionBase &A, const FSimpleVersionBase &B)
	{
		return A.Name == B.Name;
	}
};

/*
* Version List
* @ Name		Main version
* @ Crc			Check code
* @ Content		Version list
*/
USTRUCT(BlueprintType)
struct SIMPLEVERSIONCONTROL_API FVersionList :public FSimpleVersionBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Version")
	TArray<FString> Content;

	void Empty();
};

//²¹¶¡
USTRUCT(BlueprintType)
struct SIMPLEVERSIONCONTROL_API FVersionInfor :public FSimpleVersionBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Version")
	TArray<FRemoteDataDescribe> Content;

	void Empty();
};

//DLC ¸¶·ÑµÄDLC
USTRUCT(BlueprintType)
struct SIMPLEVERSIONCONTROL_API FDLCInfor :public FSimpleVersionBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DLC")
		FString Link;
};

typedef TArray<FString> FDLCList;

/*
* Specific		version
* @ Name		Current version
* @ Crc			Check code
* @ Content		DLC list
*/
typedef FVersionInfor FVersion;

namespace SimpleVersionControl
{
	SIMPLEVERSIONCONTROL_API void Save(const FVersionInfor &Infor, FString &Json);
	SIMPLEVERSIONCONTROL_API bool Read(const FString &Json,FVersionInfor &Infor);

	SIMPLEVERSIONCONTROL_API void Save(const FDLCInfor &Infor, FString &Json);
	SIMPLEVERSIONCONTROL_API bool Read(const FString &Json, FDLCInfor &Infor);

	SIMPLEVERSIONCONTROL_API void Save(const FDLCList &Infor, FString &Json);
	SIMPLEVERSIONCONTROL_API bool Read(const FString &Json, FDLCList &Infor);

	SIMPLEVERSIONCONTROL_API void Save(const FVersionList &Infor, FString &Json);
	SIMPLEVERSIONCONTROL_API bool Read(const FString &Json, FVersionList &Infor);
}