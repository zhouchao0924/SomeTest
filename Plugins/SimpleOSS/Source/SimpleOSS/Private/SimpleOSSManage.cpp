// Copyright (C) RenZhai.2019.All Rights Reserved.

#include "SimpleOSSManage.h"
#include "oss_api.h"
#include "aos_http_io.h"
#include "SimpleOSSMacro.h"
#include <memory>
#include <fstream>
#include "Async/AsyncWork.h"
#include "Misc/ScopeLock.h"
#include "Engine/Engine.h"
#include "Async/TaskGraphInterfaces.h"
#include "aos_string.h"
#include "oss_define.h"
#include "HAL/PlatformFilemanager.h"
#include "CoreGlobals.h"

#if PLATFORM_WINDOWS

#endif

//#ifdef _MSC_VER
//#pragma warning(push)
//#pragma warning(disable:4616)
//#pragma warning(disable:2005)
//#endif
#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif


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

		/*Call AOS HTTP IO initialize method at the program entry to initialize global resources such as network and memory */
		if (aos_http_io_initialize(NULL, 0) != AOSE_OK)
		{
			exit(1);
		}

		/**
		* Provide a local initialization account function, which needs to be configured in the project configuration.
		* For details, please refer to " ../../../Projectname/Config/OssLoadAccountsSettings.ini"
		*/
		SIMPLE_OSS.InitLocalAccounts();
	}

	return SimpleOSS;
}

void FSimpleOSSManage::Destroy()
{
	if (SimpleOSS)
	{
		//Save cofig
		SimpleOSS->GetOSS().SaveAccounts();

		aos_http_io_deinitialize();
		
		delete SimpleOSS;
	}

	SimpleOSS = nullptr;
}
//
//struct FAuxiliaryTools
//{
//	ClientConfiguration ConvertToOSSConf(const FClientConfiguration &InConf)
//	{
//		ClientConfiguration OSSConf;
//
//		FString NetworkInterface;
//		OSSConf.caFile = TCHAR_TO_UTF8(*InConf.CaFile);
//		OSSConf.scheme = (Http::Scheme)InConf.Scheme;
//		OSSConf.proxyScheme = (Http::Scheme)InConf.ProxyScheme;
//		OSSConf.maxConnections = InConf.MaxConnections;
//		OSSConf.requestTimeoutMs = InConf.RequestTimeoutMs;
//		OSSConf.enableDateSkewAdjustment = InConf.bEnableDateSkewAdjustment;
//		OSSConf.enableCrc64 = InConf.bEnableCrc64;
//		OSSConf.isCname = InConf.bCname;
//		OSSConf.verifySSL = InConf.bVerifySSL;
//		OSSConf.caPath = TCHAR_TO_UTF8(*InConf.CaPath);
//		OSSConf.proxyPassword = TCHAR_TO_UTF8(*InConf.ProxyPassword);
//		OSSConf.proxyUserName = TCHAR_TO_UTF8(*InConf.ProxyUserName);
//		OSSConf.proxyPort = InConf.ProxyPort;
//		OSSConf.connectTimeoutMs = InConf.ConnectTimeoutMs;
//		OSSConf.proxyHost = TCHAR_TO_UTF8(*InConf.ProxyHost);
//
//		if (InConf.RecvRateLimiter != 0)
//		{
//			std::shared_ptr<FSimpleOSSRateLimiter> Recv = std::make_shared<FSimpleOSSRateLimiter>();
//			Recv->setRate(InConf.RecvRateLimiter);
//			OSSConf.recvRateLimiter = Recv;
//		}
//
//		if (InConf.SendRateLimiter != 0)
//		{
//			std::shared_ptr<FSimpleOSSRateLimiter> Send = std::make_shared<FSimpleOSSRateLimiter>();
//			Send->setRate(InConf.SendRateLimiter);
//			OSSConf.sendRateLimiter = Send;
//		}
//
//		return OSSConf;
//	}
//
//	void Print(const FString &Msg, float Time = 10.f, FColor Color = FColor::Red)
//	{
//		if (GEngine)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, Time, Color, Msg);
//		}
//	}
//	void Print(const ANSICHAR* &Msg, float Time = 10.f, FColor Color = FColor::Red)
//	{
//		if (GEngine)
//		{
//			GEngine->AddOnScreenDebugMessage(-1, Time, Color, Msg.c_str());
//		}
//	}
//
//	int64_t GetFileSize(const ANSICHAR*& file)
//	{
//		std::fstream f(file, std::ios::in | std::ios::binary);
//		f.seekg(0, f.end);
//		int64_t size = f.tellg();
//		f.close();
//
//		return size;
//	}
//};

FString MakeFilenameToFolder(const FString &Dir)
{
	FString DirPoint = Dir;
	FPaths::NormalizeFilename(DirPoint);
	int32 i = INDEX_NONE;
	if (DirPoint.FindLastChar('/', i))
	{
		if (i != INDEX_NONE && i != (DirPoint.Len() - 1))
		{
			DirPoint /= TEXT("");
		}
	}

	return DirPoint;
}

FString NormalizePaths(const FString &InPaths, const FString &InFilename)
{
	FString Filename = FPaths::GetCleanFilename(InFilename);

	FString NewInlocalPaths = InPaths;
	if (!NewInlocalPaths.Contains(TEXT(".")))
	{
		int32 Position = NewInlocalPaths.Len() - 1;
		int32 i = 0;
		FPaths::NormalizeFilename(NewInlocalPaths);

		if (NewInlocalPaths.FindLastChar('/', i))
		{
			if (i == Position)
			{
				NewInlocalPaths += Filename;
			}
			else
			{
				NewInlocalPaths /= Filename;
			}
		}
		else
		{
			NewInlocalPaths /= Filename;
		}

		if (!InPaths.Contains(TEXT(":")))
		{
			checkf(NewInlocalPaths == InPaths, TEXT("This path is not valid. Please provide the correct path."));
		}	
	}

	return NewInlocalPaths;
}

EOSSStorageType GetStorageClass(ANSICHAR *ACLChar)
{
	FString ALCString = UTF8_TO_TCHAR(ACLChar);
	if (ALCString == "1")
	{
		return EOSSStorageType::ARCHIVE;
	}
	else if (ALCString == "2")
	{
		return EOSSStorageType::STANDARD;
	}
	else if (ALCString == "3")
	{
		return EOSSStorageType::IA;
	}

	return EOSSStorageType::NONE;
}

EAcl GetAcl(ANSICHAR *ACLChar,const FString &InBucketName = "")
{
	FString AclString = UTF8_TO_TCHAR(ACLChar);
	if (AclString == "private")
	{
		return EAcl::PRIVATE;
	}
	else if (AclString == "public-read")
	{
		return EAcl::PUBLICREAD;
	}
	else if (AclString == "public-read-write")
	{
		return EAcl::PUBLICREADWRITE;
	}
	else if (AclString == "default")
	{
		if (!InBucketName.IsEmpty())
		{
			return SIMPLE_OSS.GetBucketAcl(InBucketName);
		}	
	}

	return EAcl::NONE;
}

int64_t get_file_size(const ANSICHAR *file_path)
{
	int64_t filesize = -1;
	struct stat statbuff;
	if (stat(file_path, &statbuff) < 0)
	{
		return filesize;
	}
	else 
	{
		filesize = statbuff.st_size;
	}
	return filesize;
}

FString FSimpleOSSManage::FOSS::GetEndpoint()
{
	return UTF8_TO_TCHAR(Endpoint.c_str());
}

void FSimpleOSSManage::FOSS::InitAccounts(const FString &InAccessKeyId, const FString &InAccessKeySecret, const FString &InEndpoint)
{
	AccessKeyId = TCHAR_TO_UTF8(*InAccessKeyId);
	AccessKeySecret = TCHAR_TO_UTF8(*InAccessKeySecret);
	Endpoint = TCHAR_TO_UTF8(*InEndpoint);

	OSS_SUCCESS_MSG("Account infomation update succeeded through InitAccounts(const FString &InAccessKeyId, const FString &InAccessKeySecret, const FString &InEndpoint);->SimpleOSSManange.cpp [230]\n")
}

const TCHAR* Section = TEXT("OSSAccounts");
void FSimpleOSSManage::FOSS::InitLocalAccounts()
{
	OSS_REGISTRATION_CONFIGURATION

	if (GConfig)
	{
		FString InAccessKeyId;
		FString InAccessKeySecret;
		FString InEndpoint;

		GConfig->GetString(Section, TEXT("sg.AccessKeyId"), InAccessKeyId, AccountsSettingsPaths);
		GConfig->GetString(Section, TEXT("sg.AccessKeySecret"), InAccessKeySecret, AccountsSettingsPaths);
		GConfig->GetString(Section, TEXT("sg.Endpoint"), InEndpoint, AccountsSettingsPaths);

		AccessKeyId = TCHAR_TO_UTF8(*InAccessKeyId);
		AccessKeySecret = TCHAR_TO_UTF8(*InAccessKeySecret);
		Endpoint = TCHAR_TO_UTF8(*InEndpoint);

		if (!InAccessKeyId.IsEmpty() && !InAccessKeySecret.IsEmpty() && !InEndpoint.IsEmpty())
		{
			OSS_SUCCESS_MSG("Account initialization succeeded through local [%s]\n", *AccountsSettingsPaths)
		}	
	}
}

void FSimpleOSSManage::FOSS::SaveAccounts()
{
	OSS_REGISTRATION_CONFIGURATION

	if (GConfig)
	{
		GConfig->SetString(Section, TEXT("sg.AccessKeyId"), UTF8_TO_TCHAR(AccessKeyId.c_str()), AccountsSettingsPaths);
		GConfig->SetString(Section, TEXT("sg.AccessKeySecret"), UTF8_TO_TCHAR(AccessKeySecret.c_str()), AccountsSettingsPaths);
		GConfig->SetString(Section, TEXT("sg.Endpoint"), UTF8_TO_TCHAR(Endpoint.c_str()), AccountsSettingsPaths);
		GConfig->Flush(true, AccountsSettingsPaths);

		OSS_SUCCESS_MSG("Account save succeeded through local [%s]\n", *AccountsSettingsPaths)
	}
}

void FSimpleOSSManage::FOSS::InitConf(const FClientConfiguration &InConf)
{
	Conf = InConf;

	OSS_SUCCESS_MSG(" OSS configuration update.")
}

bool FSimpleOSSManage::FOSS::DoesBucketExist(const FString &InBucketName)
{
	return GetBucketAcl(InBucketName) != EAcl::NONE;
}

