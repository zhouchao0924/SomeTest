// Fill out your copyright notice in the Description page of Project Settings.


#include "TestOSS.h"
#include "string"
#include "OSSThirdParty/include/alibabacloud/oss/OssClient.h"
#include "Misc/FileHelper.h"
#include "fstream"

//#include <OSSThirdParty/include/alibabacloud/oss/client/ClientConfiguration.h>
using namespace AlibabaCloud::OSS;
//https://help.aliyun.com/document_detail/106557.html?spm=a2c4g.11186623.6.1021.142927f32uOqDb
#if PLATFORM_WINDOWS
#pragma optimize("", off)
#endif

TestOSS::~TestOSS()
{
}

//账号信息
std::string AccessKeyId = "LTAI4G3rcebBgb94ddzFiApX";
std::string AccessKeySecret = "ZEkp9OesH1cmJZClwepjkwVdFhqbk1";
std::string EndPoint = "oss-cn-beijing.aliyuncs.com";
std::string BucketName = "smartuiloss";

//创建bucket
//int32 OSSTest::MainOSS()
//{
//	std::string AccessKeyId = "LTAI4G3rcebBgb94ddzFiApX";
//	std::string AccessKeySecret = "ZEkp9OesH1cmJZClwepjkwVdFhqbk1";
//	std::string EndPoint = "oss-cn-beijing.aliyuncs.com";
//	std::string BucketName = "smartuiloss";
//
//	//初始化网络资源
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient Client(EndPoint, AccessKeyId, AccessKeySecret, conf);
//
//	//指定新创建bucket的名称、存储级别和ACL
//	CreateBucketRequest Request(BucketName, StorageClass::IA, CannedAccessControlList::PublicReadWrite);
//
//	auto Outcome = Client.CreateBucket(Request);
//
//	if (!Outcome.isSuccess())
//	{
//		/* 异常处理 */
//		std::cout << "CreateBucket fail" <<
//			",code:" << Outcome.error().Code() <<
//			",message:" << Outcome.error().Message() <<
//			",requestId:" << Outcome.error().RequestId() << std::endl;
//		ShutdownSdk();
//		return -1;
//	}
//
//	//释放网络等资源
//	ShutdownSdk();
//
//	return 0;
//}

//设置限速处理
//int32 OSSTest::MainOSS()
//{
//	//初始化网络资源
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	/* 设置连接池数，默认为16个。*/
//	conf.maxConnections = 20;
//
//	/* 设置请求超时时间，超时没有收到数据就关闭连接，默认为10000ms。*/
//	conf.requestTimeoutMs = 8000;
//
//	/* 设置建立连接的超时时间，默认为5000ms。*/
//	conf.connectTimeoutMs = 8000;
//
//	auto sendrateLimiter = std::make_shared<TestOSS>();
//	auto recvrateLimiter = std::make_shared<TestOSS>();
//	conf.sendRateLimiter = sendrateLimiter;
//	conf.recvRateLimiter = recvrateLimiter;
//
//	OssClient Client(EndPoint, AccessKeyId, AccessKeySecret, conf);
//
//	/* 设置下载限速（单位KB/s）。*/
//	recvrateLimiter->setRate(256);
//
//	/* 设置上传限速（单位KB/s）。*/
//	sendrateLimiter->setRate(256);
//
//	/* 上传文件。*/
//	auto outcome = Client.PutObject(BucketName, "Test.zip", "C:/Users/Smartuil/Desktop/SomeTest/OSS/Test.zip");
//
//	/* 上传过程中更新上传限速（单位KB/s）。*/
//	sendrateLimiter->setRate(300);
//
//	//释放网络等资源
//	ShutdownSdk();
//
//	return 0;
//}

