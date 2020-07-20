#include "SimpleOSSManage.h"
#include "alibabacloud/oss/OssClient.h"
#include "SimpleOSSMacro.h"
#include <memory>
#include <fstream>
#include "Async/AsyncWork.h"
#include "Misc/ScopeLock.h"
#include "Engine/Engine.h"
#include "Async/TaskGraphInterfaces.h"
#include "string"
#include "algorithm"

#if PLATFORM_WINDOWS

#endif

using namespace AlibabaCloud::OSS;

FSimpleOSSManage *FSimpleOSSManage::SimpleOSS = nullptr;

class FOSSAsynTask :public FNonAbandonableTask
{
public:
	FOSSAsynTask(const TFunction<void()> InFuncton)
		:Functon(InFuncton)
	{
	}

	void DoWork()
	{
		Functon();
	}

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FOSSAsynTask, STATGROUP_ThreadPoolAsyncTasks);
	}

protected:
	const TFunction<void()> Functon;
};



FSimpleOSSManage::FSimpleOSSManage()
{

}

FSimpleOSSManage::~FSimpleOSSManage()
{

}

FSimpleOSSManage * FSimpleOSSManage::Get()
{
	if (!SimpleOSS)
	{
		SimpleOSS = new FSimpleOSSManage();

		/* 初始化网络等资源 */
		if (!IsSdkInitialized())
		{
			InitializeSdk();
		}
	}

	return SimpleOSS;
}

void FSimpleOSSManage::Destroy()
{
	if (SimpleOSS)
	{
		if (IsSdkInitialized())
		{
			ShutdownSdk();
		}
		
		delete SimpleOSS;
	}

	SimpleOSS = nullptr;
}

struct FAuxiliaryTools
{
	ClientConfiguration ConvertToOSSConf(const FClientConfiguration &InConf)
	{
		ClientConfiguration OSSConf;

		FString NetworkInterface;
		OSSConf.caFile = TCHAR_TO_UTF8(*InConf.CaFile);
		OSSConf.scheme = (Http::Scheme)InConf.Scheme;
		OSSConf.proxyScheme = (Http::Scheme)InConf.ProxyScheme;
		OSSConf.maxConnections = InConf.MaxConnections;
		OSSConf.requestTimeoutMs = InConf.RequestTimeoutMs;
		OSSConf.enableDateSkewAdjustment = InConf.bEnableDateSkewAdjustment;
		OSSConf.enableCrc64 = InConf.bEnableCrc64;
		OSSConf.isCname = InConf.bCname;
		OSSConf.verifySSL = InConf.bVerifySSL;
		OSSConf.caPath = TCHAR_TO_UTF8(*InConf.CaPath);
		OSSConf.proxyPassword = TCHAR_TO_UTF8(*InConf.ProxyPassword);
		OSSConf.proxyUserName = TCHAR_TO_UTF8(*InConf.ProxyUserName);
		OSSConf.proxyPort = InConf.ProxyPort;
		OSSConf.connectTimeoutMs = InConf.ConnectTimeoutMs;
		OSSConf.proxyHost = TCHAR_TO_UTF8(*InConf.ProxyHost);

		if (InConf.RecvRateLimiter != 0)
		{
			std::shared_ptr<FSimpleOSSRateLimiter> Recv = std::make_shared<FSimpleOSSRateLimiter>();
			Recv->setRate(InConf.RecvRateLimiter);
			OSSConf.recvRateLimiter = Recv;
		}

		if (InConf.SendRateLimiter != 0)
		{
			std::shared_ptr<FSimpleOSSRateLimiter> Send = std::make_shared<FSimpleOSSRateLimiter>();
			Send->setRate(InConf.SendRateLimiter);
			OSSConf.sendRateLimiter = Send;
		}

		return OSSConf;
	}

	void Print(const FString &Msg, float Time = 10.f, FColor Color = FColor::Red)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, Time, Color, Msg);
		}
	}
	void Print(const string &Msg, float Time = 10.f, FColor Color = FColor::Red)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, Time, Color, Msg.c_str());
		}
	}

	int64_t GetFileSize(const string& file)
	{
		std::fstream f(file, std::ios::in | std::ios::binary);
		f.seekg(0, f.end);
		int64_t size = f.tellg();
		f.close();

		return size;
	}
};

void FSimpleOSSManage::FOSS::InitAccounts(const FString &InAccessKeyId, const FString &InAccessKeySecret, const FString &InEndpoint)
{
	AccessKeyId		= TCHAR_TO_UTF8(*InAccessKeyId);
	AccessKeySecret = TCHAR_TO_UTF8(*InAccessKeySecret);
	Endpoint		= TCHAR_TO_UTF8(*InEndpoint);
}

void FSimpleOSSManage::FOSS::InitConf(const FClientConfiguration &InConf)
{
	Conf = InConf;
}

bool FSimpleOSSManage::FOSS::DoesBucketExist(const FString &InBucketName)
{
	string BucketName = TCHAR_TO_UTF8(*InBucketName);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	return Client.DoesBucketExist(BucketName);
}

bool FSimpleOSSManage::FOSS::CreateBucket(const FString &InBucketName, EOSSStorageType OSSStorageType , EAcl Acl)
{
	string BucketName = TCHAR_TO_UTF8(*InBucketName);
	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	/* 获取存储空间地域 */
	CreateBucketRequest Request(
			BucketName, 
			(StorageClass) OSSStorageType,
			(CannedAccessControlList)Acl);
		
	auto Outcome = Client.CreateBucket(Request);

	//是否存在这个桶
	if (!Outcome.isSuccess())
	{
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
		return false;
	}

	return true;
}

bool FSimpleOSSManage::FOSS::ListBuckets(TArray<FString> &InBucketName)
{
	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	/* 列举存储空间 */
	ListBucketsRequest request;
	auto Outcome = Client.ListBuckets(request);

	if (Outcome.isSuccess())
	{
		/* 打印存储空间信息 */
		for (auto result : Outcome.result().Buckets())
		{
			FString ObjectName = FString(result.Name().c_str());
			InBucketName.Add(ObjectName);
		}
	}
	else
	{
		/* 异常处理 */
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
		return false;
	}

	return true;
}