bool FSimpleOSSManage::FOSS::GetImageObject_Internal(const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave, const FString &Command)
{
	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	STRING_TO_AOS_STRING_T(object, InObjectName);
	STRING_TO_AOS_STRING_T(file, InFileNametoSave);
	CREATE_CONFIGURATION_AND_STORAGE;

	aos_table_t *headers = NULL;
	aos_table_t *params = NULL;
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;

	/* zoom */
	params = aos_table_make(pool, 1);
	apr_table_set(params, /*OSS_PROCESS*/ "x-oss-process",/* "image/resize,m_fixed,w_100,h_100"*/ TCHAR_TO_UTF8(*Command));

	/*Download the processed pictures to the local file. */
	resp_status = oss_get_object_to_file(oss_client_options, &bucket, &object, headers, params, &file, &resp_headers);
	CHECK_OSS_RESULT(
		OSS_SUCCESS_MSG("get object to file succeeded\n"),
		OSS_ERROR_MSG("get object to file failed\n"))

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

bool FSimpleOSSManage::FOSS::GetImageObject(const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave, const FString &Command)
{
	const FString NewInlocalPaths = NormalizePaths(InFileNametoSave, InObjectName);

	return GetImageObject_Internal(InBucketName, InObjectName, NewInlocalPaths, Command);
}

void FSimpleOSSManage::FOSS::init_options(void *inoptions)
{
	oss_request_options_t* options = (oss_request_options_t*)inoptions;
	options->config = oss_config_create(options->pool);

	/*Initializes the aos_string_t type with a string of type ansichar *. */
	aos_str_set(&options->config->endpoint, Endpoint.c_str());
	aos_str_set(&options->config->access_key_id, AccessKeyId.c_str());
	aos_str_set(&options->config->access_key_secret, AccessKeySecret.c_str());

	/*Whether to use CNAME. 0 means not used */
	options->config->is_cname = Conf.bCname;

	/*Set network related parameters, such as timeout. */
	options->ctl = aos_http_controller_create(options->pool, 0);

	/*Set the link timeout, which is 10 seconds by default. */
	options->ctl->options->connect_timeout = Conf.ConnectTimeoutS;

	/*Set DNS timeout, default is 60 seconds. */
	options->ctl->options->dns_cache_timeout = Conf.DNSTimeoutS;

	/*
	Set request timeout:
	The minimum rate that can be tolerated is controlled by setting the value of speed limit, which is 1024 by default, i.e. 1KB / s.            
	The maximum time that can be tolerated is controlled by setting the value of speed "time, which is 15 seconds by default.            
	Indicates a timeout if the transfer rate is less than 1KB / s for 15 seconds. 
	*/
	options->ctl->options->speed_limit = Conf.MinimumRateOfTolerance;
	options->ctl->options->speed_time = Conf.MinimumRateOfToleranceTime;
}

ANSICHAR* FSimpleOSSManage::FOSS::multibyte_to_utf8(const ANSICHAR * ch, ANSICHAR *str, int32 size)
{
	if (!ch || !str || size <= 0)
	{
		return NULL;
	}

	int32 chlen = strlen(ch);
	int32 multi_byte_cnt = 0;
	for (int32 i = 0; i < chlen - 1; i++)
	{
		if ((ch[i] & 0x80) && (ch[i + 1] & 0x80)) 
		{
			i++;
			multi_byte_cnt++;
		}
	}

	if (multi_byte_cnt == 0)
	{
		return const_cast<ANSICHAR*>(ch);
	}

	int32 len = MultiByteToWideChar(CP_ACP, 0, ch, -1, NULL, 0);
	if (len <= 0)
	{
		return NULL;
	}

	WIDECHAR *wch = (WIDECHAR *)malloc(sizeof(WIDECHAR) * (len + 1));
	if (!wch)
	{
		return NULL;
	}

	wmemset(wch, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, ch, -1, wch, len);

	len = WideCharToMultiByte(CP_UTF8, 0, wch, -1, NULL, 0, NULL, NULL);
	if ((len <= 0) || (size < len + 1)) 
	{
		free(wch);

		return NULL;
	}
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wch, -1, str, len, NULL, NULL);
	free(wch);

	return str;
}

