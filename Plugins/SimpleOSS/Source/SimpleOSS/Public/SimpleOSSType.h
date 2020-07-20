#pragma once

#include "CoreMinimal.h"
#include "alibabacloud/oss/client/RateLimiter.h"

class RateLimiter;
using namespace AlibabaCloud::OSS;

namespace SimpleOSS
{
	namespace OSS
	{
		struct SIMPLEOSS_API FMultipartPartsUploadsInfo
		{
			FString Key;
			FString UploadId;
			FString Initiated;
		};

		struct SIMPLEOSS_API FRange
		{
			uint64 Start;
			uint64 End;

			FRange()
				:Start(0)
				, End(0)
			{}

			FRange(uint64 InStart,uint64 InEnd)
				:Start(InStart)
				, End(InEnd)
			{}

			FORCEINLINE bool IsValid() { return Start < End && End != 0; }
		};

		enum class EOSSStorageType :uint8
		{
			STANDARD,
			IA,
			ARCHIVE,
			NONE
		};

		struct SIMPLEOSS_API FOSSObjectInfo
		{
			FString Name;
			int64 Size;
			FString LastmodifyTime;
		};

		struct SIMPLEOSS_API FOSSObjectMeta
		{
			FOSSObjectMeta()
				:ContentLength(0)
				,Crc64(0)
			{}

			//常用数据
			FString ContentType;
			int64 ContentLength;
			uint64 Crc64;
			FString CacheControl;
			FString ContentDisposition;
			FString ContentEncoding;
			FString ContentMd5;
			FString Tag;

			FString LastModified;
			FString ExpirationTime;

			TMap<FString, FString> MetaData;
			TMap<FString, FString> UserMetaData;
		public:

			FString FindMeta(const FString &Key) const{ return MetaData[Key];}
			FString FindUserMeta(const FString &Key) const{ return UserMetaData[Key];}
		};

		struct SIMPLEOSS_API FRefererList
		{
			FRefererList()
				:RequestId("")
			{}

			FORCEINLINE bool IsValid(){return !RequestId.IsEmpty();}

			bool bAllowEmptyReferer;
			int64 RefererListNum;
			FString RequestId;		
		};

		enum class ERequestPayerType :uint8
		{
			NOTSET = 0,
			BUCKETOWNER,
			REQUESTER,
			NONE
		};

		enum class EAcl :uint8
		{
			PRIVATE = 0,
			PUBLICREAD,
			PUBLICREADWRITE,
			DEFAULT,
			NONE
		};

		class SIMPLEOSS_API FSimpleOSSRateLimiter : public AlibabaCloud::OSS::RateLimiter
		{
		public:
			FSimpleOSSRateLimiter()
				:M_Rate(0)
			{}
			~FSimpleOSSRateLimiter() {};
			virtual void setRate(int rate) { M_Rate = rate; };
			virtual int Rate() const { return M_Rate; };

		private:
			int32 M_Rate;
		};

		enum Scheme
		{
			HTTP,
			HTTPS
		};
		class SIMPLEOSS_API FClientConfiguration
		{
		public:
			FClientConfiguration();
			~FClientConfiguration() = default;
		public:
			/**
			* Http scheme to use. E.g. Http or Https.
			*/
			OSS::Scheme Scheme;
			/**
			* Max concurrent tcp connections for a single http client to use.
			*/
			unsigned MaxConnections;
			/**
			* Socket read timeouts. Default 3000 ms.
			*/
			long RequestTimeoutMs;
			/**
			* Socket connect timeout.
			*/
			long ConnectTimeoutMs;
			/**
			* The proxy scheme. Default HTTP
			*/
			OSS::Scheme ProxyScheme;
			/**
			* The proxy host.
			*/
			FString ProxyHost;
			/**
			* The proxy port.
			*/
			uint32 ProxyPort;
			/**
			* The proxy username.
			*/
			FString ProxyUserName;
			/**
			*  The proxy password
			*/
			FString ProxyPassword;
			/**
			* set false to bypass SSL check.
			*/
			bool bVerifySSL;
			/**
			* your Certificate Authority path.
			*/
			FString CaPath;
			/**
			* your certificate file.
			*/
			FString CaFile;
			/**
			* your certificate file.
			*/
			bool bCname;
			/**
			* enable or disable crc64 check.
			*/
			bool bEnableCrc64;
			/**
			* enable or disable auto correct http request date.
			*/
			bool bEnableDateSkewAdjustment;
			/**
			* Rate limit data upload speed.
			*/
			uint32 SendRateLimiter;
			/**
			* Rate limit data download speed.
			*/
			uint32 RecvRateLimiter;
			/**
			* The interface for outgoing traffic. E.g. eth0 in linux
			*/
			FString NetworkInterface;
		};
	}
}

using namespace SimpleOSS::OSS;

typedef void(*ProgressCallback)(uint64, int64, int64, void*);

typedef void(*CallbackUploadPart)(FString,uint32, uint64, uint64, bool, bool);