////设置重试策略
//int32 OSSTest::MainOSS()
//{
//	/* 初始化网络等资源。*/
//	InitializeSdk();
//
//	ClientConfiguration conf;
//
//	/* 设置失败请求重试次数，默认为3次。*/
//	auto defaultRetryStrategy = std::make_shared<UserRetryStrategy>(5);
//	conf.retryStrategy = defaultRetryStrategy;
//
//	OssClient client(EndPoint, AccessKeyId, AccessKeySecret, conf);
//
//	/* 释放网络等资源。*/
//	ShutdownSdk();
//	return 0;
//}
//
//bool UserRetryStrategy::shouldRetry(const Error & error, long attemptedRetries) const
//{
//	if (attemptedRetries >= m_maxRetries)
//		return false;
//
//	long responseCode = error.Status();
//
//	//http code
//	if ((responseCode == 403 && error.Message().find("RequestTimeTooSkewed") != std::string::npos) ||
//		(responseCode > 499 && responseCode < 599)) {
//		return true;
//	}
//	else {
//		switch (responseCode)
//		{
//		case CURLE_COULDNT_CONNECT:
//		case CURLE_PARTIAL_FILE:
//		case CURLE_WRITE_ERROR:
//		case CURLE_OPERATION_TIMEDOUT:
//		case CURLE_GOT_NOTHING:
//		case CURLE_SEND_ERROR:
//		case CURLE_RECV_ERROR:
//			return true;
//		default:
//			break;
//		};
//	}
//
//	return false;
//}
//
//long UserRetryStrategy::calcDelayTimeMs(const Error & error, long attemptedRetries) const
//{
//	UNUSED_PARAM(error);
//	return (1 << attemptedRetries) * m_scaleFactor;
//}

void print(const std::string &NewStr)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, NewStr.c_str());
	}
}

void print(const FString &NewStr)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, NewStr);
	}
}

//存储空间
//int32 OSSTest::MainOSS()
//{
//	//初始化网络资源
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient Client(EndPoint, AccessKeyId, AccessKeySecret, conf);
//
//	/* 列举存储空间 */
//	ListBucketsRequest request;
//
//	auto outcome = Client.ListBuckets(request);
//
//	if (outcome.isSuccess()) {
//		/* 打印存储空间信息 */
//		std::cout << " success, and bucket count is" << outcome.result().Buckets().size() << std::endl;
//		std::cout << "Bucket name is" << std::endl;
//		for (auto result : outcome.result().Buckets())
//		{
//			std::cout << result.Name() << std::endl;
//			print(result.Name());
//		}
//	}
//	else {
//		/* 异常处理 */
//		std::cout << "ListBuckets fail" <<
//			",code:" << outcome.error().Code() <<
//			",message:" << outcome.error().Message() <<
//			",requestId:" << outcome.error().RequestId() << std::endl;
//		ShutdownSdk();
//		return -1;
//	}
//
//	/* 设置存储空间访问权限 */
//	SetBucketAclRequest request2(BucketName, CannedAccessControlList::PublicRead);
//	auto outcome2 = Client.SetBucketAcl(request2);
//
//	/* 获取存储空间地域 */
//	GetBucketLocationRequest request3(BucketName);
//	auto outcome3 = Client.GetBucketLocation(request3);
//
//	if (outcome3.isSuccess()) {
//		print(outcome3.result().Location());
//	}
//
//	/* 判断存储空间是否存在 */
//	auto outcome4 = Client.DoesBucketExist(BucketName);
//
//	if (outcome4) {
//		std::cout << " The Bucket exists" << std::endl;
//		print(" The Bucket exists");
//	}
//	else {
//		std::cout << "The Bucket does not exist" << std::endl;
//		print("The Bucket does not exist");
//	}
//
//	/* 指定要删除的bucket名称 */
//	DeleteBucketRequest request5("smartuilnewoss");
//
//	/* 删除bucket */
//	auto outcome5 = Client.DeleteBucket(request5);
//
//
//	/* 设置请求者付费模式 */
//	SetBucketRequestPaymentRequest request6(BucketName);
//	request6.setRequestPayer(RequestPayer::Requester);
//
//	auto outcome6 = Client.SetBucketRequestPayment(request6);
//
//	/* 获取请求者付费模式 */
//	GetBucketRequestPaymentRequest request7(BucketName);
//	auto outcome7 = Client.GetBucketRequestPayment(request7);
//
//	auto cc = outcome7.result().Payer();
//
//	//释放网络等资源
//	ShutdownSdk();
//
//	return 0;
//}

int64_t getFileSize(const std::string& file)
{
	std::fstream f(file, std::ios::in | std::ios::binary);
	f.seekg(0, f.end);
	int64_t size = f.tellg();
	f.close();
	return size;
}