bool FSimpleOSSManage::FOSS::CreateBucket(const FString &InBucketName, EOSSStorageType OSSStorageType , EAcl Acl)
{
	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	CREATE_CONFIGURATION_AND_STORAGE;

	if (OSSStorageType == EOSSStorageType::NONE)
	{
		OSSStorageType = EOSSStorageType::STANDARD;
	}

	if (Acl == EAcl::NONE)
	{
		Acl = EAcl::PRIVATE;
	}

	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;

//	resp_status = oss_create_bucket(oss_client_options, &bucket, (oss_acl_e)Acl, &resp_headers);
	resp_status = oss_create_bucket_with_storage_class(oss_client_options, &bucket, (oss_acl_e)Acl,(oss_storage_class_type_e)OSSStorageType, nullptr);

	/*Determine whether the upload is successful. */
	CHECK_OSS_RESULT(
		OSS_SUCCESS_MSG("Create a bucket called %s", *InBucketName),
		OSS_ERROR_MSG("Failed to create a bucket %s,It is possible that the account is wrong or the bucket with the same name already exists. It is also possible that the bucket name contains capitals or the name is not standardized",*InBucketName));

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

bool FSimpleOSSManage::FOSS::ListBuckets(TArray<FString> &InBucketName)
{
	CREATE_CONFIGURATION_AND_STORAGE;

	aos_table_t *resp_headers = NULL;
	oss_list_buckets_params_t *params = NULL;
	oss_list_bucket_content_t *content = NULL;
	int32 size = 0;
	params = oss_create_list_buckets_params(pool);

	aos_status_t *resp_status = oss_list_bucket(oss_client_options, params, &resp_headers);

	/*Determine whether the upload is successful. */
	CHECK_OSS_RESULT(
		/*Print storage space */ 
		aos_list_for_each_entry(oss_list_bucket_content_t, content, &params->bucket_list, node)
		{
			OSS_SUCCESS_MSG("Find bucket %s from OSS server .",UTF8_TO_TCHAR(content->name.data))
			InBucketName.Add(UTF8_TO_TCHAR(content->name.data));
			++size;
		},
		OSS_ERROR_MSG("Unable to list all buckets through the account provided please check if the oss account is correct."));

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

bool FSimpleOSSManage::FOSS::GetBucketsInfo(const FString &InBucketName, FBucketInfo &InBucketInfo)
{
	STRING_TO_AOS_STRING_T(bucket,InBucketName);
	CREATE_CONFIGURATION_AND_STORAGE;

	oss_bucket_info_t bucket_info;
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;

	resp_status = oss_get_bucket_info(oss_client_options, &bucket, &bucket_info, &resp_headers);

	/*Determine whether the upload is successful. */
	CHECK_OSS_RESULT(
		{
			InBucketInfo.Created = UTF8_TO_TCHAR(bucket_info.created_date.data);
			InBucketInfo.Acl = GetAcl(bucket_info.acl.data);
			InBucketInfo.OwnerID = UTF8_TO_TCHAR(bucket_info.owner_id.data);
			InBucketInfo.OwnerName = UTF8_TO_TCHAR(bucket_info.owner_name.data);
			InBucketInfo.ExtranetEndpoint = UTF8_TO_TCHAR(bucket_info.extranet_endpoint.data);
			InBucketInfo.IntranetEndpoint = UTF8_TO_TCHAR(bucket_info.intranet_endpoint.data);
			InBucketInfo.Location = UTF8_TO_TCHAR(bucket_info.location.data);

			OSS_SUCCESS_MSG("Bucket %s information obtained successfully.", *InBucketName)
		}
		, OSS_ERROR_MSG("Failed to get bucket %s information. Check whether the OSS account is correct or the network is connected or the bucket name is correct .",*InBucketName));

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}


//int64 FSimpleOSSManage::FOSS::GetBucketsStorageCapacity(const FString &InBucketName)
//{
//	long storage_capacity = INDEX_NONE;
//
//	/*Assign data of type ANSICHAR * to the storage space of type AOS ﹣ string ﹣ t */
//	STRING_TO_AOS_STRING_T(bucket, InBucketName);
//	CREATE_CONFIGURATION_AND_STORAGE;
//
//	/* 初始化参数。*/
//	aos_table_t *resp_headers = NULL;
//	aos_status_t *resp_status = NULL;
//
//	/*Get the region of the storage space. */
//	resp_status = oss_get_bucket_storage_capacity(oss_client_options, &bucket, &storage_capacity, &resp_headers);
//	/*Determine whether the upload is successful. */
//	CHECK_OSS_RESULT(
//		OSS_SUCCESS_MSG("get bucket storage capacity succeeded : %i \n", storage_capacity)
//	,
//		OSS_ERROR_MSG("get bucket capacity succeeded failed\n"))
//
//		/*Get response message of OSS server */
//		OSS_SDK_RESPONSE_MSG
//
//	return storage_capacity;
//}

FString FSimpleOSSManage::FOSS::GetBucketLocation(const FString &InBucketName)
{
	FString OssLocation;

	/*Assign data of type ANSICHAR * to the storage space of type AOS ﹣ string ﹣ t */
	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	CREATE_CONFIGURATION_AND_STORAGE;

	/* 初始化参数。*/
	aos_string_t oss_location;
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;

	/*Get the region of the storage space. */
	resp_status = oss_get_bucket_location(oss_client_options, &bucket, &oss_location, &resp_headers);
	/*Determine whether the upload is successful. */
	CHECK_OSS_RESULT(
		OSS_SUCCESS_MSG("get bucket location succeeded : %s \n", UTF8_TO_TCHAR(oss_location.data))
		OssLocation = UTF8_TO_TCHAR(oss_location.data);
		,
		OSS_ERROR_MSG("get bucket location failed\n"))

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return OssLocation;
}

bool FSimpleOSSManage::FOSS::DeleteBucket(const FString &InBucketName)
{
	int32 Number = 1000;

	TArray<FMultipartPartsUploadsInfo> MultipartPartsUploadsInfo;
	if (ListMultipartUploads(InBucketName, MultipartPartsUploadsInfo, Number))
	{
		//Delete fragment first 
		for (auto &Tmp : MultipartPartsUploadsInfo)
		{
			AbortMultipartUpload(InBucketName, Tmp.Key, Tmp.UploadId);
		}

		//List objects 
		TArray<FOSSObjectInfo> OSSObjectInfo;
		if (ListObjects(InBucketName, OSSObjectInfo,Number))
		{
			TArray<FString> ObjectName;
			for (auto &Tmp : OSSObjectInfo)
			{
				ObjectName.Add(Tmp.Name);
			}
			DeleteObjects(InBucketName, ObjectName);

			STRING_TO_AOS_STRING_T(bucket, InBucketName);
			CREATE_CONFIGURATION_AND_STORAGE;

			aos_table_t *resp_headers = NULL;
			aos_status_t *resp_status = NULL;

			resp_status = oss_delete_bucket(oss_client_options, &bucket, &resp_headers);

			/*Determine whether the upload is successful. */
			CHECK_OSS_RESULT(
				OSS_SUCCESS_MSG("delete bucket succeeded\n");
			,
				OSS_ERROR_MSG("delete bucket failed\n"););

			/*Get response message of OSS server */
			OSS_SDK_RESPONSE_MSG

			return bVisitSuccess;
		}

		OSS_ERROR_MSG("Unable to successfully object of the bucket  %s", *InBucketName)
	}

	OSS_ERROR_MSG("Unable to successfully enumerate fragments of the bucket  %s",*InBucketName)
	return false;
}

bool FSimpleOSSManage::FOSS::SetBucketAcl(const FString &InBucketName, EAcl BucketAcl)
{
	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	CREATE_CONFIGURATION_AND_STORAGE;

	if (BucketAcl == EAcl::NONE)
	{
		BucketAcl = EAcl::PRIVATE;
	}

	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;
	/*Assign ansichar * type data to storage space of type AOS ﹐ string ﹐ t */
	aos_str_set(&bucket, bucket_name);

	//Set the storage space permission to public read (OSS ACL public read). 
	resp_status = oss_put_bucket_acl(oss_client_options, &bucket, (oss_acl_e)BucketAcl, &resp_headers);

	/*Determine whether the upload is successful. */
	CHECK_OSS_RESULT(
		OSS_SUCCESS_MSG("Permission of bucket set successfully ."),
		OSS_ERROR_MSG("Failed to set %s permissions .",*InBucketName));

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

EAcl FSimpleOSSManage::FOSS::GetBucketAcl(const FString InBucketName)
{
	EAcl Acl = EAcl::NONE;

	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	CREATE_CONFIGURATION_AND_STORAGE;

	aos_string_t oss_acl;
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;

	resp_status = oss_get_bucket_acl(oss_client_options, &bucket, &oss_acl, &resp_headers);

	/*Determine whether the upload is successful. */
	CHECK_OSS_RESULT(
		OSS_SUCCESS_MSG("Permission of bucket get successfully .")
		Acl = GetAcl(oss_acl.data);,
		OSS_WARNING_MSG("Unable to get ownership of the %s, it may not exist on the server .", *InBucketName));

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return Acl;
}

//bool FSimpleOSSManage::FOSS::SetBucketRequestPayment(const FString &InBucketName, ERequestPayerType InRequestPayer)
//{
//	ANSICHAR* bucket_name = TCHAR_TO_UTF8(*InBucketName);
//
//	FAuxiliaryTools  AuxiliaryTools;
//	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));
//
//	SetBucketRequestPaymentRequest Request(bucket_name);
//	Request.setRequestPayer((RequestPayer)InRequestPayer);
//	auto Outcome = Client.SetBucketRequestPayment(Request);
//
//	if (!Outcome.isSuccess())
//	{
//#if WITH_EDITOR
//		SIMPLEOSS_MACRO_ERROR(Outcome)
//#endif
//			return false;
//	}
//
//	return true;
//}
//
//ERequestPayerType FSimpleOSSManage::FOSS::GetBucketRequestPayment(const FString &InBucketName)
//{
//	ANSICHAR* bucket_name = TCHAR_TO_UTF8(*InBucketName);
//
//	FAuxiliaryTools  AuxiliaryTools;
//	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));
//
//	/* 获取请求者付费模式 */
//	GetBucketRequestPaymentRequest Request(bucket_name);
//	auto Outcome = Client.GetBucketRequestPayment(Request);
//
//	if (!Outcome.isSuccess())
//	{
//#if WITH_EDITOR
//		SIMPLEOSS_MACRO_ERROR(Outcome)
//#endif
//		return ERequestPayerType::NONE;
//	}
//
//	return (ERequestPayerType)Outcome.result().Payer();
//}

bool FSimpleOSSManage::FOSS::SetBucketReferer(const FString &InBucketName, const TArray<FString> &URLReferer, bool bAllowEmptyRefere /*= false*/)
{
	CREATE_CONFIGURATION_AND_STORAGE;
	STRING_TO_AOS_STRING_T(bucket, InBucketName);

	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;
	oss_referer_config_t referer_config;

	aos_list_init(&referer_config.referer_list);
	for (const auto &Tmp : URLReferer)
	{
		//Eam "http://renzhai.net"
		oss_create_and_add_refer(pool, &referer_config,TCHAR_TO_UTF8(*Tmp));
	}

	referer_config.allow_empty_referer = bAllowEmptyRefere;

	/*Add the referer white list. The referer parameter supports the wildcard asterisk (*) and question mark (?). */
	resp_status = oss_put_bucket_referer(oss_client_options, &bucket, &referer_config, &resp_headers);
	CHECK_OSS_RESULT(
		OSS_SUCCESS_MSG("put bucket %s referer succeeded\n", *InBucketName),
		OSS_ERROR_MSG("put bucket %s referer failed\n", *InBucketName) )

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

bool FSimpleOSSManage::FOSS::GetBucketReferer(const FString &InBucketName, TArray<FString> &BucketReferers)
{
	CREATE_CONFIGURATION_AND_STORAGE;
	STRING_TO_AOS_STRING_T(bucket, InBucketName);

	/* 初始化参数。*/
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;
	oss_referer_config_t referer_config;
	oss_referer_t *referer;
	aos_list_init(&referer_config.referer_list);

	/*Get the referer white list. */
	resp_status = oss_get_bucket_referer(oss_client_options, &bucket, &referer_config, &resp_headers);
	CHECK_OSS_RESULT(
		aos_list_for_each_entry(oss_referer_t, referer, &referer_config.referer_list, node)
		{
			BucketReferers.Add(UTF8_TO_TCHAR(referer->referer.data));
			OSS_SUCCESS_MSG("get %s referer %s\n", *InBucketName)
		}	
		,
		OSS_ERROR_MSG("get %s referer failed\n", *InBucketName))

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

bool FSimpleOSSManage::FOSS::DeleteBucketReferer(const FString &InBucketName, bool bAllowEmptyRefere /*= true*/)
{
	CREATE_CONFIGURATION_AND_STORAGE;
	STRING_TO_AOS_STRING_T(bucket, InBucketName);

	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;
	oss_referer_config_t referer_config;
	aos_list_init(&referer_config.referer_list);
	referer_config.allow_empty_referer = bAllowEmptyRefere;

	/*The anti-theft chain cannot be emptied directly. You need to create a new rule that allows null referers to override the previous rule .*/
	resp_status = oss_put_bucket_referer(oss_client_options, &bucket, &referer_config, &resp_headers);
	CHECK_OSS_RESULT(
		OSS_SUCCESS_MSG("delete bucket %s referer succeeded\n", *InBucketName)
		,
		OSS_ERROR_MSG("delete bucket %s referer failed\n", *InBucketName));

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

bool FSimpleOSSManage::FOSS::GetObjectMeta(const FString &InBucketName, const FString &InObjectName, FOSSObjectMeta &OSSObjectInfo)
{	
	CREATE_CONFIGURATION_AND_STORAGE;

	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	STRING_TO_AOS_STRING_T(object, InObjectName);

	aos_table_t *headers = nullptr;
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;

	/*Get file permissions. */
	resp_status = oss_get_object_meta(oss_client_options, &bucket, &object, &resp_headers);

	auto InitPram = [&]()
	{
		ANSICHAR* content_length_str = (ANSICHAR*)apr_table_get(resp_headers, "Content-Length");
		if (content_length_str != NULL)
		{
			OSSObjectInfo.ContentLength = atol(content_length_str);
		}

		content_length_str = (ANSICHAR*)apr_table_get(resp_headers, "x-oss-hash-crc64ecma");
		if (content_length_str != NULL)
		{
			OSSObjectInfo.Crc64 = atol(content_length_str);
		}

	//	OSSObjectInfo.Location = UTF8_TO_TCHAR((ANSICHAR*)apr_table_get(resp_headers, "Location"));
	//	OSSObjectInfo.UploadId = UTF8_TO_TCHAR((ANSICHAR*)apr_table_get(resp_headers, "uploadId"));
		OSSObjectInfo.Date = UTF8_TO_TCHAR((ANSICHAR*)apr_table_get(resp_headers, "Date"));
	//	OSSObjectInfo.Authorization = UTF8_TO_TCHAR((ANSICHAR*)apr_table_get(resp_headers, "Authorization"));
	//	OSSObjectInfo.XOssObjectType = UTF8_TO_TCHAR((ANSICHAR*)apr_table_get(resp_headers, "x-oss-object-type"));
	//	OSSObjectInfo.Acl = GetAcl((ANSICHAR*)apr_table_get(resp_headers, "acl"));
	//	OSSObjectInfo.StorageClass = GetStorageClass((ANSICHAR*)apr_table_get(resp_headers, "StorageCapacity"));
	//	OSSObjectInfo.ContentType = UTF8_TO_TCHAR((ANSICHAR*)apr_table_get(resp_headers, "Content-Type"));
	//	OSSObjectInfo.ContentMd5 = UTF8_TO_TCHAR((ANSICHAR*)apr_table_get(resp_headers, "Content-MD5"));
	//	OSSObjectInfo.Tag = UTF8_TO_TCHAR((ANSICHAR*)apr_table_get(resp_headers, "tagging"));
	};

	CHECK_OSS_RESULT(InitPram();
	OSS_SUCCESS_MSG("get object meta succeeded, content_length:%ld\n", OSSObjectInfo.ContentLength)
	,
	OSS_ERROR_MSG("req:%s, get object meta failed\n",resp_status->req_id)
	);

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

EAcl FSimpleOSSManage::FOSS::GetObjectAcl(const FString &InBucketName, const FString &InObjectName)
{
	CREATE_CONFIGURATION_AND_STORAGE;

	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	STRING_TO_AOS_STRING_T(object, InObjectName);

	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;

	/* 获取文件权限。*/
	aos_string_t oss_acl_string;
	resp_status = oss_get_object_acl(oss_client_options, &bucket, &object, &oss_acl_string, &resp_headers);

	/*Determine whether the upload is successful. */
	CHECK_OSS_RESULT(
		OSS_SUCCESS_MSG("get %s->%s object acl success!\n", *InBucketName, *InObjectName),
		OSS_ERROR_MSG("get %s->%s object acl failed!\n", *InBucketName, *InObjectName))

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return GetAcl(oss_acl_string.data,InBucketName);
}

bool FSimpleOSSManage::FOSS::SetObjectAcl(const FString &InBucketName, const FString &InObjectName, EAcl InAcl)
{
	CREATE_CONFIGURATION_AND_STORAGE;

	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	STRING_TO_AOS_STRING_T(object, InObjectName);

	if (InAcl == EAcl::NONE)
	{
		InAcl = EAcl::PRIVATE;
	}

	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;

	/* 设置文件权限。*/
	resp_status = oss_put_object_acl(oss_client_options, &bucket, &object, (oss_acl_e)InAcl, &resp_headers);

	/*Determine whether the upload is successful. */
	CHECK_OSS_RESULT(
		OSS_SUCCESS_MSG("put %s->%s object acl success!\n", *InBucketName, *InObjectName),
		OSS_ERROR_MSG("put %s->%s object acl failed!\n", *InBucketName, *InObjectName))

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

bool FSimpleOSSManage::FOSS::DoesObjectExist(const FString &InBucketName, const FString &InObjectName)
{
	return GetObjectAcl(InBucketName, InObjectName) != EAcl::NONE;
}

bool FSimpleOSSManage::FOSS::ListObjectsByPrefix(const FString &InBucketName, const FString &Prefix, TArray<FOSSObjectInfo> &ObjectName, int32 MaxNumber)
{
	return ListObjects_Internal(InBucketName, "/", Prefix,  ObjectName, MaxNumber);
}

bool FSimpleOSSManage::FOSS::RecursiveListObjectsByPrefix(const FString &InBucketName, const FString &Prefix, TArray<FOSSObjectInfo> &ObjectName, int32 MaxNumber /*= 100*/)
{
	return ListObjects_Internal(InBucketName,"" ,Prefix, ObjectName, MaxNumber);
}

bool FSimpleOSSManage::FOSS::ListObjects(const FString &InBucketName, TArray<FOSSObjectInfo> &ObjectName, int32 MaxNumber /*= 100*/)
{
	return ListObjects_Internal(InBucketName,"","",ObjectName, MaxNumber);
}

//bool FSimpleOSSManage::FOSS::SetObjectStorageType(const FString &InBucketName, const FString &InObjectName, EOSSStorageType OSSStorageType)
//{
//	if (OSSStorageType == EOSSStorageType::NONE)
//	{
//		OSSStorageType = EOSSStorageType::STANDARD;
//	}
//
//	EOSSStorageType StorageType = GetObjectStorageType(InBucketName, InObjectName);
//	if (StorageType != EOSSStorageType::NONE)
//	{
//		auto SSStorage = [](EOSSStorageType OSSStorageType)->ANSICHAR*
//		{
//			switch (OSSStorageType)
//			{
//			case EOSSStorageType::STANDARD:
//				return "Standard";
//			case EOSSStorageType::IA:
//				return "IA";
//			case EOSSStorageType::ARCHIVE:
//				return "Archive";
//			}
//
//			return "";
//		};
//		CREATE_CONFIGURATION_AND_STORAGE;
//
//		STRING_TO_AOS_STRING_T(bucket, InBucketName);
//		STRING_TO_AOS_STRING_T(object, InObjectName);
//
//		aos_list_t buffer;
//		oss_acl_e oss_acl = OSS_ACL_PRIVATE;
//		aos_buf_t *content = NULL;
//		aos_table_t *headers = NULL;
//		aos_table_t *resp_headers = NULL;
//		aos_status_t *resp_status = NULL;
//		headers = aos_table_make(pool, 0);
//		STATEMENT_VISITSUCCESS
//
//		if (StorageType == EOSSStorageType::ARCHIVE)
//		{
//			/*Create archive storage space. */
//			resp_status = oss_create_bucket_with_storage_class(oss_client_options, &bucket, oss_acl, (oss_storage_class_type_e)OSSStorageType, &resp_headers);
//			
//			CHECK_OSS_RESULT_NOBOOL(
//				OSS_SUCCESS_MSG("create %s bucket succeeded\n",*InBucketName),
//				OSS_ERROR_MSG("create %s bucket failed\n",*InBucketName))
//
//			aos_list_init(&buffer);
//
//			/*Unfreeze files. */
//			do {
//				headers = aos_table_make(pool, 0);
//				resp_status = oss_restore_object(oss_client_options, &bucket, &object, headers, &resp_headers);
//				OSS_INFO_MSG("restore object resp_status->code: %d \n", resp_status->code);
//				if (resp_status->code != 409)
//				{
//					break;
//				}
//				else 
//				{
//					OSS_INFO_MSG("restore object is already in progress, resp_status->code: %d \n", resp_status->code);
//					apr_sleep(5000);
//				}
//			} while (1);
//		}
//
//		headers = aos_table_make(pool, 1);
//		apr_table_set(headers, "StorageClass", SSStorage(OSSStorageType));
//
//		/*Copy files. */
//		resp_status = oss_copy_object(oss_client_options, &bucket, &object, &bucket, &object, headers, &resp_headers);
//
//		CHECK_OSS_RESULT_NOBOOL(
//			OSS_SUCCESS_MSG("Set %s bucket storage type succeeded\n", *InBucketName),
//			OSS_ERROR_MSG("create %s bucket storage type failed\n", *InBucketName))
//
//		AOS_POOL_DESTROY_IT(pool);
//
//		/*Get response message of OSS server */
//		OSS_SDK_RESPONSE_MSG
//
//		return bVisitSuccess;
//	}
//
//	OSS_ERROR_MSG("create %s bucket storage type failed,Unable to get metadata of bucket. Please check the network connection or whether the account is stored incorrectly \n", *InBucketName)
//	return false;
//}
//
//EOSSStorageType FSimpleOSSManage::FOSS::GetObjectStorageType(const FString &InBucketName, const FString &InObjectName)
//{
//	FOSSObjectMeta OSSObjectInfo;
//	if (GetObjectMeta(InBucketName, InObjectName, OSSObjectInfo))
//	{
//		return OSSObjectInfo.StorageClass;
//	}
//
//	return EOSSStorageType::NONE;
//}

bool FSimpleOSSManage::FOSS::CreateSymlink(const FString &InBucketName, const FString &InObjectName, const FString &InLinkObjectName)
{
	CREATE_CONFIGURATION_AND_STORAGE;

	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	STRING_TO_AOS_STRING_T(object, InObjectName);
	STRING_TO_AOS_STRING_T(sym_object, InLinkObjectName);

	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;

	resp_status = oss_put_symlink(oss_client_options, &bucket, &sym_object, &object, &resp_headers);

	/*Determine whether the upload is successful. */
	CHECK_OSS_RESULT(
		OSS_SUCCESS_MSG("put %s->%s->%s symlink succeeded\n",* InBucketName,*InObjectName,*InLinkObjectName),
		OSS_ERROR_MSG("put %s->%s->%s symlink failed\n", *InBucketName, *InObjectName, *InLinkObjectName))

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

FString FSimpleOSSManage::FOSS::GetSymlink(const FString &InBucketName, const FString &InLinkName)
{
	CREATE_CONFIGURATION_AND_STORAGE;

	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	STRING_TO_AOS_STRING_T(link_object, InLinkName);

	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;
	ANSICHAR *target_object_name = NULL;
	
	resp_status = oss_get_symlink(oss_client_options, &bucket, &link_object, &resp_headers);

	/*Determine whether the upload is successful. */
	CHECK_OSS_RESULT(
		target_object_name = (ANSICHAR*)(apr_table_get(resp_headers,"x-oss-symlink-target")); 
		OSS_SUCCESS_MSG("get symlink succeeded\n"),
		OSS_ERROR_MSG("target_object_name: %s \n", target_object_name))

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return ANSI_TO_TCHAR(target_object_name);
}

bool FSimpleOSSManage::FOSS::GetObjectToMemory(
	const FString &InBucketName, 
	const FString &InObjectName, 
	FString &ContextString, 
	FRange Range,
	ProgressCallback InProgressCallback /*= nullptr*/)
{
	FScopeLock ScopeLock(&Mutex);

	CREATE_CONFIGURATION_AND_STORAGE

	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	STRING_TO_AOS_STRING_T(object, InObjectName);

	aos_list_t buffer;
	aos_buf_t *content = NULL;
	aos_table_t *params = NULL;
	aos_table_t *headers = NULL;
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;
	ANSICHAR *buf = NULL;
	int64_t len = 0;
	int64_t size = 0;
	int64_t pos = 0;
	aos_list_init(&buffer);

	/* set range*/
	if (Range.IsValid())
	{
		headers = aos_table_make(pool, 0);
		apr_table_set(headers, "Range", TCHAR_TO_ANSI(*Range.ToString()));
	}

	/*Download files to local memory. */
	if (InProgressCallback)
	{
		resp_status = oss_do_get_object_to_buffer(oss_client_options, &bucket, &object,
			headers, params, &buffer, InProgressCallback, &resp_headers);
	}
	else
	{
		resp_status = oss_get_object_to_buffer(oss_client_options, &bucket, &object,
			headers, params, &buffer, &resp_headers);
	}

	auto DownLoadString =[&]()
	{
		/*Copy the download to the buffer. */
		len = aos_buf_list_len(&buffer);
		buf = (ANSICHAR *)aos_pcalloc(pool, len + 1);
		buf[len] = '\0';

		aos_list_for_each_entry(aos_buf_t, content, &buffer, node)
		{
			size = aos_buf_size(content);
			memcpy(buf + pos, content->pos, size);
			pos += size;
		}

		ContextString = ANSI_TO_TCHAR(buf);
	};

	/*Determine whether the upload is successful. */
	CHECK_OSS_RESULT(DownLoadString();
		OSS_SUCCESS_MSG("get object to buffer succeeded\n");,
		OSS_ERROR_MSG("get object to buffer failed\n"););

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

bool FSimpleOSSManage::FOSS::GetObjectToLocal(
	const FString &InBucketName, 
	const FString &InObjectName,
	const FString &InLocalPaths,
	FRange Range,
	ProgressCallback InProgressCallback /*= nullptr*/)
{
	FScopeLock ScopeLock(&Mutex);

	const FString NewInlocalPaths = NormalizePaths(InLocalPaths, InObjectName);

	CREATE_CONFIGURATION_AND_STORAGE

	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	STRING_TO_AOS_STRING_T(object, InObjectName);
	UTF8_STRING_TO_AOS_STRING_T(file, NewInlocalPaths); //Avoid Chinese characters being unable to upload 

	/* 初始化参数。*/
	aos_table_t *params = nullptr;
	aos_table_t *headers = NULL;
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;

	/* 设置Range*/
	if (Range.IsValid())
	{
		headers = aos_table_make(pool, 0);
		apr_table_set(headers, "Range", TCHAR_TO_ANSI(*Range.ToString()));
	}

	params = aos_table_make(pool,0);

	/* 下载文件。如果指定的本地文件存在会覆盖，不存在则新建。*/
	if (InProgressCallback)
	{
		resp_status = oss_do_get_object_to_file(oss_client_options, &bucket, &object, headers, params, &file, InProgressCallback, &resp_headers);
	}
	else
	{
		resp_status = oss_get_object_to_file(oss_client_options, &bucket, &object, headers, params, &file, &resp_headers);
	}

	/*Determine whether the upload is successful. */
	CHECK_OSS_RESULT(
		OSS_SUCCESS_MSG("get %s object to [%s] succeeded\n", *InObjectName,*InLocalPaths); ,
		OSS_ERROR_MSG("get %s object to [%s] failed\n",*InObjectName,*InLocalPaths););

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

#include <thread>
bool FSimpleOSSManage::FOSS::ResumableDownloadObject(
	const FString &InBucketName,
	const FString &InObjectName,
	const FString &InDownloadFilePath,
	int32 PartSize /*= 1024 * 1024 * 10*/,
	ProgressCallback InProgressCallback /*= nullptr*/)
	/*const FString &InCheckpointFilePath*/ /*= FPaths::ProjectSavedDir() / TEXT("OSSCache")*/
{
	FScopeLock ScopeLock(&Mutex);

	//FString CheckpointFilePathPoint = MakeFilenameToFolder(InCheckpointFilePath);
	//CheckpointFilePathPoint.RemoveFromEnd(TEXT("/"));
	const FString NewInlocalPaths = NormalizePaths(InDownloadFilePath, InObjectName);

	CREATE_CONFIGURATION_AND_STORAGE

	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	STRING_TO_AOS_STRING_T(object, InObjectName);
	UTF8_STRING_TO_AOS_STRING_T(file, NewInlocalPaths); //Avoid Chinese characters being unable to upload 

	aos_table_t *headers = NULL;
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;
	oss_resumable_clt_params_t *clt_params;

	//string CheckpointFilePathPointStr = TCHAR_TO_UTF8(*CheckpointFilePathPoint);
	int32 ThreadNumber = std::thread::hardware_concurrency();

	clt_params = oss_create_resumable_clt_params_content(pool, PartSize, ThreadNumber, AOS_TRUE,/* CheckpointFilePathPointStr.c_str()*/NULL);

	resp_status = oss_resumable_download_file(oss_client_options, &bucket, &object, &file, headers, NULL, clt_params, InProgressCallback, &resp_headers);

	/*Determine whether the upload is successful. */
	CHECK_OSS_RESULT(
	OSS_SUCCESS_MSG("Resumable download %s->[%s] succeeded\n", *InObjectName, *InDownloadFilePath);
	, 
	OSS_ERROR_MSG("Resumable download %s->[%s]  failed\n", *InObjectName, *InDownloadFilePath));

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

bool FSimpleOSSManage::FOSS::CopyObject(
	const FString &InSourceBucketName, 
	const FString &InSourceObjectName, 
	const FString &InCopyBucketName, 
	const FString &InCopyObjectName)
{
	FScopeLock ScopeLock(&Mutex);

	CREATE_CONFIGURATION_AND_STORAGE

	STRING_TO_AOS_STRING_T(source_bucket, InSourceBucketName);
	STRING_TO_AOS_STRING_T(source_object, InSourceObjectName);
	STRING_TO_AOS_STRING_T(dest_bucket, InCopyBucketName);
	STRING_TO_AOS_STRING_T(dest_object, InCopyObjectName);
	
	aos_table_t *headers = NULL;
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;

	headers = aos_table_make(pool, 0);

	/* 拷贝文件。*/
	resp_status = oss_copy_object(oss_client_options, &source_bucket, &source_object, &dest_bucket, &dest_object, headers, &resp_headers);

	/*Determine whether the upload is successful. */
	CHECK_OSS_RESULT(
	OSS_SUCCESS_MSG("copy object succeeded\n");
	,
	OSS_ERROR_MSG("copy object failed\n"););
	
	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}
//
//bool FSimpleOSSManage::FOSS::CopyPart(FString &UploadID, const FString &InSourceBucketName, const FString &InSourceObjectName, const FString &InCopyBucketName, const FString &InCopyObjectName)
//{
//	//CREATE_CONFIGURATION_AND_STORAGE
//
//	//STRING_TO_AOS_STRING_T(source_bucket, InSourceBucketName);
//	//STRING_TO_AOS_STRING_T(source_object, InSourceObjectName);
//	//STRING_TO_AOS_STRING_T(dest_bucket, InCopyBucketName);
//	//STRING_TO_AOS_STRING_T(dest_object, InCopyObjectName);
//
//	///* 初始化参数。*/
//	//aos_string_t upload_id;
//	//aos_table_t *headers = NULL;
//	//aos_table_t *resp_headers = NULL;
//	//aos_status_t *resp_status = NULL;
//	//oss_list_upload_part_params_t *list_upload_part_params;
//	//oss_upload_part_copy_params_t *upload_part_copy_params1;
//	//oss_upload_part_copy_params_t *upload_part_copy_params2;
//	//oss_list_part_content_t *part_content;
//	//aos_list_t complete_part_list;
//	//oss_complete_part_content_t *complete_content;
//	//aos_table_t *list_part_resp_headers = NULL;
//	//aos_table_t *complete_resp_headers = NULL;
//	//int32 part1 = 1;
//	//int32 part2 = 2;
//	//int64_t range_start1 = 0;
//	//int64_t range_end1 = 6000000;//not less than 5MB
//	//int64_t range_start2 = 6000001;
//	//int64_t range_end2;
//	//int32 max_ret = 1000;
//	//headers = aos_table_make(pool, 0);
//
//	//resp_status = oss_init_multipart_upload(oss_client_options, &dest_bucket, &dest_object, &upload_id, headers, &resp_headers);
//	//if (aos_status_is_ok(resp_status)) 
//	//{
//	//	printf("init multipart upload succeeded, upload_id is %s\n", upload_id.data);
//	//}
//	//else {
//	//	printf("init multipart upload failed\n");
//	//}
//	///* 拷贝第一个分片数据。*/
//	//upload_part_copy_params1 = oss_create_upload_part_copy_params(pool);
//	//aos_str_set(&upload_part_copy_params1->source_bucket, source_bucket_name);
//	//aos_str_set(&upload_part_copy_params1->source_object, source_object_name);
//	//aos_str_set(&upload_part_copy_params1->dest_bucket, dest_bucket_name);
//	//aos_str_set(&upload_part_copy_params1->dest_object, dest_object_name);
//	//aos_str_set(&upload_part_copy_params1->upload_id, upload_id.data);
//	//upload_part_copy_params1->part_num = part1;
//	//upload_part_copy_params1->range_start = range_start1;
//	//upload_part_copy_params1->range_end = range_end1;
//	//headers = aos_table_make(pool, 0);
//	//resp_status = oss_upload_part_copy(oss_client_options, upload_part_copy_params1, headers, &resp_headers);
//	//if (aos_status_is_ok(resp_status)) {
//	//	printf("upload part 1 copy succeeded\n");
//	//}
//	//else {
//	//	printf("upload part 1 copy failed\n");
//	//}
//	///* 拷贝第二个分片数据。*/
//	//range_end2 = get_file_size(local_filename) - 1;
//	//upload_part_copy_params2 = oss_create_upload_part_copy_params(pool);
//	//aos_str_set(&upload_part_copy_params2->source_bucket, source_bucket_name);
//	//aos_str_set(&upload_part_copy_params2->source_object, source_object_name);
//	//aos_str_set(&upload_part_copy_params2->dest_bucket, dest_bucket_name);
//	//aos_str_set(&upload_part_copy_params2->dest_object, dest_object_name);
//	//aos_str_set(&upload_part_copy_params2->upload_id, upload_id.data);
//	//upload_part_copy_params2->part_num = part2;
//	//upload_part_copy_params2->range_start = range_start2;
//	//upload_part_copy_params2->range_end = range_end2;
//	//headers = aos_table_make(pool, 0);
//	//resp_status = oss_upload_part_copy(oss_client_options, upload_part_copy_params2, headers, &resp_headers);
//	//if (aos_status_is_ok(resp_status)) {
//	//	printf("upload part 2 copy succeeded\n");
//	//}
//	//else {
//	//	printf("upload part 2 copy failed\n");
//	//}
//	///* 列出分片。*/
//	//headers = aos_table_make(pool, 0);
//	//list_upload_part_params = oss_create_list_upload_part_params(pool);
//	//list_upload_part_params->max_ret = max_ret;
//	//aos_list_init(&complete_part_list);
//	//resp_status = oss_list_upload_part(oss_client_options, &dest_bucket, &dest_object, &upload_id, list_upload_part_params, &list_part_resp_headers);
//	//aos_list_for_each_entry(oss_list_part_content_t, part_content, &list_upload_part_params->part_list, node) {
//	//	complete_content = oss_create_complete_part_content(pool);
//	//	aos_str_set(&complete_content->part_number, part_content->part_number.data);
//	//	aos_str_set(&complete_content->etag, part_content->etag.data);
//	//	aos_list_add_tail(&complete_content->node, &complete_part_list);
//	//}
//	///* 完成分片拷贝。*/
//	//resp_status = oss_complete_multipart_upload(oss_client_options, &dest_bucket, &dest_object, &upload_id, &complete_part_list, headers, &complete_resp_headers);
//	//if (aos_status_is_ok(resp_status)) {
//	//	printf("complete multipart upload succeeded\n");
//	//}
//	//else {
//	//	printf("complete multipart upload failed\n");
//	//}
//	///* 释放内存池，相当于释放了请求过程中各资源分配的内存。*/
//	//aos_pool_destroy(pool);
//	///* 释放之前分配的全局资源。*/
//	//aos_http_io_deinitialize();
//	//return 0;
//
//	return false;
//}

bool FSimpleOSSManage::FOSS::DeleteObject(const FString &InBucketName, const FString &InObjectName)
{
	CREATE_CONFIGURATION_AND_STORAGE

	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	STRING_TO_AOS_STRING_T(object, InObjectName);

	/* 初始化参数。*/
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;

	/* 删除文件。*/
	resp_status = oss_delete_object(oss_client_options, &bucket, &object, &resp_headers);

	/*Determine whether the upload is successful. */
	CHECK_OSS_RESULT(
	OSS_SUCCESS_MSG("Form % delete objects %s succeeded\n",*InBucketName,*InObjectName)
	,
	OSS_ERROR_MSG("Form % delete objects %s failed\n", *InBucketName, *InObjectName))

	return bVisitSuccess;
}

bool FSimpleOSSManage::FOSS::DeleteObjectByPrefix(const FString &InBucketName, const FString &InPrefix)
{
	CREATE_CONFIGURATION_AND_STORAGE

	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	STRING_TO_AOS_STRING_T(prefix, InPrefix);

	/* 初始化参数。*/
	aos_status_t *resp_status = NULL;

	/* 删除指定前缀的文件。*/
	resp_status = oss_delete_objects_by_prefix(oss_client_options, &bucket, &prefix);

	/*Determine whether the upload is successful. */
	CHECK_OSS_RESULT(
	OSS_SUCCESS_MSG("delete objects by prefix succeeded\n",)
	,
	OSS_ERROR_MSG("delete objects by prefix failed\n"))

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

bool FSimpleOSSManage::FOSS::DeleteObjects(const FString &InBucketName, const TArray<FString> &InObjectName)
{
	CREATE_CONFIGURATION_AND_STORAGE

	STRING_TO_AOS_STRING_T(bucket, InBucketName);

	/*Build the list of files to be deleted. */
	aos_list_t object_list;
	aos_list_t deleted_object_list;
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;

	int32 is_quiet = 1;
	aos_list_init(&object_list);
	aos_list_init(&deleted_object_list);

	//It must be stored globally because in aos_str_set(), there is no memcopy () inside the pointer operation; 
	TArray<string> RemoveAosString;
	for (const auto &Tmp : InObjectName)
	{
		RemoveAosString.Add(TCHAR_TO_UTF8(*Tmp));
	}

	for (const string &Tmp: RemoveAosString)
	{
		oss_object_key_t *content_key = oss_create_oss_object_key(pool);
		aos_str_set(&content_key->key,Tmp.c_str());
		aos_list_add_tail(&content_key->node, &object_list);
	}

	/*Delete files from the file list. `Is_quiet 'indicates whether to return the result of deletion. */
	resp_status = oss_delete_objects(oss_client_options, &bucket, &object_list, is_quiet, &resp_headers, &deleted_object_list);
	
	auto MSG = [&]()
	{
		for (const auto &Tmp : InObjectName)
		{
			OSS_SUCCESS_MSG("Form % delete objects %s succeeded\n", *InBucketName, *Tmp);
		}
	};

	/*Determine whether the upload is successful. */
	CHECK_OSS_RESULT(MSG();
	, OSS_ERROR_MSG("delete objects failed\n"));
	
	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

bool FSimpleOSSManage::FOSS::ListMultipartUploads(const FString &InBucketName, TArray<FMultipartPartsUploadsInfo> &MultipartPartsInfo, int32 MaxParts /*= 1000*/)
{
	bool bVisitSuccess = true;

	CREATE_CONFIGURATION_AND_STORAGE
	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	/* 初始化参数。*/
	aos_status_t *resp_status = NULL;
	//oss_list_object_params_t *params = NULL;
	oss_list_multipart_upload_params_t *params = NULL;
	//oss_list_object_content_t *content = NULL;
	oss_list_multipart_upload_content_t *content = nullptr;
	int32 size = 0;
	ANSICHAR *line = NULL;
	ANSICHAR *prefix = "";
	ANSICHAR *nextMarker = "";
	params = oss_create_list_multipart_upload_params(pool);
	params->max_ret = 100;
	aos_str_set(&params->prefix, prefix);
	aos_str_set(&params->key_marker, nextMarker);

	OSS_DEBUG_MSG("Object\tSize\tLastModified\n");
	/* 列举所有文件。*/
	do {
		resp_status = oss_list_multipart_upload(oss_client_options, &bucket, params, NULL);
		if (!aos_status_is_ok(resp_status))
		{
			OSS_ERROR_MSG("list object failed\n");
			bVisitSuccess = false;

			break;
		}

		aos_list_for_each_entry(oss_list_multipart_upload_content_t, content, &params->upload_list, node)
		{
			MultipartPartsInfo.Add(FMultipartPartsUploadsInfo());

			MultipartPartsInfo[MultipartPartsInfo.Num() - 1].Initiated = UTF8_TO_TCHAR(content->initiated.data);
			MultipartPartsInfo[MultipartPartsInfo.Num() - 1].UploadId = UTF8_TO_TCHAR(content->upload_id.data);
			MultipartPartsInfo[MultipartPartsInfo.Num() - 1].Key = UTF8_TO_TCHAR(content->key.data);
			++size;

			line = apr_psprintf(pool, "%.*s\t%.*s\t%.*s\n", content->key.len, content->key.data,
				content->upload_id.len, content->upload_id.data,
				content->initiated.len, content->initiated.data);
			OSS_SUCCESS_MSG("%s",TCHAR_TO_UTF8(line));
		}

		nextMarker = apr_psprintf(pool, "%.*s", params->next_key_marker.len, params->next_key_marker.data);
		aos_str_set(&params->next_key_marker, nextMarker);
		aos_list_init(&params->upload_list);
	} while (params->truncated == AOS_TRUE);

	OSS_DEBUG_MSG("Total %d\n", size);
	/* 释放内存池，相当于释放了请求过程中各资源分配的内存。*/
	aos_pool_destroy(pool);
	
	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

void FSimpleOSSManage::FOSS::AsyncPutObject(
	const FString &InBucketName, 
	const FString &InLocalPaths, 
	const FString &InObjectName /*= FString()*/, 
	ProgressCallback InProgressCallback /*= nullptr*/, 
	const TMap<FString, FString> &OSSMeta /*= TMap<FString, FString>()*/)
{
	auto Lamada = [=]()
	{
		PutObject(
			InBucketName, 
			InLocalPaths, 
			InObjectName, 
			InProgressCallback,
			OSSMeta);
	};

	(new FAutoDeleteAsyncTask<FOSSAsynTask>(Lamada))->StartBackgroundTask();
}

void FSimpleOSSManage::FOSS::AsyncPutObjectByMemory(const FString &InBucketName, const FString &Data, const FString &InObjectName /*= FString()*/, ProgressCallback InProgressCallback /*= nullptr*/, const TMap<FString, FString> &OSSMeta /*= TMap<FString, FString>()*/)
{
	auto Lamada = [=]()
	{
		PutObjectByMemory(
			InBucketName,
			Data,
			InObjectName,
			InProgressCallback,
			OSSMeta);
	};

	(new FAutoDeleteAsyncTask<FOSSAsynTask>(Lamada))->StartBackgroundTask();
}

void FSimpleOSSManage::FOSS::AsyncResumableUploadObject(
	const FString &InBucketName, 
	const FString &InObjectName,
	const FString &InUploadFilePath,
	int64 PartSize,
	ProgressCallback InProgressCallback /*= nullptr*/, 
	const TMap<FString, FString> &OSSMeta /*= TMap<FString, FString>()*/)
{
	auto Lamada = [=]()
	{
		ResumableUploadObject(
			InBucketName,
			InObjectName,
			InUploadFilePath,
			PartSize,
			InProgressCallback,
			OSSMeta);
	};

	(new FAutoDeleteAsyncTask<FOSSAsynTask>(Lamada))->StartBackgroundTask();
}

bool FSimpleOSSManage::FOSS::UploadPart_Internal(
	void *in_pool,
	void *in_oss_client_options,
	const FString &InUploadId,
	const FString &InBucketName,
	const FString &InObjectName,
	const FString &InLocalPaths,
	int32 PartSize /*= 1024 * 1024 * 10*/,
	CallbackUploadPart InCallbackUploadPart)
{
	aos_pool_t *pool = (aos_pool_t*)in_pool; 
	oss_request_options_t *oss_client_options = (oss_request_options_t*)in_oss_client_options;

	STRING_TO_AOS_STRING_T(upload_id, InUploadId);
	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	STRING_TO_AOS_STRING_T(object, InObjectName);
	UTF8_STRING_TO_AOS_STRING_T(file, InLocalPaths); //Avoid Chinese characters being unable to upload 

	oss_upload_file_t *upload_file = NULL;
	aos_table_t *complete_headers = NULL;
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;
	complete_headers = aos_table_make(pool, 0);
	int32 part_num = 1;

	auto Lamada = [&InCallbackUploadPart](FString UploadId, uint32 PartCount, uint64 Size, uint64 InTotal, bool bPart, bool bComplete)
	{
		if (InCallbackUploadPart != nullptr)
		{
			//呼叫主线程
			FGraphEventRef FenceHandle = FFunctionGraphTask::CreateAndDispatchWhenReady(
				[=]()
			{
				InCallbackUploadPart(UploadId, PartCount, Size, InTotal, bPart, bComplete);
			}, TStatId(), nullptr, ENamedThreads::GameThread);
		}
	};

	STATEMENT_VISITSUCCESS

	/*Upload tiles. */
	int64_t file_length = 0;
	int64_t pos = 0;
	file_length = get_file_size(file.data);
	while (pos < file_length)
	{
		upload_file = oss_create_upload_file(pool);
		aos_str_set(&upload_file->filename, Tmp_file);
		upload_file->file_pos = pos;
		pos += PartSize;
		upload_file->file_last = pos < file_length ? pos : file_length;
		resp_status = oss_upload_part_from_file(oss_client_options, &bucket, &object, &upload_id, part_num++, upload_file, &resp_headers);
		CHECK_OSS_RESULT_NOBOOL(
			Lamada(InUploadId, part_num, file_length - pos, file_length, true, false);
		OSS_SUCCESS_MSG("upload part from file success")
			,
			OSS_ERROR_MSG("upload part from file failed")
			RETURN_FALSE)
	}

	oss_list_upload_part_params_t *params = NULL;
	aos_list_t complete_part_list;

	/*Get the uploaded fragment. */
	params = oss_create_list_upload_part_params(pool);
	params->max_ret = 1000;
	aos_list_init(&complete_part_list);
	resp_status = oss_list_upload_part(oss_client_options, &bucket, &object, &upload_id, params, &resp_headers);

	/*Determine whether the fragment list is obtained successfully. */
	CHECK_OSS_RESULT_NOBOOL(
		OSS_SUCCESS_MSG("upload upload part success")
		,
		OSS_ERROR_MSG("upload upload part failed")
		RETURN_FALSE)
		oss_complete_part_content_t *complete_part_content = NULL;
	oss_list_part_content_t *part_content = NULL;
	aos_list_for_each_entry(oss_list_part_content_t, part_content, &params->part_list, node)
	{
		complete_part_content = oss_create_complete_part_content(pool);
		aos_str_set(&complete_part_content->part_number, part_content->part_number.data);
		aos_str_set(&complete_part_content->etag, part_content->etag.data);
		aos_list_add_tail(&complete_part_content->node, &complete_part_list);
	}

	/*Complete fragment upload. */
	resp_status = oss_complete_multipart_upload(oss_client_options, &bucket, &object, &upload_id,
		&complete_part_list, complete_headers, &resp_headers);
	CHECK_OSS_RESULT_NOBOOL(
		Lamada(InUploadId, part_num, file_length - pos, file_length, false, true);
	OSS_SUCCESS_MSG("oss complete multipart upload success")
		,
		OSS_ERROR_MSG("oss complete multipart upload failed")
		Lamada(InUploadId, part_num, file_length - pos, file_length, false, false);)

		AOS_POOL_DESTROY_IT(pool);

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

bool FSimpleOSSManage::FOSS::ListObjects_Internal(const FString &InBucketName, const FString &Delimiter, const FString &Prefix, TArray<FOSSObjectInfo> &ObjectName, int32 MaxNumber /*= 100*/)
{
	CREATE_CONFIGURATION_AND_STORAGE;

	STRING_TO_AOS_STRING_T(bucket, InBucketName);

	/* 初始化参数。*/
	aos_status_t *resp_status = NULL;
	oss_list_object_params_t *params = NULL;
	oss_list_object_content_t *content = NULL;
	int32 size = 0;
	ANSICHAR *line = NULL;
	
	/* 列举包含指定前缀的文件。*/
	ANSICHAR *nextMarker = "";

	/* 设置正斜线（/）为文件夹的分隔符。*/
	params = oss_create_list_object_params(pool);
	params->max_ret = MaxNumber;
	string DelimiterString = TCHAR_TO_UTF8(*Delimiter);
	string PrefixString = TCHAR_TO_UTF8(*Prefix);
	aos_str_set(&params->delimiter, DelimiterString.c_str());
	aos_str_set(&params->prefix, PrefixString.c_str());
	aos_str_set(&params->marker, nextMarker);

	OSS_INFO_MSG("Object\tSize\tLastModified\n");

	STATEMENT_VISITSUCCESS

	/* 列举所有文件。*/
	do {
		resp_status = oss_list_object(oss_client_options, &bucket, params, NULL);
		CHECK_OSS_RESULT_NOBOOL(,
			{
				OSS_ERROR_MSG("list object failed\n");
				break;
			});

		aos_list_for_each_entry(oss_list_object_content_t, content, &params->object_list, node)
		{
			++size;

			FOSSObjectInfo Info;
			Info.Name = UTF8_TO_TCHAR(content->key.data);
			Info.Size = atol(content->size.data);
			Info.LastmodifyTime = UTF8_TO_TCHAR(content->last_modified.data);

			ObjectName.Add(Info);

			line = apr_psprintf(pool, "%.*s\t%.*s\t%.*s\n", content->key.len, content->key.data,
				content->size.len, content->size.data,
				content->last_modified.len, content->last_modified.data);

			OSS_SUCCESS_MSG("%s", line);
		}

		nextMarker = apr_psprintf(pool, "%.*s", params->next_marker.len, params->next_marker.data);
		aos_str_set(&params->marker, nextMarker);
		aos_list_init(&params->object_list);
		aos_list_init(&params->common_prefix_list);

	} while (params->truncated == AOS_TRUE);

	OSS_INFO_MSG("Total %d\n", size);
	AOS_POOL_DESTROY_IT(pool);

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

FString FSimpleOSSManage::FOSS::AsyncUploadPart(
	const FString &InBucketName, 
	const FString &InObjectName, 
	const FString &InLocalPaths, 
	int32 PartSize /*= 1024 * 1024 * 10*/,
	CallbackUploadPart InCallbackUploadPart,
	const TMap<FString, FString> &OSSMeta)
{
	FScopeLock ScopeLock(&Mutex);

	FString InUploadId;

	const FString NewLocalPaths = NormalizePaths(InLocalPaths, InObjectName);

	CREATE_CONFIGURATION_AND_STORAGE;

	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	STRING_TO_AOS_STRING_T(object, InObjectName);
	UTF8_STRING_TO_AOS_STRING_T(file, NewLocalPaths); //Avoid Chinese characters being unable to upload 

	aos_string_t upload_id;
	aos_table_t *headers = NULL;
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;
	aos_str_null(&upload_id);

	/*Set user-defined meta information. */
	CLAIM_METADATA(OSSMeta);

	/*Initialize fragment upload to obtain an upload ID. */
	resp_status = oss_init_multipart_upload(oss_client_options, &bucket, &object, &upload_id, headers, &resp_headers);

	/*Determine whether the fragment upload initialization is successful. */
	STATEMENT_VISITSUCCESS
	CHECK_OSS_RESULT_NOBOOL(
	InUploadId = ANSI_TO_TCHAR(upload_id.data);
	auto Lamada = [=]()
	{
		UploadPart_Internal(pool, oss_client_options, InUploadId, InBucketName, InObjectName, NewLocalPaths, PartSize, InCallbackUploadPart);
	};

	(new FAutoDeleteAsyncTask<FOSSAsynTask>(Lamada))->StartBackgroundTask();
	OSS_SUCCESS_MSG("init multipart upload success."),
	OSS_ERROR_MSG("init multipart upload failed."))

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return InUploadId;
}

void FSimpleOSSManage::FOSS::AsyncGetObjectToLocal(
	const FString &InBucketName,
	const FString &InObjectName, 
	const FString &InLocalPaths, 
	FRange Range /*= FRange()*/,
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
	int32 PartSize /*= 1024 * 1024 * 10,*/,
	ProgressCallback InProgressCallback /*= nullptr*/)
{
	auto Lamada = [=]()
	{
		ResumableDownloadObject(
			InBucketName,
			InObjectName,
			InDownloadFilePath,
			PartSize,
			InProgressCallback);
	};

	(new FAutoDeleteAsyncTask<FOSSAsynTask>(Lamada))->StartBackgroundTask();
}

bool FSimpleOSSManage::FOSS::PutObject(
	const FString &InBucketName, 
	const FString &InLocalPaths,
	const FString &InObjectName /*= FString()*/,
	ProgressCallback InProgressCallback /*= nullptr*/, 
	const TMap<FString, FString> &OSSMeta /*= TMap<FString, FString>()*/)
{
	FScopeLock ScopeLock(&Mutex);
	const FString NewLocalPaths = NormalizePaths(InLocalPaths, InObjectName);

	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	STRING_TO_AOS_STRING_T(object, InObjectName);
	UTF8_STRING_TO_AOS_STRING_T(file, NewLocalPaths); //Avoid Chinese characters being unable to upload 
	CREATE_CONFIGURATION_AND_STORAGE;

	aos_table_t *headers = NULL;
	aos_list_t resp_body;
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;

	/*apr_table_set(headers, "x-oss-meta-author", "oss");
	apr_table_set(headers, "Expires", "Fri, 28 Feb 2032 05:38:42 GMT");*/

	/*Set user-defined meta information. */
	CLAIM_METADATA(OSSMeta);

	if (InProgressCallback)
	{
		resp_status = oss_do_put_object_from_file(oss_client_options, &bucket, &object, &file, headers, NULL, InProgressCallback, &resp_headers, &resp_body);
	}
	else
	{
		resp_status = oss_put_object_from_file(oss_client_options, &bucket, &object, &file, headers, &resp_headers);
	}
	
	CHECK_OSS_RESULT(
		OSS_SUCCESS_MSG("Upload from local %s object %s to OSS server %s succeeded .", *InLocalPaths,*InObjectName ,*InBucketName),
		OSS_ERROR_MSG("Upload from local %s object %s to OSS server %s fail .", *InLocalPaths, *InObjectName, *InBucketName));
	
	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

bool FSimpleOSSManage::FOSS::PutObjectByMemory(
	const FString &InBucketName,
	const FString &Data, 
	const FString &InObjectName /*= FString()*/,
	ProgressCallback InProgressCallback /*= nullptr*/, 
	const TMap<FString, FString> &OSSMeta /*= TMap<FString, FString>()*/)
{
	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	STRING_TO_AOS_STRING_T(object, InObjectName);
	CREATE_CONFIGURATION_AND_STORAGE;

	string Object_Data = TCHAR_TO_UTF8(*Data);

	/* 初始化参数。*/
	aos_list_t buffer;
	aos_buf_t *content = NULL;
	aos_table_t *params = nullptr;
	aos_table_t *headers = NULL;
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;
	aos_list_t *resp_body = nullptr;
	aos_list_init(&buffer);
	content = aos_buf_pack(oss_client_options->pool, Object_Data.c_str(), strlen(Object_Data.c_str()));
	aos_list_add_tail(&content->node, &buffer);

	/* 上传文件。*/
	if (InProgressCallback)
	{
		resp_status = oss_do_put_object_from_buffer(oss_client_options, &bucket, &object, &buffer, headers, params, InProgressCallback, &resp_headers, resp_body);
	}
	else
	{
		resp_status = oss_put_object_from_buffer(oss_client_options, &bucket, &object, &buffer, headers, &resp_headers);
	}

	/*Determine whether the upload is successful. */
	CHECK_OSS_RESULT(
		OSS_SUCCESS_MSG("put object from buffer succeeded\n."),
		OSS_ERROR_MSG("put object from buffer failed\n"))

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

//bool FSimpleOSSManage::FOSS::AppendObject(const FString &InBucketName, const FString &InLocalPaths, const FString &InObjectName /*= FString()*/)
//{
//	FScopeLock ScopeLock(&Mutex);
//
//	const FString NewLocalPaths = NormalizePaths(InLocalPaths, InObjectName);
//
//	CREATE_CONFIGURATION_AND_STORAGE;
//
//	STRING_TO_AOS_STRING_T(bucket, InBucketName);
//	STRING_TO_AOS_STRING_T(object, InObjectName);
//	UTF8_STRING_TO_AOS_STRING_T(file, NewLocalPaths); //Avoid Chinese characters being unable to upload 
//
//	int32 position = 0;
//	ANSICHAR *next_append_position = NULL;
//	aos_table_t *headers1 = NULL;
//	aos_table_t *headers2 = NULL;
//	aos_table_t *resp_headers = NULL;
//	aos_status_t *resp_status = NULL;
//
//	auto CurrentPos = [&]()
//	{
//		next_append_position = (ANSICHAR*)(apr_table_get(resp_headers, "x-oss-next-append-position"));
//		if (next_append_position)
//		{
//			position = FCString::Atoi(UTF8_TO_TCHAR(next_append_position));
//		}
//	};
//
//	/*Gets the start append location. */
//	resp_status = oss_head_object(oss_client_options, &bucket, &object, headers1, &resp_headers);
//
//	STATEMENT_VISITSUCCESS;
//	CHECK_OSS_RESULT_NOBOOL(CurrentPos();
//		OSS_SUCCESS_MSG("Get next append position %i", position)
//		,
//		OSS_ERROR_MSG("Get next append position Failed. ")
//		RETURN_FALSE)
//
//	/* append file。*/
//	resp_status = oss_append_object_from_file(oss_client_options, &bucket, &object, position, &file, headers2, &resp_headers);
//	//oss_do_append_object_from_file
//
//	/*Determine whether the upload is successful. */
//	CHECK_OSS_RESULT_NOBOOL(
//		OSS_SUCCESS_MSG("Append data success."),
//		OSS_ERROR_MSG("Append data failed."))
//	AOS_POOL_DESTROY_IT(pool)
//
//	/*Get response message of OSS server */
//	OSS_SDK_RESPONSE_MSG
//
//	return bVisitSuccess;
//}

bool FSimpleOSSManage::FOSS::ResumableUploadObject(
	const FString &InBucketName, 
	const FString &InObjectName,
	const FString &InUploadFilePath,
	int64 PartSize,
	ProgressCallback InProgressCallback /*= nullptr*/, 
	const TMap<FString, FString> &OSSMeta)
{
	FScopeLock ScopeLock(&Mutex);

	const FString NewLocalPaths = NormalizePaths(InUploadFilePath, InObjectName);

	CREATE_CONFIGURATION_AND_STORAGE;

	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	STRING_TO_AOS_STRING_T(object, InObjectName);
	UTF8_STRING_TO_AOS_STRING_T(file, NewLocalPaths); //Avoid Chinese characters being unable to upload 

	aos_list_t resp_body;
	aos_table_t *headers = NULL;
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;
	oss_resumable_clt_params_t *clt_params;
	aos_list_init(&resp_body);

	/*Set user-defined meta information. */
	CLAIM_METADATA(OSSMeta);

	int32 ThreadNumber = std::thread::hardware_concurrency();

	/* 断点续传。*/
	clt_params = oss_create_resumable_clt_params_content(pool, PartSize, ThreadNumber, AOS_TRUE, NULL);
	resp_status = oss_resumable_upload_file(oss_client_options, &bucket, &object, &file, headers, NULL, clt_params, InProgressCallback, &resp_headers, &resp_body);
	
	/*Determine whether the upload is successful. */
	CHECK_OSS_RESULT(
		OSS_SUCCESS_MSG("Resumable upload %s success.",*InObjectName),
		OSS_ERROR_MSG("Resumable upload %s failed.",*InObjectName))

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

bool FSimpleOSSManage::FOSS::UploadPart(
	FString &InUploadId, 
	const FString &InBucketName,
	const FString &InObjectName, 
	const FString &InLocalPaths,
	int32 PartSize /*= 1024 * 1024 * 10*/,
	const TMap<FString, FString> &OSSMeta)
{
	FScopeLock ScopeLock(&Mutex);

	const FString NewLocalPaths = NormalizePaths(InLocalPaths, InObjectName);

	CREATE_CONFIGURATION_AND_STORAGE;

	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	STRING_TO_AOS_STRING_T(object, InObjectName);
	UTF8_STRING_TO_AOS_STRING_T(file, NewLocalPaths); //Avoid Chinese characters being unable to upload 
	
	aos_string_t upload_id;
	aos_table_t *headers = NULL;
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;
	aos_str_null(&upload_id);

	/*Set user-defined meta information. */
	CLAIM_METADATA(OSSMeta);

	/*Initialize fragment upload to obtain an upload ID. */
	resp_status = oss_init_multipart_upload(oss_client_options, &bucket, &object, &upload_id, headers, &resp_headers);

	/*Determine whether the fragment upload initialization is successful. */
	STATEMENT_VISITSUCCESS
	CHECK_OSS_RESULT_NOBOOL(
		InUploadId = ANSI_TO_TCHAR(upload_id.data);
		OSS_SUCCESS_MSG("init multipart upload success."),
		OSS_ERROR_MSG("init multipart upload failed.")
		RETURN_FALSE)

	return UploadPart_Internal(pool, oss_client_options, InUploadId, InBucketName, InObjectName, NewLocalPaths, PartSize, nullptr);
}

bool FSimpleOSSManage::FOSS::AbortMultipartUpload(const FString &InBucketName, const FString &InObjectName, const FString &InUploadId)
{
	CREATE_CONFIGURATION_AND_STORAGE;

	STRING_TO_AOS_STRING_T(bucket, InBucketName);
	STRING_TO_AOS_STRING_T(object, InObjectName);
	STRING_TO_AOS_STRING_T(upload_id, InUploadId);

	aos_table_t *headers = NULL;
	aos_table_t *resp_headers = NULL;
	aos_status_t *resp_status = NULL;

	/*Initialize fragment upload to obtain an upload ID. */
	//resp_status = oss_init_multipart_upload(oss_client_options, &bucket, &object, &upload_id, headers, &resp_headers);

	///*Determine whether the initialization of fragment upload is successful */
	//
	//CHECK_OSS_RESULT_NOBOOL(
	//	OSS_SUCCESS_MSG("oss init multipart upload success")
	//	,
	//	OSS_ERROR_MSG("oss init multipart upload  failed"))

	/*Cancel this fragment upload. */
	resp_status = oss_abort_multipart_upload(oss_client_options, &bucket, &object, &upload_id, &resp_headers);
	
	STATEMENT_VISITSUCCESS
	/*Judge whether the segment upload cancellation is successful. */
	CHECK_OSS_RESULT_NOBOOL(
		OSS_SUCCESS_MSG("oss abort multipart upload success")
		,
		OSS_ERROR_MSG("oss abort multipart upload failed"))

	AOS_POOL_DESTROY_IT(pool);

	/*Get response message of OSS server */
	OSS_SDK_RESPONSE_MSG

	return bVisitSuccess;
}

//
//FString FSimpleOSSManage::FOSS::GeneratePresignedUrl(const FString &InBucketName, const FString &InObjectName)
//{
//	ANSICHAR* bucket_name = TCHAR_TO_UTF8(*InBucketName);
//	ANSICHAR* ObjectName = TCHAR_TO_UTF8(*InObjectName);
//
//	FAuxiliaryTools  AuxiliaryTools;
//	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));
//
//	auto Outcome = Client.GeneratePresignedUrl(bucket_name, ObjectName);
//	if (!Outcome.isSuccess())
//	{
//#if WITH_EDITOR
//		SIMPLEOSS_MACRO_ERROR(Outcome)
//#endif
//
//		return "";
//	}
//
//	return Outcome.result().c_str();
//}
//
//void FSimpleOSSManage::FOSS::AsyncGetObjectByUrl(const FString& InURL, const FString& InFilePaths, ProgressCallback InProgressCallback) const
//{
//	auto Lamada = [=]()
//	{
//		GetObjectByUrl(
//			InURL,
//			InFilePaths,
//			InProgressCallback);
//	};
//
//	(new FAutoDeleteAsyncTask<FOSSAsynTask>(Lamada))->StartBackgroundTask();
//}
//
//void FSimpleOSSManage::FOSS::AsyncPutObjectByUrl(const FString& InURL, const FString& InFilePaths, ProgressCallback InProgressCallback) const
//{
//	auto Lamada = [=]()
//	{
//		PutObjectByUrl(
//			InURL,
//			InFilePaths,
//			InProgressCallback);
//	};
//
//	(new FAutoDeleteAsyncTask<FOSSAsynTask>(Lamada))->StartBackgroundTask();
//}
//
//bool FSimpleOSSManage::FOSS::GetObjectByUrl(const FString& InURL, const FString& InFilePaths, ProgressCallback InProgressCallback) const
//{
//	ANSICHAR* URL = TCHAR_TO_UTF8(*InURL);
//	ANSICHAR* FilePaths = TCHAR_TO_UTF8(*InFilePaths);
//
//	FAuxiliaryTools  AuxiliaryTools;
//	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));
//
//	GetObjectByUrlRequest Request(URL);
//	Request.setResponseStreamFactory([=]() {return std::make_shared<std::fstream>(FilePaths, std::ios_base::out | std::ios_base::in | std::ios_base::trunc); });
//
//	TransferProgress progressCallback = { InProgressCallback , nullptr };
//	Request.setTransferProgress(progressCallback);
//
//	auto Outcome = Client.GetObjectByUrl(Request);
//	if (!Outcome.isSuccess())
//	{
//#if WITH_EDITOR
//		SIMPLEOSS_MACRO_ERROR(Outcome)
//#endif
//
//		return false;
//	}
//
//	return true;
//}
//
//bool FSimpleOSSManage::FOSS::PutObjectByUrl(const FString& InURL, const FString& InFilePaths, ProgressCallback InProgressCallback) const
//{
//	ANSICHAR* URL = TCHAR_TO_UTF8(*InURL);
//	ANSICHAR* FilePaths = TCHAR_TO_UTF8(*InFilePaths);
//
//	FAuxiliaryTools  AuxiliaryTools;
//	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, AuxiliaryTools.ConvertToOSSConf(Conf));
//
//	std::shared_ptr<std::iostream> content = std::make_shared<std::fstream>(FilePaths, std::ios::in | std::ios::binary);
//
//	PutObjectByUrlRequest Request(URL, content);
//
//	TransferProgress progressCallback = { InProgressCallback , nullptr };
//	Request.setTransferProgress(progressCallback);
//
//	auto Outcome = Client.PutObjectByUrl(Request);
//	if (!Outcome.isSuccess())
//	{
//#if WITH_EDITOR
//		SIMPLEOSS_MACRO_ERROR(Outcome)
//#endif
//
//		return false;
//	}
//
//	return true;
//}

bool FSimpleOSSManage::FOSS::GetImageObjectByResize(const FVector2D &Size, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave)
{
	FString Command = FString::Printf(TEXT("image/resize,m_fixed,w_%i,h_%i"), (int32)Size.X, (int32)Size.Y);
	return GetImageObject(InBucketName, InObjectName, InFileNametoSave, Command);
}

bool FSimpleOSSManage::FOSS::GetImageObjectByCrop(const FVector4 &Size, int32 Ratio, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave)
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

bool FSimpleOSSManage::FOSS::GetImageInfo(const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave)
{
	FString ObjectExtension = FPaths::GetExtension(InObjectName,true);
	const FString NewInlocalPaths = NormalizePaths(InFileNametoSave, InObjectName.Replace(*ObjectExtension,TEXT(".txt")));

	return GetImageObject_Internal(InBucketName, InObjectName, NewInlocalPaths,TEXT("image/info"));
}

FClientConfiguration::FClientConfiguration()
	//:Scheme(EScheme::HTTP),
	//MaxConnections(16),
	:DNSTimeoutS(60),
	ConnectTimeoutS(10),
	//ProxyScheme(EScheme::HTTP),
	//ProxyPort(0),
	//bVerifySSL(false),
	bCname(false),
	//bEnableCrc64(true),
	//bEnableDateSkewAdjustment(true),
	//SendRateLimiter(0),
	//RecvRateLimiter(0),
	MinimumRateOfTolerance(1024),//1KB
	MinimumRateOfToleranceTime(15)//Indicates a timeout if the transfer rate is less than 1KB / s for 15 seconds. 
{

}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif

//#ifdef _MSC_VER
//#pragma warning(pop)
//#endif