FString FSimpleOSSManage::FOSS::GetBucketLocation(const FString &InBucketName)
{
	string BucketName = TCHAR_TO_UTF8(*InBucketName);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	/* 获取存储空间地域 */
	GetBucketLocationRequest Request(BucketName);
	auto Outcome = Client.GetBucketLocation(Request);

	if (!Outcome.isSuccess())
	{
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
		return TEXT("");
	}

	return Outcome.result().Location().c_str();
}

bool FSimpleOSSManage::FOSS::DeleteBucket(const FString &InBucketName)
{
	int32 Number = 1000;

	TArray<FMultipartPartsUploadsInfo> MultipartPartsUploadsInfo;
	if (ListMultipartUploads(InBucketName, MultipartPartsUploadsInfo, Number))
	{
		//先删除碎片
		for (auto &Tmp : MultipartPartsUploadsInfo)
		{
			AbortMultipartUpload(InBucketName, Tmp.Key, Tmp.UploadId);
		}

		//列举对象
		TArray<FOSSObjectInfo> OSSObjectInfo;
		if (ListObjects(InBucketName, OSSObjectInfo, Number))
		{
			TArray<FString> ObjectName;
			for (auto &Tmp : OSSObjectInfo)
			{
				ObjectName.Add(Tmp.Name);
			}

			DeleteObjects(InBucketName, ObjectName);

			string BucketName = TCHAR_TO_UTF8(*InBucketName);

			FAuxiliaryTools  AuxiliaryTools;
			OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

			/* 指定要删除的bucket名称 */
			DeleteBucketRequest request(BucketName);
			/* 删除bucket */
			auto Outcome = Client.DeleteBucket(request);
			if (!Outcome.isSuccess())
			{
#if WITH_EDITOR
				SIMPLEOSS_MACRO_ERROR(Outcome)
#endif				
			}
			else
			{
				return true;
			}
		}
	}

	return false;
}

bool FSimpleOSSManage::FOSS::SetBucketAcl(const FString &InBucketName, EAcl BucketAcl)
{
	string BucketName = TCHAR_TO_UTF8(*InBucketName);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	/* 设置存储空间访问权限 */
	SetBucketAclRequest Request(BucketName, (CannedAccessControlList)BucketAcl);
	auto Outcome = Client.SetBucketAcl(Request);

	if (!Outcome.isSuccess())
	{
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
		return false;
	}

	return true;
}

SimpleOSS::OSS::EAcl FSimpleOSSManage::FOSS::GetBucketAcl(const FString InBucketName)
{
	string BucketName = TCHAR_TO_UTF8(*InBucketName);
	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	/* 设置存储空间访问权限 */
	GetBucketAclRequest Request(BucketName);
	auto Outcome = Client.GetBucketAcl(Request);

	if (!Outcome.isSuccess())
	{
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
		return EAcl::NONE;
	}

	return (EAcl)Outcome.result().Acl();
}

bool FSimpleOSSManage::FOSS::SetBucketRequestPayment(const FString &InBucketName, ERequestPayerType InRequestPayer)
{
	string BucketName = TCHAR_TO_UTF8(*InBucketName);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	SetBucketRequestPaymentRequest Request(BucketName);
	Request.setRequestPayer((RequestPayer)InRequestPayer);
	auto Outcome = Client.SetBucketRequestPayment(Request);

	if (!Outcome.isSuccess())
	{
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
			return false;
	}

	return true;
}

SimpleOSS::OSS::ERequestPayerType FSimpleOSSManage::FOSS::GetBucketRequestPayment(const FString &InBucketName)
{
	string BucketName = TCHAR_TO_UTF8(*InBucketName);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	/* 获取请求者付费模式 */
	GetBucketRequestPaymentRequest Request(BucketName);
	auto Outcome = Client.GetBucketRequestPayment(Request);

	if (!Outcome.isSuccess())
	{
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
		return ERequestPayerType::NONE;
	}

	return (ERequestPayerType)Outcome.result().Payer();
}

bool FSimpleOSSManage::FOSS::SetBucketReferer(const FString &InBucketName, const TArray<FString> &URLReferer, bool bAllowEmptyRefere /*= false*/)
{
	string BucketName = TCHAR_TO_UTF8(*InBucketName);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	/* 设置防盗链 */
	SetBucketRefererRequest Request(BucketName);
	for (auto &Tmp : URLReferer)
	{
		Request.addReferer(TCHAR_TO_UTF8(*Tmp));
	}

	Request.setAllowEmptyReferer(bAllowEmptyRefere);

	auto Outcome = Client.SetBucketReferer(Request);
	if (!Outcome.isSuccess())
	{
		/* 异常处理 */
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
		return false;
	}

	return true;
}

SimpleOSS::OSS::FRefererList FSimpleOSSManage::FOSS::GetBucketReferer(const FString &InBucketName)
{
	FRefererList MyRefererList;
	string BucketName = TCHAR_TO_UTF8(*InBucketName);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	/* 获取防盗链信息 */
	GetBucketRefererRequest Request(BucketName);
	auto Outcome = Client.GetBucketReferer(Request);
	if (!Outcome.isSuccess())
	{
		/* 异常处理 */
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
	}
	else
	{
		MyRefererList.RequestId = Outcome.result().RequestId().c_str();
		MyRefererList.bAllowEmptyReferer = Outcome.result().AllowEmptyReferer();
		MyRefererList.RefererListNum = Outcome.result().RefererList().size();
	}

	return MyRefererList;
}

bool FSimpleOSSManage::FOSS::DeleteBucketReferer(const FString &InBucketName, bool bAllowEmptyRefere /*= true*/)
{
	string BucketName = TCHAR_TO_UTF8(*InBucketName);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	/* 清空防盗链,防盗链不能直接清空，需要新建一个允许空referer的规则来覆盖之前的规则 */
	SetBucketRefererRequest Request(BucketName);
	Request.setAllowEmptyReferer(bAllowEmptyRefere);

	auto Outcome = Client.SetBucketReferer(Request);

	if (!Outcome.isSuccess())
	{
		/* 异常处理 */
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
		return false;
	}

	return true;
}