//int32 OSSTest::MainOSS()
//{
//	TArray<uint8> StringArray;
//	FFileHelper::LoadFileToArray(StringArray, *FString("C:/Users/Smartuil/Desktop/SomeTest/OSS/Test.txt"));
//
//	std::string ObjectName = "Test.txt";
//	InitializeSdk();
//	ClientConfiguration conf;
//	OssClient client(EndPoint, AccessKeyId, AccessKeySecret, conf);
//	std::shared_ptr<std::iostream> content = std::make_shared<std::stringstream>();
//	*content << "Thank you for using Alibaba Cloud Object Storage Service!";
//	PutObjectRequest request(BucketName, ObjectName, content);
//
//	/*（可选）请参见如下示例设置存储类型及访问权限ACL* /
//	//request.MetaData().addHeader("x-oss-object-acl", "private");
//	//request.MetaData().addHeader("x-oss-storage-class", "Standard");
//
//	/* 上传文件 */
//	auto outcome = client.PutObject(request);
//
//	if (!outcome.isSuccess()) {
//		/* 异常处理 */
//		std::cout << "PutObject fail" <<
//			",code:" << outcome.error().Code() <<
//			",message:" << outcome.error().Message() <<
//			",requestId:" << outcome.error().RequestId() << std::endl;
//		ShutdownSdk();
//		return -1;
//	}
//
//	std::string ObjectName2 = "GTAVTool.zip";
//	std::string UploadFilePath = "C:/Users/Smartuil/Desktop/SomeTest/OSS/GTAVTool.zip";
//	std::string CheckpointFilePath = "C:/Users/Smartuil/Desktop/SomeTest/OSS/";
//
//	/* 断点续传上传 */
//	//UploadObjectRequest request2(BucketName, ObjectName2, UploadFilePath, CheckpointFilePath);
//	//auto outcome2 = client.ResumableUploadObject(request2);
//
//	{
//		//std::vector<PutObjectOutcomeCallable> Callables;
//		//for (int i = 0; i < 4; i++) {
//		//	std::shared_ptr<std::iostream> content2 = std::make_shared<std::fstream>(
//		//		"C:/Users/Smartuil/Desktop/SomeTest/OSS/GTAVTool2.zip", 
//		//		std::ios::in | std::ios::binary);
//
//		//	PutObjectRequest request3(BucketName, ObjectName2, content2);
//		//	auto outcomeCallable = client.PutObjectCallable(request3);
//		//	Callables.emplace_back(std::move(outcomeCallable));
//		//}
//		//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//
//		///* 中断上传 */
//		//client.DisableRequest();
//
//		//for (size_t i = 0; i < Callables.size(); i++) {
//		//	auto outcome3 = Callables[i].get();
//		//	if (outcome3.error().Code() == "ClientError:100002" ||
//		//		outcome3.error().Code() == "ClientError:200042") {
//		//		std::cout << "disable putobject success" << std::endl;
//		//		print("disable putobject success");
//		//	}
//		//}
//	}
//
//	{
//		InitiateMultipartUploadRequest initUploadRequest(BucketName, ObjectName2);
//		/*（可选）请参见如下示例设置存储类型 */
//		//initUploadRequest.MetaData().addHeader("x-oss-storage-class", "Standard");
//
//		/* 初始化分片上传事件 */
//		auto uploadIdResult = client.InitiateMultipartUpload(initUploadRequest);
//		auto uploadId = uploadIdResult.result().UploadId();
//		std::string fileToUpload = "C:/Users/Smartuil/Desktop/SomeTest/OSS/GTAVTool2.zip";
//		int64_t partSize = 1024 * 1024;
//		PartList partETagList;
//		auto fileSize = getFileSize(fileToUpload);
//		int32 partCount = static_cast<int32>(fileSize / partSize);
//		/* 计算分片个数 */
//		if (fileSize % partSize != 0) {
//			partCount++;
//		}
//
//		/* 对每一个分片进行上传 */
//		for (int32 i = 1; i <= partCount; i++) {
//			auto skipBytes = partSize * (i - 1);
//			auto size = (partSize < fileSize - skipBytes) ? partSize : (fileSize - skipBytes);
//			std::shared_ptr<std::iostream> content4 = std::make_shared<std::fstream>(fileToUpload, std::ios::in | std::ios::binary);
//			content4->seekg(skipBytes, std::ios::beg);
//
//			UploadPartRequest uploadPartRequest(BucketName, ObjectName2, content4);
//			uploadPartRequest.setContentLength(size);
//			uploadPartRequest.setUploadId(uploadId);
//			uploadPartRequest.setPartNumber(i);
//			auto uploadPartOutcome = client.UploadPart(uploadPartRequest);
//			if (uploadPartOutcome.isSuccess()) {
//				Part part(i, uploadPartOutcome.result().ETag());
//				partETagList.push_back(part);
//			}
//			else {
//				std::cout << "uploadPart fail" <<
//					",code:" << uploadPartOutcome.error().Code() <<
//					",message:" << uploadPartOutcome.error().Message() <<
//					",requestId:" << uploadPartOutcome.error().RequestId() << std::endl;
//			}
//
//		}
//
//		{
//			/* 列举已上传分片，默认列举1000个分片 */
//			ListPartsRequest listuploadrequest(BucketName, ObjectName2);
//			listuploadrequest.setMaxParts(2);
//			listuploadrequest.setUploadId(uploadId);
//			ListPartsOutcome listuploadresult;
//			do {
//				listuploadresult = client.ListParts(listuploadrequest);
//				if (!listuploadresult.isSuccess()) {
//					/* 异常处理 */
//					std::cout << "ListParts fail" <<
//						",code:" << listuploadresult.error().Code() <<
//						",message:" << listuploadresult.error().Message() <<
//						",requestId:" << listuploadresult.error().RequestId() << std::endl;
//					break;
//				}
//				else {
//					for (const auto& part : listuploadresult.result().PartList()) {
//						std::cout << "part" <<
//							",name:" << part.PartNumber() <<
//							",size:" << part.Size() <<
//							",etag:" << part.ETag() <<
//							",lastmodify time:" << part.LastModified() << std::endl;
//						//print(std::to_string(part.PartNumber()));
//						//print(std::to_string(part.Size()));
//						//print(part.ETag());
//						//print(part.LastModified());
//
//					}
//				}
//				listuploadrequest.setPartNumberMarker(listuploadresult.result().NextPartNumberMarker());
//			} while (listuploadresult.result().IsTruncated());
//		}
//
//		{
//			/* 列举已上传事件，默认列举1000个分片 */
//			ListMultipartUploadsRequest listmultiuploadrequest(BucketName);
//			ListMultipartUploadsOutcome listresult;
//			do {
//				listresult = client.ListMultipartUploads(listmultiuploadrequest);
//				if (!listresult.isSuccess()) {
//					/* 异常处理 */
//					std::cout << "ListMultipartUploads fail" <<
//						",code:" << listresult.error().Code() <<
//						",message:" << listresult.error().Message() <<
//						",requestId:" << listresult.error().RequestId() << std::endl;
//					break;
//				}
//				else {
//					for (const auto& part : listresult.result().MultipartUploadList()) {
//						std::cout << "part" <<
//							",name:" << part.Key <<
//							",uploadid:" << part.UploadId <<
//							",initiated time:" << part.Initiated << std::endl;
//						print(part.Key);
//						print(part.UploadId);
//						print(part.Initiated);
//					}
//				}
//				listmultiuploadrequest.setKeyMarker(listresult.result().NextKeyMarker());
//				listmultiuploadrequest.setUploadIdMarker(listresult.result().NextUploadIdMarker());
//			} while (listresult.result().IsTruncated());
//		}
//
//		{
//			/* 初始化分片上传事件 */
//			auto uploadIdResult2 = client.InitiateMultipartUpload(initUploadRequest);
//			auto uploadId2 = uploadIdResult2.result().UploadId();
//
//			/* 取消分片上传 */
//			AbortMultipartUploadRequest  abortUploadRequest(BucketName, ObjectName2, uploadId2);
//			auto abortUploadIdResult = client.AbortMultipartUpload(abortUploadRequest);
//		}
//
//		/* 完成分片上传 */
//		CompleteMultipartUploadRequest request4(BucketName, ObjectName2);
//		request4.setUploadId(uploadId);
//		request4.setPartList(partETagList);
//		/*（可选）请参见如下示例设置读写权限ACL */
//		//request.setAcl(CannedAccessControlList::Private);
//
//		auto outcome4 = client.CompleteMultipartUpload(request4);
//
//		if (!outcome4.isSuccess()) {
//			/* 异常处理 */
//			std::cout << "CompleteMultipartUpload fail" <<
//				",code:" << outcome4.error().Code() <<
//				",message:" << outcome4.error().Message() <<
//				",requestId:" << outcome4.error().RequestId() << std::endl;
//			ShutdownSdk();
//			return -1;
//		}
//	}
//	/* 释放网络等资源 */
//	ShutdownSdk();
//
//	return 0;
//}

