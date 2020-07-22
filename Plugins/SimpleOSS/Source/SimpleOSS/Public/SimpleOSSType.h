// Copyright (C) RenZhai.2019.All Rights Reserved.
/*
下面的网址讲解了如何使用本套插件
 DocURL：			https://zhuanlan.zhihu.com/p/83673743

 下面的网址讲解如何开发当前的插件
 MarketplaceURL :  https://www.aboutcg.org/courseDetails/682/introduce

 如果想了解更多关于UE4教程请参考：
 URL :				https://zhuanlan.zhihu.com/p/60117613

 如果想系统了解人宅系列教程以及相关插件迭代更新 可以关注我的博客
 URL :				http://renzhai.net/

 如果想了解我们下一节的课程安排可以 可以在微信公众号搜所 人宅 关注即可 我们会推送很多富有技术的文章

 新浪博客 https://weibo.com/BZRZ/profile?s=6cm7D0  //这个博客从16年到现在 有三四年没怎么用过 以后说不定可以重新启用 先发个连接
 */
#pragma once

#include "CoreMinimal.h"
#include "SimpleOSSType.generated.h"

//#ifdef _MSC_VER
//#pragma warning(push)
//#pragma warning(disable:2005)
//#endif
//
//class RateLimiter;
//using namespace AlibabaCloud::OSS;
//

UENUM(BlueprintType)
enum class ERequestPayerType :uint8
{
	NOTSET = 0,
	BUCKETOWNER,
	REQUESTER,
	NONE
};

UENUM(BlueprintType)
enum class EAcl :uint8
{
	PRIVATE = 0,
	PUBLICREAD,
	PUBLICREADWRITE,
	NONE
};

UENUM(BlueprintType)
enum class EScheme :uint8
{
	HTTP,
	HTTPS
};

UENUM(BlueprintType)
enum class EOSSStorageType :uint8
{
	STANDARD,
	IA,
	ARCHIVE,
	NONE
};

USTRUCT(BlueprintType)
struct SIMPLEOSS_API FBucketInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|BucketInfo")
	FString Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|BucketInfo")
	FString OwnerID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|BucketInfo")
	FString OwnerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|BucketInfo")
	FString ExtranetEndpoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|BucketInfo")
	FString IntranetEndpoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|BucketInfo")
	EAcl Acl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|BucketInfo")
	FString Created;
};

USTRUCT(BlueprintType)
struct SIMPLEOSS_API FMultipartPartsUploadsInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|MultipartPartsUploadsInfo")
	FString Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|MultipartPartsUploadsInfo")
	FString UploadId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|MultipartPartsUploadsInfo")
	FString Initiated;
};

USTRUCT(BlueprintType)
struct SIMPLEOSS_API FRange
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Range")
	int64 Start;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Range")
	int64 End;

	FRange()
		:Start(0)
		, End(0)
	{}

	FRange(int64 InStart, int64 InEnd)
		:Start(InStart)
		, End(InEnd)
	{}

	FORCEINLINE bool IsValid() { return Start < End && End != 0; }

	FString ToString()
	{
		return FString::Printf(TEXT("bytes=%d-%i"),Start,End);
	}
};

USTRUCT(BlueprintType)
struct SIMPLEOSS_API FOSSObjectInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|ObjectInfo")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|ObjectInfo")
	int64 Size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|ObjectInfo")
	FString LastmodifyTime;
};

USTRUCT(BlueprintType)
struct SIMPLEOSS_API FRefererList
{
	GENERATED_USTRUCT_BODY()

	FRefererList()
		:bAllowEmptyReferer(false)
		,RefererListNum(INDEX_NONE)
	{}

	FORCEINLINE bool IsValid() { return !RequestId.IsEmpty(); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|RefererList")
	bool bAllowEmptyReferer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|RefererList")
	int64 RefererListNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|RefererList")
	FString RequestId;
};

USTRUCT(BlueprintType)
struct SIMPLEOSS_API FOSSObjectMeta
{
	GENERATED_USTRUCT_BODY()