bool FSimpleOSSManage::FOSS::GetObjectMeta(const FString &InBucketName, const FString &InObjectName, FOSSObjectMeta &OSSObjectInfo)
{	string BucketName = TCHAR_TO_UTF8(*InBucketName);
	string ObjectName = TCHAR_TO_UTF8(*InObjectName);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	auto Outcome = Client.HeadObject(BucketName, ObjectName);
	if (!Outcome.isSuccess())
	{
		/*异常处理*/
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
		return false;
	}
	else
	{
		auto headMeta = Outcome.result();
		OSSObjectInfo.CacheControl = headMeta.CacheControl().c_str();
		OSSObjectInfo.ContentType = headMeta.ContentType().c_str();
		OSSObjectInfo.ContentLength = headMeta.ContentLength();
		OSSObjectInfo.ContentDisposition = headMeta.ContentDisposition().c_str();
		OSSObjectInfo.ContentEncoding = headMeta.ContentEncoding().c_str();
		OSSObjectInfo.ContentMd5 = headMeta.ContentMd5().c_str();
		OSSObjectInfo.Crc64 = headMeta.CRC64();
		OSSObjectInfo.ContentEncoding = headMeta.CacheControl().c_str();

		OSSObjectInfo.Tag = headMeta.ETag().c_str();
		OSSObjectInfo.LastModified = headMeta.LastModified().c_str();
		OSSObjectInfo.ExpirationTime = headMeta.ExpirationTime().c_str();

		for (map<string, string>::iterator iter = headMeta.HttpMetaData().begin(); iter != headMeta.HttpMetaData().end(); iter++)
		{
			OSSObjectInfo.MetaData.Add(iter->first.c_str(), iter->second.c_str());
		}

		for (map<string, string>::iterator iter = headMeta.UserMetaData().begin(); iter != headMeta.UserMetaData().end(); iter++)
		{
			OSSObjectInfo.UserMetaData.Add(iter->first.c_str(), iter->second.c_str());
		}
	}

	return true;
}

SimpleOSS::OSS::EAcl FSimpleOSSManage::FOSS::GetObjectAcl(const FString &InBucketName, const FString &InObjectName)
{
	string BucketName = TCHAR_TO_UTF8(*InBucketName);
	string ObjectName = TCHAR_TO_UTF8(*InObjectName);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	GetObjectAclRequest Request(BucketName, ObjectName);
	auto Outcome = Client.GetObjectAcl(Request);

	if (!Outcome.isSuccess())
	{
		/*异常处理*/
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
		return EAcl::NONE;
	}

	return(EAcl)Outcome.result().Acl();
}

bool FSimpleOSSManage::FOSS::SetObjectAcl(const FString &InBucketName, const FString &InObjectName, EAcl InAcl)
{
	string BucketName = TCHAR_TO_UTF8(*InBucketName);
	string ObjectName = TCHAR_TO_UTF8(*InObjectName);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	SetObjectAclRequest request(BucketName, ObjectName);
	request.setAcl((CannedAccessControlList)InAcl);

	auto Outcome = Client.SetObjectAcl(request);
	if (!Outcome.isSuccess())
	{
		/*异常处理*/
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
		return false;
	}

	return true;
}

bool FSimpleOSSManage::FOSS::DoesObjectExist(const FString &InBucketName, const FString &InObjectName)
{
	string BucketName = TCHAR_TO_UTF8(*InBucketName);
	string ObjectName = TCHAR_TO_UTF8(*InObjectName);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	return Client.DoesObjectExist(BucketName, ObjectName);
}

bool FSimpleOSSManage::FOSS::ListObjects(const FString &InBucketName, TArray<FOSSObjectInfo> &ObjectName, int32 MaxNumber /*= 100*/)
{
	string BucketName = TCHAR_TO_UTF8(*InBucketName);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	/* 列举文件 */
	ListObjectsRequest request(BucketName);

	/* 设置最大个数 */
	request.setMaxKeys(MaxNumber);

	auto Outcome = Client.ListObjects(request);
	if (!Outcome.isSuccess())
	{
		/* 异常处理 */
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
		return false;
	}
	else
	{
		for (const auto& object : Outcome.result().ObjectSummarys())
		{
			FOSSObjectInfo OSSObjectInfo;
			OSSObjectInfo.Name = FString(object.Key().c_str());
			OSSObjectInfo.Size = object.Size();
			OSSObjectInfo.LastmodifyTime = FString(object.LastModified().c_str());
			ObjectName.Add(OSSObjectInfo);
		}
	}

	return true;
}

bool FSimpleOSSManage::FOSS::ListObjectsByPrefix(const FString &InBucketName, const FString &InKeyPrefix, TArray<FOSSObjectInfo> &ObjectName)
{
	bool bSuccess = true;

	string BucketName = TCHAR_TO_UTF8(*InBucketName);
	string keyPrefix = TCHAR_TO_UTF8(*InKeyPrefix);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	string nextMarker = "";
	ListObjectOutcome Outcome;

	do
	{
		/* 列举文件 */
		ListObjectsRequest request(BucketName);

		/* 设置正斜线（/）为文件夹的分隔符 */
		request.setDelimiter("/");
		request.setPrefix(keyPrefix);
		request.setMarker(nextMarker);

		Outcome = Client.ListObjects(request);
		if (!Outcome.isSuccess())
		{
			/* 异常处理 */
#if WITH_EDITOR
			SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
			bSuccess = false;
			break;
		}
		for (const auto& object : Outcome.result().ObjectSummarys())
		{
			FOSSObjectInfo OSSObjectInfo;
			OSSObjectInfo.Name = FString(object.Key().c_str());
			OSSObjectInfo.Size = object.Size();
			OSSObjectInfo.LastmodifyTime = FString(object.LastModified().c_str());
			ObjectName.Add(OSSObjectInfo);
		}
#if WITH_EDITOR
		for (const auto& commonPrefix : Outcome.result().CommonPrefixes())
		{
			AuxiliaryTools.Print(commonPrefix);
			//		std::cout << "commonPrefix" << ",name:" << commonPrefix << std::endl;
		}
#endif
		nextMarker = Outcome.result().NextMarker();
	} while (Outcome.result().IsTruncated());

	return bSuccess;
}