void ProgressCallback(size_t increment, int64_t transfered, int64_t total, void* userData)
{
	std::cout << "ProgressCallback[" << userData << "] => " <<
		increment << " ," << transfered << "," << total << std::endl;
	print(FString::Printf(TEXT("increment = %i,transfered = %i,total = %i"), increment, transfered, total));
}

#include "Core/ThreadCoreMacro.h"

//int32 OSSTest::MainOSS()
//{
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(EndPoint, AccessKeyId, AccessKeySecret, conf);
//
//	std::string ObjectName = "GTAVTool.zip";
//	std::shared_ptr<std::iostream> content = std::make_shared<std::fstream>(
//		"C:/Users/Smartuil/Desktop/SomeTest/OSS/GTAVTool2.zip", 
//		std::ios::in | std::ios::binary);
//	PutObjectRequest request(BucketName, ObjectName, content);
//
//	TransferProgress progressCallback = { ProgressCallback , nullptr };
//	request.setTransferProgress(progressCallback);
//
//	auto OutLamada = [=]()
//	{
//		/* 上传文件 */
//		auto outcome = client.PutObject(request);
//		if (!outcome.isSuccess()) {
//			/* 异常处理 */
//
//			ShutdownSdk();
//		}
//	};
//
//	ASYNCTASK_Lambda(OutLamada);
//
//	return 0;
//}