	FOSSObjectMeta()
		:ContentLength(0)
		, Crc64(0)
		//, Acl(EAcl::NONE)
	{}

	//常用数据
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString ContentType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
		int64 ContentLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
		int64 Crc64;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	EOSSStorageType StorageClass;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString ContentDisposition;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString ContentEncoding;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString ContentMd5;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString Tag;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString LastModified;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString ExpirationTime;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	EAcl XOssAcl;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString XOssData;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString TargetSymlink;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
		FString Date;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString Authorization;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString OSSAccessKeyId;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString Expect;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString Expires;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString Signature;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	EAcl Acl;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString Location;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString Stat;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString UploadId;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString MaxParts;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString PartNumber;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString SecurityToke;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString XOssSecurityToken;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString XOssObjectType;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString XOssNextAppendPosition;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString XOssProcess;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString Lifecycle;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString Referer;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString Cors;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString Website;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString Logging;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString Appendable;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString Live;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString Status;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString Comp;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString History;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString Vod;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString StartTime;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString EndTime;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString PlaylistName;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString HLS;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Meta")
	//	FString PlayListM3u8;
};

USTRUCT(BlueprintType)
struct SIMPLEOSS_API FClientConfiguration
{
	GENERATED_USTRUCT_BODY()
public:
	FClientConfiguration();
	~FClientConfiguration() = default;
public:
	///**
	//* Http scheme to use. E.g. Http or Https.
	//*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	//EScheme Scheme;
	///**
	//* Max concurrent tcp connections for a single http client to use.
	//*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	//int32 MaxConnections;
	/**
	* DNS timeouts. Default 60 .
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	int32 DNSTimeoutS;
	/**
	* Socket connect timeout.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	int32 ConnectTimeoutS;
	///**
	//* The proxy scheme. Default HTTP
	//*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	//EScheme ProxyScheme;
	///**
	//* The proxy host.
	//*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	//FString ProxyHost;
	///**
	//* The proxy port.
	//*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	//int32 ProxyPort;
	///**
	//* The proxy username.
	//*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	//FString ProxyUserName;
	///**
	//*  The proxy password
	//*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	//FString ProxyPassword;
	///**
	//* set false to bypass SSL check.
	//*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	//bool bVerifySSL;
	///**
	//* your Certificate Authority path.
	//*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	//FString CaPath;
	///**
	//* your certificate file.
	//*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	//FString CaFile;
	/**
	* your certificate file.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	bool bCname;
	///**
	//* enable or disable crc64 check.
	//*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	//bool bEnableCrc64;
	///**
	//* enable or disable auto correct http request date.
	//*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	//bool bEnableDateSkewAdjustment;
	///**
	//* Rate limit data upload speed.
	//*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	//int64 SendRateLimiter;
	///**
	//* Rate limit data download speed.
	//*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	//int64 RecvRateLimiter;
	///**
	//* The interface for outgoing traffic. E.g. eth0 in linux
	//*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	//FString NetworkInterface;
	/**
	* The minimum rate that can be tolerated is controlled by setting the value of speed limit, which is 1024 by default, i.e. 1KB / s. 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	int64 MinimumRateOfTolerance;
	/**
	* The maximum time that can be tolerated is controlled by setting the value of speed "time, which is 15 seconds by default. 
	* Indicates a timeout if the transfer rate is less than 1KB / s for 15 seconds. 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SimpleOSS|Config")
	int64 MinimumRateOfToleranceTime;
};

typedef void(*ProgressCallback)(int64,int64);//consumed_bytes total_bytes
typedef void(*CallbackUploadPart)(FString, int64, int64, int64, bool, bool);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FProgressCallbackDelegate, int64, ConsumedBytes, int64, TotalBytes);
DECLARE_DYNAMIC_DELEGATE_SixParams(FCallbackUploadPartDelegate, FString , UploadId, int64 , PartCount, int64, Size, int64, InTotal, bool, bPart, bool , bComplete);
////
////#ifdef _MSC_VER
////#pragma warning(pop)
////#endif