bool FSimpleOSSManage::FOSS::SetObjectStorageType(const FString &InBucketName, const FString &InObjectName, EOSSStorageType OSSStorageType)
{
	EOSSStorageType StorageType = GetObjectStorageType(InBucketName, InObjectName);
	if (StorageType != EOSSStorageType::NONE)
	{
		auto SSStorage = [](EOSSStorageType InOSSStorageType)->string
		{
			switch (InOSSStorageType)
			{
			case EOSSStorageType::STANDARD:
				return "Standard";
			case EOSSStorageType::IA:
				return "IA";
			case EOSSStorageType::ARCHIVE:
				return "Archive";
			}

			return "";
		};

		string BucketName = TCHAR_TO_UTF8(*InBucketName);
		string ObjectName = TCHAR_TO_UTF8(*InObjectName);

		FAuxiliaryTools  AuxiliaryTools;
		OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

		if (StorageType == EOSSStorageType::ARCHIVE)
		{
			/* 检查目标文件是否为归档类型。如果是，则需要先解冻才能更改存储类型 */
			auto restoreOutcome = Client.RestoreObject(BucketName, ObjectName);
			if (!restoreOutcome.isSuccess())
			{
				/*异常处理*/
#if WITH_EDITOR
				SIMPLEOSS_MACRO_ERROR(restoreOutcome)
#endif
					return false;
			}

			std::string onGoingRestore("ongoing-request=\"false\"");
			int32 maxWaitTimeInSeconds = 600;
			while (maxWaitTimeInSeconds > 0)
			{
				auto meta = Client.HeadObject(BucketName, ObjectName);
				std::string restoreStatus = meta.result().HttpMetaData()["x-oss-restore"];
				std::transform(restoreStatus.begin(), restoreStatus.end(), restoreStatus.begin(), ::tolower);//toupper
				if (!restoreStatus.empty() &&
					restoreStatus.compare(0, onGoingRestore.size(), onGoingRestore) == 0)
				{
#if WITH_EDITOR
					/* 成功解冻归档文件*/
					AuxiliaryTools.Print(restoreStatus);
#endif
					break;
				}

				std::this_thread::sleep_for(std::chrono::seconds(10));
				maxWaitTimeInSeconds--;
			}
		}

		ObjectMetaData objectMeta;
		objectMeta.addHeader("x-oss-storage-class", SSStorage(OSSStorageType));//Standard、IA、Archive
		CopyObjectRequest Request(BucketName, ObjectName, objectMeta);
		Request.setCopySource(BucketName, ObjectName);

		/* 修改为上述指定的文件存储类型 */
		auto Outcome = Client.CopyObject(Request);
		if (!Outcome.isSuccess())
		{
			/*异常处理*/
#if WITH_EDITOR
			SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
			return false;
		}

		return true;
	}

	return false;
}

SimpleOSS::OSS::EOSSStorageType FSimpleOSSManage::FOSS::GetObjectStorageType(const FString &InBucketName, const FString &InObjectName)
{
	FOSSObjectMeta OSSObjectInfo;
	if (GetObjectMeta(InBucketName, InObjectName, OSSObjectInfo))
	{
		auto SSStorage = [](const FString &OSSStorageType)->EOSSStorageType
		{
			if (OSSStorageType == "Standard")
			{
				return EOSSStorageType::STANDARD;
			}
			else if (OSSStorageType == "IA")
			{
				return EOSSStorageType::IA;
			}
			else if (OSSStorageType == "Archive")
			{
				return EOSSStorageType::ARCHIVE;
			}
			else
			{
				return EOSSStorageType::NONE;
			}
		};

		return SSStorage(OSSObjectInfo.FindMeta("x-oss-storage-class"));
	}

	return EOSSStorageType::NONE;
}

bool FSimpleOSSManage::FOSS::CreateSymlink(const FString &InBucketName, const FString &InObjectName, const FString &InLinkObjectName)
{
	string BucketName = TCHAR_TO_UTF8(*InBucketName);
	string ObjectName = TCHAR_TO_UTF8(*InObjectName);
	string LinkObjectName = TCHAR_TO_UTF8(*InLinkObjectName);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	/* 创建软链接 */
	auto meta = ObjectMetaData();
	CreateSymlinkRequest request(BucketName, ObjectName, meta);
	request.SetSymlinkTarget(LinkObjectName);
	auto Outcome = Client.CreateSymlink(request);
	if (!Outcome.isSuccess())
	{
		/* 异常处理 */
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
		return false;
	}

	return true;
}

FString FSimpleOSSManage::FOSS::GetSymlink(const FString &InBucketName, const FString &InLinkName)
{
	string BucketName = TCHAR_TO_UTF8(*InBucketName);
	string LinkName = TCHAR_TO_UTF8(*InLinkName);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	GetSymlinkRequest request(BucketName, LinkName);
	auto Outcome = Client.GetSymlink(request);
	if (!Outcome.isSuccess())
	{
		/* 异常处理 */
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
		return "";
	}

	return Outcome.result().SymlinkTarget().c_str();
}

bool FSimpleOSSManage::FOSS::GetObjectToMemory(
	const FString &InBucketName, 
	const FString &InObjectName, 
	FString &ContextString, 
	SimpleOSS::OSS::FRange Range,
	ProgressCallback InProgressCallback /*= nullptr*/)
{
	string BucketName = TCHAR_TO_UTF8(*InBucketName);
	string ObjectName = TCHAR_TO_UTF8(*InObjectName);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	GetObjectRequest Request(BucketName, ObjectName);
	TransferProgress progressCallback = { InProgressCallback , nullptr }; 
	Request.setTransferProgress(progressCallback);
	if (Range.IsValid())
	{
		Request.setRange(Range.Start, Range.End);
	}

	auto Outcome = Client.GetObject(Request);
	if (Outcome.isSuccess())
	{
#if WITH_EDITOR
		AuxiliaryTools.Print(FString::FromInt(Outcome.result().Metadata().ContentLength()));
#endif
		/*打印下载内容*/
		std::string content;
		*(Outcome.result().Content()) >> content;
		ContextString = content.c_str();
	}
	else
	{
		/*异常处理*/
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif

		return false;
	}

	return true;
}