//int32 OSSTest::MainOSS()
//{
//	std::string ObjectName = "GTAVTool.zip";
//	std::string FileNametoSave = "C:/Users/Smartuil/Desktop/SomeTest/OSS/1.zip";
//	std::string DownloadFilePath = "C:/Users/Smartuil/Desktop/SomeTest/OSS/2.zip";
//	std::string CheckpointFilePath = "C:/Users/Smartuil/Desktop/SomeTest/OSS";
//
//	/*初始化网络等资源*/
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(EndPoint, AccessKeyId, AccessKeySecret, conf);
//
//	/* 异步获取文件 */
//	GetObjectRequest request5(BucketName, ObjectName);
//	TransferProgress progressCallback = { ProgressCallback , nullptr };
//	request5.setTransferProgress(progressCallback);
//	request5.setResponseStreamFactory([=]() 
//	{
//		return std::make_shared<std::fstream>(
//			FileNametoSave, 
//			std::ios_base::out | std::ios_base::in | std::ios_base::trunc | std::ios_base::binary); 
//	});
//	auto OutLamada = [=]()
//	{
//		auto outcome = client.GetObject(request5);
//	};
//	ASYNCTASK_Lambda(OutLamada);
//
//
//	/*获取文件到本地文件*/
//	GetObjectRequest request(BucketName, ObjectName);
//	request.setResponseStreamFactory([=]() {return std::make_shared<std::fstream>(FileNametoSave, std::ios_base::out | std::ios_base::in | std::ios_base::trunc | std::ios_base::binary); });
//
//	auto outcome = client.GetObject(request);
//
//	if (outcome.isSuccess()) {
//		std::cout << "GetObjectToFile success" << outcome.result().Metadata().ContentLength() << std::endl;
//	}
//	else {
//		/*异常处理*/
//		std::cout << "GetObjectToFile fail" <<
//			",code:" << outcome.error().Code() <<
//			",message:" << outcome.error().Message() <<
//			",requestId:" << outcome.error().RequestId() << std::endl;
//		ShutdownSdk();
//		return -1;
//	}
//
//	/*获取文件到本地内存*/
//	GetObjectRequest request2(BucketName, ObjectName);
//	auto outcome2 = client.GetObject(request2);
//
//	/* 获取文件 */
//	GetObjectRequest request3(BucketName, ObjectName);
//	/* 设置下载范围 */
//	request3.setRange(0, 1);
//	auto outcome3 = client.GetObject(request3);
//
//	/* 断点续传下载 */
//	DownloadObjectRequest request4(BucketName, ObjectName, DownloadFilePath, CheckpointFilePath);
//	auto outcome4 = client.ResumableDownloadObject(request4);
//
//	/*释放网络等资源*/
//	ShutdownSdk();
//
//	return 0;
//}