bool FSimpleOSSManage::FOSS::GetObjectToLocal(
	const FString &InBucketName, 
	const FString &InObjectName,
	const FString &InLocalPaths,
	SimpleOSS::OSS::FRange Range,
	ProgressCallback InProgressCallback /*= nullptr*/)
{
	FScopeLock ScopeLock(&Mutex);

	string BucketName = TCHAR_TO_UTF8(*InBucketName);
	string ObjectName = TCHAR_TO_UTF8(*InObjectName);
	string FileNametoSave = TCHAR_TO_UTF8(*InLocalPaths);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	GetObjectRequest Request(BucketName, ObjectName);
	Request.setResponseStreamFactory([=]() {return std::make_shared<std::fstream>(FileNametoSave, std::ios_base::out | std::ios_base::in | std::ios_base::trunc | std::ios_base::binary); });

	TransferProgress progressCallback = { InProgressCallback , nullptr };
	Request.setTransferProgress(progressCallback);
	if (Range.IsValid())
	{
		Request.setRange(Range.Start, Range.End);
	}

	auto Outcome = Client.GetObject(Request);
	if (!Outcome.isSuccess())
	{
		/*异常处理*/
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
		return false;
	}

	return true;
}

bool FSimpleOSSManage::FOSS::ResumableDownloadObject(
	const FString &InBucketName, 
	const FString &InObjectName, 
	const FString &InDownloadFilePath, 
	SimpleOSS::OSS::FRange Range, 
	ProgressCallback InProgressCallback /*= nullptr*/,
	const FString &InCheckpointFilePath /*= FPaths::ProjectSavedDir() / TEXT("OSSCache")*/)
{
	FScopeLock ScopeLock(&Mutex);

	string BucketName = TCHAR_TO_UTF8(*InBucketName);
	string DownloadFilePath = TCHAR_TO_UTF8(*InDownloadFilePath);
	string ObjectName = TCHAR_TO_UTF8(*InObjectName);
	string CheckpointFilePath = TCHAR_TO_UTF8(*InCheckpointFilePath);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	DownloadObjectRequest Request(BucketName, ObjectName, DownloadFilePath, CheckpointFilePath);
	TransferProgress progressCallback = { InProgressCallback , nullptr };
	Request.setTransferProgress(progressCallback);
	if (Range.IsValid())
	{
		Request.setRange(Range.Start, Range.End);
	}

	auto Outcome = Client.ResumableDownloadObject(Request);
	if (!Outcome.isSuccess())
	{
		/*异常处理*/
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
		return false;
	}

	return true;
}

bool FSimpleOSSManage::FOSS::CopyObject(const FString &InSourceBucketName, const FString &InSourceObjectName, const FString &InCopyBucketName, const FString &InCopyObjectName, ProgressCallback InProgressCallback /*= nullptr*/)
{
	FScopeLock ScopeLock(&Mutex);

	string SourceBucketName = TCHAR_TO_UTF8(*InSourceBucketName);
	string SourceObjectName = TCHAR_TO_UTF8(*InSourceObjectName);
	string CopyBucketName = TCHAR_TO_UTF8(*InCopyBucketName);
	string CopyObjectName = TCHAR_TO_UTF8(*InCopyObjectName);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	CopyObjectRequest Request(CopyBucketName, CopyObjectName);
	Request.setCopySource(SourceBucketName, SourceObjectName);

	TransferProgress progressCallback = { InProgressCallback , nullptr };
	Request.setTransferProgress(progressCallback);

	auto Outcome = Client.CopyObject(Request);
	if (!Outcome.isSuccess())
	{
		/*异常处理*/
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
		return false;
	}

	return true;
}

bool FSimpleOSSManage::FOSS::DeleteObject(const FString &InBucketName, const FString &InObjectName, ProgressCallback InProgressCallback)
{
	TArray<FString> MyObjectName;
	MyObjectName.Add(InObjectName);
	return DeleteObjects(InBucketName, MyObjectName, InProgressCallback);
}

bool FSimpleOSSManage::FOSS::DeleteObjects(const FString &InBucketName, const TArray<FString> &InObjectName, ProgressCallback InProgressCallback)
{
	FScopeLock ScopeLock(&Mutex);

	string BucketName = TCHAR_TO_UTF8(*InBucketName);


	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	DeleteObjectsRequest Request(BucketName);
	TransferProgress progressCallback = { InProgressCallback , nullptr };
	Request.setTransferProgress(progressCallback);

	/* 添加删除文件名 */
	for (const auto &Tmp : InObjectName)
	{
		string ObjectName = TCHAR_TO_UTF8(*Tmp);
		Request.addKey(ObjectName);
	}

	/* 删除文件 */
	auto Outcome = Client.DeleteObjects(Request);
	if (!Outcome.isSuccess())
	{
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif

			return false;
	}

	return true;
}

bool FSimpleOSSManage::FOSS::ListMultipartUploads(const FString &InBucketName, TArray<FMultipartPartsUploadsInfo> &MultipartPartsInfo, int32 MaxParts /*= 1000*/)
{
	bool bSuccess = true;

	string BucketName = TCHAR_TO_UTF8(*InBucketName);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	/* 列举已上传分片,默认列举1000个分片 */
	ListMultipartUploadsRequest listmultiuploadrequest(BucketName);
	listmultiuploadrequest.setMaxUploads(MaxParts);

	ListMultipartUploadsOutcome listresult;
	do 
	{
		listresult = Client.ListMultipartUploads(listmultiuploadrequest);
		if (!listresult.isSuccess())
		{
			/* 异常处理 */
#if WITH_EDITOR
			SIMPLEOSS_MACRO_ERROR(listresult)
#endif
			bSuccess = false;
			break;
		}
		else
		{
			for (const auto& part : listresult.result().MultipartUploadList())
			{
				FMultipartPartsUploadsInfo MultipartPartsUploadsInfo;
				MultipartPartsUploadsInfo.Key = part.Key.c_str();
				MultipartPartsUploadsInfo.UploadId = part.UploadId.c_str();
				MultipartPartsUploadsInfo.Initiated = part.Initiated.c_str();
				MultipartPartsInfo.Add(MultipartPartsUploadsInfo);
			}
		}

		listmultiuploadrequest.setKeyMarker(listresult.result().NextKeyMarker());
		listmultiuploadrequest.setUploadIdMarker(listresult.result().NextUploadIdMarker());
	} while (listresult.result().IsTruncated());

	return bSuccess;
}

void FSimpleOSSManage::FOSS::AsyncPutObject(
	const FString &InBucketName, 
	const FString &InLocalPaths, 
	const FString &InObjectName /*= FString()*/, 
	ProgressCallback InProgressCallback /*= nullptr*/, 
	const TMap<FString, FString> &CustomMeta /*= TMap<FString, FString>()*/, 
	const FOSSObjectMeta &OSSObjectMeta /*= FOSSObjectMeta()*/)
{
	auto Lamada = [=]()
	{
		PutObject(
			InBucketName, 
			InLocalPaths, 
			InObjectName, 
			InProgressCallback,
			CustomMeta, 
			OSSObjectMeta);
	};

	(new FAutoDeleteAsyncTask<FOSSAsynTask>(Lamada))->StartBackgroundTask();
}

void FSimpleOSSManage::FOSS::AsyncResumableUploadObject(
	const FString &InBucketName, 
	const FString &InObjectName,
	const FString &InUploadFilePath,
	const FString &InCheckpointFilePath /*= FPaths::ProjectSavedDir() / TEXT("OSSCache")*/,
	ProgressCallback InProgressCallback /*= nullptr*/, 
	const TMap<FString, FString> &CustomMeta /*= TMap<FString, FString>()*/,
	const FOSSObjectMeta &OSSObjectMeta /*= FOSSObjectMeta()*/, 
	const FString &DebugMsg /*= TEXT("Thank you for using Oss Storage Service!")*/)
{
	auto Lamada = [=]()
	{
		ResumableUploadObject(
			InBucketName,
			InObjectName,
			InUploadFilePath,
			InCheckpointFilePath,
			InProgressCallback,
			CustomMeta,
			OSSObjectMeta,
			DebugMsg);
	};

	(new FAutoDeleteAsyncTask<FOSSAsynTask>(Lamada))->StartBackgroundTask();
}

void FSimpleOSSManage::FOSS::AsyncUploadPart(
	FString &InUploadId, 
	const FString &InBucketName, 
	const FString &InObjectName, 
	const FString &InLocalPaths, 
	int32 PartSize /*= 1024 * 1024 * 10*/,
	CallbackUploadPart InCallbackUploadPart,
	const TMap<FString, FString> &CustomMeta /*= TMap<FString, FString>()*/, 
	const FOSSObjectMeta &OSSObjectMeta /*= FOSSObjectMeta()*/)
{
	auto Lamada = [=]()
	{
		FString NewUploadId;
		UploadPart(
			NewUploadId,
			InBucketName,
			InObjectName,
			InLocalPaths,
			PartSize,
			InCallbackUploadPart,
			CustomMeta,
			OSSObjectMeta);
	};

	(new FAutoDeleteAsyncTask<FOSSAsynTask>(Lamada))->StartBackgroundTask();
}

void FSimpleOSSManage::FOSS::AsyncGetObjectToLocal(
	const FString &InBucketName,
	const FString &InObjectName, 
	const FString &InLocalPaths, 
	SimpleOSS::OSS::FRange Range /*= FRange()*/,
	ProgressCallback InProgressCallback /*= nullptr*/)
{
	auto Lamada = [=]()
	{
		GetObjectToLocal(
			InBucketName,
			InObjectName,
			InLocalPaths,
			Range,
			InProgressCallback);
	};

	(new FAutoDeleteAsyncTask<FOSSAsynTask>(Lamada))->StartBackgroundTask();
}

void FSimpleOSSManage::FOSS::AsyncResumableDownloadObject(
	const FString &InBucketName, 
	const FString &InObjectName, 
	const FString &InDownloadFilePath, 
	SimpleOSS::OSS::FRange Range /*= FRange()*/, 
	ProgressCallback InProgressCallback /*= nullptr*/, 
	const FString &InCheckpointFilePath /*= FPaths::ProjectSavedDir() / TEXT("OSSCache")*/)
{
	auto Lamada = [=]()
	{
		ResumableDownloadObject(
			InBucketName,
			InObjectName,
			InDownloadFilePath,
			Range,
			InProgressCallback,
			InCheckpointFilePath);
	};

	(new FAutoDeleteAsyncTask<FOSSAsynTask>(Lamada))->StartBackgroundTask();
}

void FSimpleOSSManage::FOSS::AsyncCopyObject(
	const FString &InSourceBucketName, 
	const FString &InSourceObjectName, 
	const FString &InCopyBucketName, 
	const FString &InCopyObjectName, 
	ProgressCallback InProgressCallback /*= nullptr*/)
{
	auto Lamada = [=]()
	{
		CopyObject(
			InSourceBucketName,
			InSourceObjectName,
			InCopyBucketName,
			InCopyObjectName,
			InProgressCallback);
	};

	(new FAutoDeleteAsyncTask<FOSSAsynTask>(Lamada))->StartBackgroundTask();
}

void FSimpleOSSManage::FOSS::AsyncDeleteObjects(const FString &InBucketName, const TArray<FString> &InObjectName, ProgressCallback InProgressCallback /*= nullptr*/)
{
	auto Lamada = [=]()
	{
		DeleteObjects(
			InBucketName,
			InObjectName,
			InProgressCallback);
	};

	(new FAutoDeleteAsyncTask<FOSSAsynTask>(Lamada))->StartBackgroundTask();
}

bool FSimpleOSSManage::FOSS::PutObject(
	const FString &InBucketName, 
	const FString &InLocalPaths,
	const FString &InObjectName /*= FString()*/, 
	ProgressCallback InProgressCallback /*= nullptr*/, 
	const TMap<FString, FString> &CustomMeta /*= TMap<FString, FString>()*/,
	const FOSSObjectMeta &OSSObjectMeta /*= FOSSObjectMeta()*/)
{
	FScopeLock ScopeLock(&Mutex);

	string BucketName = TCHAR_TO_UTF8(*InBucketName);
	string ObjectName = TCHAR_TO_UTF8(*(InObjectName.IsEmpty() ? FPaths::GetCleanFilename(InLocalPaths) : InObjectName));
	string LocalPaths = TCHAR_TO_UTF8(*InLocalPaths);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	std::shared_ptr<std::iostream> content = std::make_shared<std::fstream>(LocalPaths,std::ios::in | std::ios::binary);

	PutObjectRequest Request(BucketName, ObjectName, content);

	SIMPLEOSS_MACRO_TYPE1(Request)

	auto Outcome = Client.PutObject(Request);
	if (!Outcome.isSuccess())
	{
		/*异常处理*/
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
		return false;
	}

	return true;
}