//int32 OSSTest::MainOSS()
//{
//	std::string ObjectName = "GTAVTool.zip";
//	std::string FileNametoSave = "C:/Users/Smartuil/Desktop/SomeTest/OSS/1.zip";
//	std::string DownloadFilePath = "C:/Users/Smartuil/Desktop/SomeTest/OSS/2.zip";
//	std::string CheckpointFilePath = "C:/Users/Smartuil/Desktop/SomeTest/OSS";
//
//	/*初始化网络等资源*/
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(EndPoint, AccessKeyId, AccessKeySecret, conf);
//
//
//	SetObjectAclRequest request(BucketName, ObjectName);
//	request.setAcl(CannedAccessControlList::Private);
//	auto outcome = client.SetObjectAcl(request);
//
//	if (!outcome.isSuccess()) {
//		/* 异常处理 */
//		std::cout << "SetObjectAcl fail" <<
//			",code:" << outcome.error().Code() <<
//			",message:" << outcome.error().Message() <<
//			",requestId:" << outcome.error().RequestId() << std::endl;
//		ShutdownSdk();
//		return -1;
//	}
//
//	/* 获取文件访问权限 */
//	GetObjectAclRequest request2(BucketName, ObjectName);
//	auto outcome2 = client.GetObjectAcl(request2);
//	if (!outcome2.isSuccess()) {
//		/* 异常处理 */
//		std::cout << "GetObjectAcl fail" <<
//			",code:" << outcome2.error().Code() <<
//			",message:" << outcome2.error().Message() <<
//			",requestId:" << outcome2.error().RequestId() << std::endl;
//
//		ShutdownSdk();
//		return -1;
//	}
//	else {
//		std::cout << " GetObjectAcl success, Acl:" << outcome2.result().Acl() << std::endl;
//		print(std::to_string(outcome2.result().Acl()));
//	}
//
//	/* 释放网络等资源 */
//	ShutdownSdk();
//
//
//	return 0;
//}


//int32 OSSTest::MainOSS()
//{
//	std::string ObjectName = "GTAVTool.zip";
//	std::string FileNametoSave = "C:/Users/Smartuil/Desktop/SomeTest/OSS/1.zip";
//	std::string DownloadFilePath = "C:/Users/Smartuil/Desktop/SomeTest/OSS/2.zip";
//	std::string CheckpointFilePath = "C:/Users/Smartuil/Desktop/SomeTest/OSS";
//
//	/*初始化网络等资源*/
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(EndPoint, AccessKeyId, AccessKeySecret, conf);
//	/* 设置HTTP header */
//	auto meta = ObjectMetaData();
//	meta.setContentType("text/plain");
//	meta.setCacheControl("max-ag e=3");
//
//	//meta.setContentDisposition();
//	//meta.setContentEncoding();
//	//meta.setContentLength();
//	//meta.setExpirationTime();
//
//	/* 设置自定义文件元信息 */
//	meta.UserMetaData()["Hello"] = "No-value";
//
//	std::shared_ptr<std::iostream> content = std::make_shared<std::stringstream>();
//	*content << "Thank you for using Aliyun Object Storage Service!";
//	auto outcome = client.PutObject(BucketName, ObjectName, content, meta);
//
//	if (!outcome.isSuccess()) {
//		/* 异常处理 */
//		std::cout << "PutObject fail" <<
//			",code:" << outcome.error().Code() <<
//			",message:" << outcome.error().Message() <<
//			",requestId:" << outcome.error().RequestId() << std::endl;
//		ShutdownSdk();
//		return -1;
//	}
//
//	/* 获取文件的部分元信息 */
//	auto outcome2 = client.GetObjectMeta(BucketName, ObjectName);
//
//	if (!outcome2.isSuccess()) {
//		/* 异常处理 */
//		std::cout << "GetObjectMeta fail" <<
//			",code:" << outcome2.error().Code() <<
//			",message:" << outcome2.error().Message() <<
//			",requestId:" << outcome2.error().RequestId() << std::endl;
//		ShutdownSdk();
//		return -1;
//	}
//	else {
//		auto metadata = outcome2.result();
//		std::cout << " get metadata success, ETag:" << metadata.ETag() << "; LastModified:"
//			<< metadata.LastModified() << "; Size:" << metadata.ContentLength() << std::endl;
//		print(metadata.ETag());
//		print(metadata.LastModified());
//		print(std::to_string(metadata.ContentLength()));
//	}
//
//	/* 获取文件的全部元信息 */
//	auto outcome3 = client.HeadObject(BucketName, ObjectName);
//
//	if (!outcome3.isSuccess()) {
//		/* 异常处理 */
//		std::cout << "HeadObject fail" <<
//			",code:" << outcome3.error().Code() <<
//			",message:" << outcome3.error().Message() <<
//			",requestId:" << outcome3.error().RequestId() << std::endl;
//		ShutdownSdk();
//		return -1;
//	}
//	else {
//		auto headMeta = outcome3.result();
//		std::cout << "headMeta success, ContentType:"
//			<< headMeta.ContentType() << "; ContentLength:" << headMeta.ContentLength()
//			<< "; CacheControl:" << headMeta.CacheControl() << std::endl;
//		print(headMeta.ContentType());	
//		print(std::to_string(headMeta.ContentLength()));
//		print(headMeta.CacheControl());
//		print(headMeta.ContentDisposition());
//		print(headMeta.ContentEncoding());
//		print(headMeta.ContentMd5());
//	}
//
//	/* 释放网络等资源 */
//	ShutdownSdk();
//	return 0;
//}