bool FSimpleOSSManage::FOSS::ResumableUploadObject(const FString &InBucketName, const FString &InObjectName, const FString &InUploadFilePath, const FString &InCheckpointFilePath /*= FPaths::ProjectSavedDir() / TEXT("OSSCache")*/, ProgressCallback InProgressCallback /*= nullptr*/, const TMap<FString, FString> &CustomMeta /*= TMap<FString, FString>()*/, const FOSSObjectMeta &OSSObjectMeta /*= FOSSObjectMeta()*/, const FString &DebugMsg /*= TEXT("Thank you for using Oss Storage Service!")*/)
{
	FScopeLock ScopeLock(&Mutex);
	
	string BucketName = TCHAR_TO_UTF8(*InBucketName);
	string UploadFilePath = TCHAR_TO_UTF8(*InUploadFilePath);
	string ObjectName = TCHAR_TO_UTF8(*(InObjectName.IsEmpty() ? FPaths::GetCleanFilename(InUploadFilePath) : InObjectName));
	string CheckpointFilePath = TCHAR_TO_UTF8(*InCheckpointFilePath);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	UploadObjectRequest Request(BucketName, ObjectName, UploadFilePath, CheckpointFilePath);

	SIMPLEOSS_MACRO_TYPE1(Request)

	auto Outcome = Client.ResumableUploadObject(Request);
	if (!Outcome.isSuccess())
	{
		/* 异常处理 */
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif

		return false;
	}

	return true;
}

bool FSimpleOSSManage::FOSS::UploadPart(
	FString &InUploadId, 
	const FString &InBucketName,
	const FString &InObjectName, 
	const FString &InLocalPaths,
	int32 PartSize /*= 1024 * 1024 * 10*/,
	CallbackUploadPart InCallbackUploadPart,
	const TMap<FString, FString> &CustomMeta /*= TMap<FString, FString>()*/,
	const FOSSObjectMeta &OSSObjectMeta /*= FOSSObjectMeta()*/)
{
	FScopeLock ScopeLock(&Mutex);

	auto Lamada = [&InCallbackUploadPart](FString UploadId, uint32 PartCount, uint64 Size ,uint64 InTotal, bool bPart, bool bComplete)
	{
		if (InCallbackUploadPart != nullptr)
		{
			//呼叫主线程
			FGraphEventRef FenceHandle = FFunctionGraphTask::CreateAndDispatchWhenReady(
			[=]() 
			{
				InCallbackUploadPart(UploadId, PartCount, Size, InTotal,bPart, bComplete);
			}, TStatId(), nullptr, ENamedThreads::GameThread);
		}
	};

	string BucketName = TCHAR_TO_UTF8(*InBucketName);
	string ObjectName = TCHAR_TO_UTF8(*(InObjectName.IsEmpty() ? FPaths::GetCleanFilename(InLocalPaths) : InObjectName));
	string FileToUpload = TCHAR_TO_UTF8(*InLocalPaths);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	InitiateMultipartUploadRequest initUploadRequest(BucketName, ObjectName);
	auto uploadIdResult = Client.InitiateMultipartUpload(initUploadRequest);
	auto uploadId = uploadIdResult.result().UploadId();

	PartList partETagList;

	auto fileSize = AuxiliaryTools.GetFileSize(FileToUpload);
	int32 partCount = static_cast<int32> (fileSize / PartSize);

	/* 计算分片个数 */
	if (fileSize % PartSize != 0)
	{
		partCount++;
	}

	for (int32 i = 1; i <= partCount; i++)
	{
		auto skipBytes = PartSize * (i - 1);
		auto size = (PartSize < fileSize - skipBytes) ? PartSize : (fileSize - skipBytes);
		std::shared_ptr<std::iostream> content = std::make_shared<std::fstream>(FileToUpload, std::ios::in | std::ios::binary);

		UploadPartRequest uploadPartRequest(BucketName, ObjectName, content);
		uploadPartRequest.setContentLength(size);
		uploadPartRequest.setUploadId(uploadId);
		uploadPartRequest.setPartNumber(i);

		auto uploadPartOutcome = Client.UploadPart(uploadPartRequest);
		if (uploadPartOutcome.isSuccess())
		{
			Part part(i, uploadPartOutcome.result().ETag());
			partETagList.push_back(part);

			Lamada(uploadId.c_str(),i,size, fileSize,true,false);
		}
		else
		{
#if WITH_EDITOR
			SIMPLEOSS_MACRO_ERROR(uploadPartOutcome)
#endif
			Lamada(uploadId.c_str(), i, size, fileSize, false, false);
		}
	}

	CompleteMultipartUploadRequest Request(BucketName, ObjectName);
	Request.setUploadId(uploadId);
	Request.setPartList(partETagList);

	SIMPLEOSS_MACRO_META(Request)

	auto Outcome = Client.CompleteMultipartUpload(Request);
	if (!Outcome.isSuccess())
	{
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif

		Lamada(uploadId.c_str(), 0, 0, fileSize, false, false);
		return false;
	}

	InUploadId = uploadId.c_str();

	Lamada(uploadId.c_str(), 0, 0, fileSize, true, true);
	return true;
}

bool FSimpleOSSManage::FOSS::AbortMultipartUpload(const FString &InBucketName, const FString &InObjectName, const FString &InUploadId)
{
	string BucketName = TCHAR_TO_UTF8(*InBucketName);
	string ObjectName = TCHAR_TO_UTF8(*InObjectName);
	string UploadId = TCHAR_TO_UTF8(*InUploadId);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	AbortMultipartUploadRequest abortUploadRequest(BucketName, ObjectName, UploadId);
	auto abortUploadIdResult = Client.AbortMultipartUpload(abortUploadRequest);
	if (!abortUploadIdResult.isSuccess())
	{
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(abortUploadIdResult)
#endif

		return false;
	}

	return true;
}

FString FSimpleOSSManage::FOSS::GeneratePresignedUrl(const FString &InBucketName, const FString &InObjectName)
{
	string BucketName = TCHAR_TO_UTF8(*InBucketName);
	string ObjectName = TCHAR_TO_UTF8(*InObjectName);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	auto Outcome = Client.GeneratePresignedUrl(BucketName, ObjectName);
	if (!Outcome.isSuccess())
	{
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif

		return "";
	}

	return Outcome.result().c_str();
}