static int64_t calculateFolderLength(const OssClient &client, const std::string &bucketName, const std::string &folder)
{
	std::string nextMarker = "";
	int64_t size = 0;
	ListObjectOutcome outcome2;
	do {
		/*列举文件*/
		ListObjectsRequest request(bucketName);
		request.setPrefix(folder);
		request.setMarker(nextMarker);
		outcome2 = client.ListObjects(request);

		if (!outcome2.isSuccess()) {
			/*异常处理*/
			std::cout << "ListObjects fail" <<
				",code:" << outcome2.error().Code() <<
				",message:" << outcome2.error().Message() <<
				",requestId:" << outcome2.error().RequestId() << std::endl;
			break;
		}

		for (const auto& object : outcome2.result().ObjectSummarys()) {
			size += _atoi64(object.Key().c_str());
		}

		nextMarker = outcome2.result().NextMarker();
	} while (outcome2.result().IsTruncated());

	return size;
}

//int32 OSSTest::MainOSS()
//{
//	std::string ObjectName = "GTAVTool.zip";
//	std::string FileNametoSave = "C:/Users/Smartuil/Desktop/SomeTest/OSS/1.zip";
//	std::string DownloadFilePath = "C:/Users/Smartuil/Desktop/SomeTest/OSS/2.zip";
//	std::string CheckpointFilePath = "C:/Users/Smartuil/Desktop/SomeTest/OSS";
//
//	/*初始化网络等资源*/
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(EndPoint, AccessKeyId, AccessKeySecret, conf);
//
//	/* 列举文件 */
//	ListObjectsRequest request(BucketName);
//	request.setMaxKeys(200);
//	auto outcome = client.ListObjects(request);
//
//	if (!outcome.isSuccess()) {
//		/* 异常处理 */
//		std::cout << "ListObjects fail" <<
//			",code:" << outcome.error().Code() <<
//			",message:" << outcome.error().Message() <<
//			",requestId:" << outcome.error().RequestId() << std::endl;
//		ShutdownSdk();
//		return -1;
//	}
//	else {
//		for (const auto& object : outcome.result().ObjectSummarys()) {
//			std::cout << "object" <<
//				",name:" << object.Key() <<
//				",size:" << object.Size() <<
//				",lastmodify time:" << object.LastModified() << std::endl;
//			print(object.Key());
//			print(std::to_string(object.Size()));
//			print(object.LastModified());
//		}
//	}
//	std::string keyPrefix = "yourkeyPrefix";
//	{
//		std::string nextMarker = "";
//		ListObjectOutcome outcome4;
//		do {
//			/* 列举文件 */
//			ListObjectsRequest request2(BucketName);
//			/* 设置正斜线（/）为文件夹的分隔符 */
//			request2.setDelimiter("/");
//			request2.setPrefix(keyPrefix);
//			request2.setMarker(nextMarker);
//			outcome4 = client.ListObjects(request2);
//
//			if (!outcome4.isSuccess()) {
//				/* 异常处理 */
//				std::cout << "ListObjects fail" <<
//					",code:" << outcome4.error().Code() <<
//					",message:" << outcome4.error().Message() <<
//					",requestId:" << outcome4.error().RequestId() << std::endl;
//				break;
//			}
//			for (const auto& object : outcome4.result().ObjectSummarys()) {
//				std::cout << "object" <<
//					",name:" << object.Key() <<
//					",size:" << object.Size() <<
//					",lastmodify time:" << object.LastModified() << std::endl;
//			}
//			for (const auto& commonPrefix : outcome4.result().CommonPrefixes()) {
//				std::cout << "commonPrefix" << ",name:" << commonPrefix << std::endl;
//			}
//			nextMarker = outcome4.result().NextMarker();
//		} while (outcome4.result().IsTruncated());
//	}
//	
//	std::string nextMarker = "";
//	ListObjectOutcome outcome3;
//	do {
//		/*列举文件*/
//		ListObjectsRequest request3(BucketName);
//		/*设置正斜线（/）为文件夹的分隔符*/
//		request3.setDelimiter("/");
//		request3.setPrefix(keyPrefix);
//		request3.setMarker(nextMarker);
//		outcome3 = client.ListObjects(request3);
//
//		if (!outcome3.isSuccess()) {
//			/*异常处理*/
//			std::cout << "ListObjects fail" <<
//				",code:" << outcome3.error().Code() <<
//				",message:" << outcome3.error().Message() <<
//				",requestId:" << outcome3.error().RequestId() << std::endl;
//			break;
//		}
//
//		for (const auto& object : outcome3.result().ObjectSummarys()) {
//			std::cout << "object" <<
//				",name:" << object.Key() <<
//				",size:" << object.Size() << std::endl;
//		}
//
//		for (const auto& commonPrefix : outcome3.result().CommonPrefixes()) {
//			int64_t foldersize = calculateFolderLength(client, BucketName, commonPrefix);
//			std::cout << "folder" <<
//				",name:" << commonPrefix <<
//				",size:" << foldersize << std::endl;
//		}
//		nextMarker = outcome3.result().NextMarker();
//	} while (outcome3.result().IsTruncated());
//
//	std::string YourMarker = "Test/";
//	ListObjectOutcome outcome5;
//	do {
//		/* 列举文件 */
//		ListObjectsRequest request5(BucketName);
//		/* 列举指定marker之后的文件 */
//		request.setMarker(YourMarker);
//		outcome5 = client.ListObjects(request5);
//
//		if (!outcome5.isSuccess()) {
//			/* 异常处理 */
//			std::cout << "ListObjects fail" <<
//				",code:" << outcome5.error().Code() <<
//				",message:" << outcome5.error().Message() <<
//				",requestId:" << outcome5.error().RequestId() << std::endl;
//			break;
//		}
//		YourMarker = outcome.result().NextMarker();
//		for (const auto& object : outcome.result().ObjectSummarys()) {
//			std::cout << "object" <<
//				",name:" << object.Key() <<
//				",size:" << object.Size() <<
//				",lastmodify time:" << object.LastModified() << std::endl;
//		}
//	} while (outcome5.result().IsTruncated());
//
//	/* 释放网络等资源 */
//	ShutdownSdk();
//	return 0;
//}

int32 OSSTest::MainOSS()
{
	std::string ObjectName = "GTAVTool.zip";
	/* 初始化网络等资源 */
	InitializeSdk();

	ClientConfiguration conf;
	OssClient client(EndPoint, AccessKeyId, AccessKeySecret, conf);

	/* ObjectName表示删除OSS文件时需要指定包含文件后缀在内的完整路径，例如abc/efg/123.jpg */
	/* 如需删除文件夹，请将ObjectName设置为对应的文件夹名称。如果文件夹非空，则需要将文件夹下的所有object删除后才能删除该文件夹 */
	DeleteObjectRequest request(BucketName, ObjectName);

	/* 删除文件 */
	auto outcome = client.DeleteObject(request);

	if (!outcome.isSuccess()) {
		/* 异常处理 */
		std::cout << "DeleteObject fail" <<
			",code:" << outcome.error().Code() <<
			",message:" << outcome.error().Message() <<
			",requestId:" << outcome.error().RequestId() << std::endl;
		ShutdownSdk();
		return -1;
	}

	/* 释放网络等资源 */
	ShutdownSdk();

	return 0;
}

#if PLATFORM_WINDOWS
#pragma optimize("", on)
#endif