void FSimpleOSSManage::FOSS::AsyncGetObjectByUrl(const FString& InURL, const FString& InFilePaths, ProgressCallback InProgressCallback) const
{
	auto Lamada = [=]()
	{
		GetObjectByUrl(
			InURL,
			InFilePaths,
			InProgressCallback);
	};

	(new FAutoDeleteAsyncTask<FOSSAsynTask>(Lamada))->StartBackgroundTask();
}

void FSimpleOSSManage::FOSS::AsyncPutObjectByUrl(const FString& InURL, const FString& InFilePaths, ProgressCallback InProgressCallback) const
{
	auto Lamada = [=]()
	{
		PutObjectByUrl(
			InURL,
			InFilePaths,
			InProgressCallback);
	};

	(new FAutoDeleteAsyncTask<FOSSAsynTask>(Lamada))->StartBackgroundTask();
}

bool FSimpleOSSManage::FOSS::GetObjectByUrl(const FString& InURL, const FString& InFilePaths, ProgressCallback InProgressCallback) const
{
	string URL = TCHAR_TO_UTF8(*InURL);
	string FilePaths = TCHAR_TO_UTF8(*InFilePaths);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	GetObjectByUrlRequest Request(URL);
	Request.setResponseStreamFactory([=]() {return std::make_shared<std::fstream>(FilePaths, std::ios_base::out | std::ios_base::in | std::ios_base::trunc); });

	TransferProgress progressCallback = { InProgressCallback , nullptr };
	Request.setTransferProgress(progressCallback);

	auto Outcome = Client.GetObjectByUrl(Request);
	if (!Outcome.isSuccess())
	{
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif

		return false;
	}

	return true;
}

bool FSimpleOSSManage::FOSS::PutObjectByUrl(const FString& InURL, const FString& InFilePaths, ProgressCallback InProgressCallback) const
{
	string URL = TCHAR_TO_UTF8(*InURL);
	string FilePaths = TCHAR_TO_UTF8(*InFilePaths);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	std::shared_ptr<std::iostream> content = std::make_shared<std::fstream>(FilePaths, std::ios::in | std::ios::binary);

	PutObjectByUrlRequest Request(URL, content);

	TransferProgress progressCallback = { InProgressCallback , nullptr };
	Request.setTransferProgress(progressCallback);

	auto Outcome = Client.PutObjectByUrl(Request);
	if (!Outcome.isSuccess())
	{
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif

		return false;
	}

	return true;
}

bool FSimpleOSSManage::FOSS::GetImageObjectByResize(const FVector2D &Size, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave)
{
	FString Command = FString::Printf(TEXT("image/resize,m_fixed,w_%i,h_%i"), (int32)Size.X, (int32)Size.Y);
	return GetImageObject(InBucketName, InObjectName, InFileNametoSave, Command);
}

bool FSimpleOSSManage::FOSS::GetImageObjectByCrop(const FVector4 &Size,int32 Ratio, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave)
{
	FString Command = FString::Printf(TEXT("image/crop,w_%i,h_%i,x_%i,y_%i,r_%i"), (int32)Size.X, (int32)Size.Y, (int32)Size.Z, (int32)Size.W, Ratio);
	return GetImageObject(InBucketName, InObjectName, InFileNametoSave, Command);
}

bool FSimpleOSSManage::FOSS::GetImageObjectBySharpen(const int32 Sharpen, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave)
{
	FString Command = FString::Printf(TEXT("image/sharpen,%d"), Sharpen);
	return GetImageObject(InBucketName, InObjectName, InFileNametoSave, Command);
}

bool FSimpleOSSManage::FOSS::GetImageObjectByWaterMark(const FString WaterMarkObjectName, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave)
{
	FString Command = FString::Printf(TEXT("image/watermark,%s"), *WaterMarkObjectName);
	return GetImageObject(InBucketName, InObjectName, InFileNametoSave, Command);
}

bool FSimpleOSSManage::FOSS::GetImageObjectByRotate(const int32 Rotate, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave)
{
	FString Command = FString::Printf(TEXT("image/rotate,%i"), Rotate);
	return GetImageObject(InBucketName, InObjectName, InFileNametoSave, Command);
}

bool FSimpleOSSManage::FOSS::GetImageObjectByStyle(const FString OSSStyleName, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave)
{
	FString Command = FString::Printf(TEXT("style/%s"), *OSSStyleName);
	return GetImageObject(InBucketName, InObjectName, InFileNametoSave, Command);
}

bool FSimpleOSSManage::FOSS::GetImageObjectByCascade(const TArray<FString> &CascadeCommand, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave)
{
	FString Command = "image";
	for (auto &Tmp : CascadeCommand)
	{
		Command /= Tmp;
	}

	return GetImageObject(InBucketName, InObjectName, InFileNametoSave, Command);
}

bool FSimpleOSSManage::FOSS::GetImageObject(const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave, const FString &Command)
{
	string BucketName = TCHAR_TO_UTF8(*InBucketName);
	string ObjectName = TCHAR_TO_UTF8(*InObjectName);
	string FileNametoSave = TCHAR_TO_UTF8(*InFileNametoSave);
	string Process	= TCHAR_TO_UTF8(*Command);

	FAuxiliaryTools  AuxiliaryTools;
	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));

	GetObjectRequest Requests(BucketName, ObjectName);

	Requests.setResponseStreamFactory([=]()
	{
		return std::make_shared<std::fstream>(
			FileNametoSave,
			std::ios_base::out |
			std::ios_base::in |
			std::ios_base::trunc |
			std::ios_base::binary);
	});
	Requests.setProcess(Process);

	auto Outcome = Client.GetObject(Requests);
	if (!Outcome.isSuccess())
	{
		/* 异常处理 */
#if WITH_EDITOR
		SIMPLEOSS_MACRO_ERROR(Outcome)
#endif
		return false;
	}

	return true;
}

FClientConfiguration::FClientConfiguration()
	:Scheme(OSS::Scheme::HTTP),
	MaxConnections(16),
	RequestTimeoutMs(10000),
	ConnectTimeoutMs(5000),
	ProxyScheme(OSS::Scheme::HTTP),
	ProxyPort(0),
	bVerifySSL(false),
	bCname(false),
	bEnableCrc64(true),
	bEnableDateSkewAdjustment(true),
	SendRateLimiter(0),
	RecvRateLimiter(0)
{

}