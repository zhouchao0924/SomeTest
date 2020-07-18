// Fill out your copyright notice in the Description page of Project Settings.


#include "TestOSS.h"
#include "string"
#include "OSSThirdParty/include/alibabacloud/oss/OssClient.h"
//#include <OSSThirdParty/include/alibabacloud/oss/client/ClientConfiguration.h>
using namespace AlibabaCloud::OSS;
//https://help.aliyun.com/document_detail/106557.html?spm=a2c4g.11186623.6.1021.142927f32uOqDb

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

//存储空间
int32 OSSTest::MainOSS()
{
	//初始化网络资源
	InitializeSdk();

	ClientConfiguration conf;
	OssClient Client(EndPoint, AccessKeyId, AccessKeySecret, conf);

	/* 列举存储空间 */
	ListBucketsRequest request;

	auto outcome = Client.ListBuckets(request);

	if (outcome.isSuccess()) {
		/* 打印存储空间信息 */
		std::cout << " success, and bucket count is" << outcome.result().Buckets().size() << std::endl;
		std::cout << "Bucket name is" << std::endl;
		for (auto result : outcome.result().Buckets())
		{
			std::cout << result.Name() << std::endl;
			print(result.Name());
		}
	}
	else {
		/* 异常处理 */
		std::cout << "ListBuckets fail" <<
			",code:" << outcome.error().Code() <<
			",message:" << outcome.error().Message() <<
			",requestId:" << outcome.error().RequestId() << std::endl;
		ShutdownSdk();
		return -1;
	}

	/* 设置存储空间访问权限 */
	SetBucketAclRequest request2(BucketName, CannedAccessControlList::PublicRead);
	auto outcome2 = Client.SetBucketAcl(request2);

	/* 获取存储空间地域 */
	GetBucketLocationRequest request3(BucketName);
	auto outcome3 = Client.GetBucketLocation(request3);

	if (outcome3.isSuccess()) {
		print(outcome3.result().Location());
	}

	/* 判断存储空间是否存在 */
	auto outcome4 = Client.DoesBucketExist(BucketName);

	if (outcome4) {
		std::cout << " The Bucket exists" << std::endl;
		print(" The Bucket exists");
	}
	else {
		std::cout << "The Bucket does not exist" << std::endl;
		print("The Bucket does not exist");
	}

	/* 指定要删除的bucket名称 */
	DeleteBucketRequest request5("smartuilnewoss");

	/* 删除bucket */
	auto outcome5 = Client.DeleteBucket(request5);


	/* 设置请求者付费模式 */
	SetBucketRequestPaymentRequest request6(BucketName);
	request6.setRequestPayer(RequestPayer::Requester);

	auto outcome6 = Client.SetBucketRequestPayment(request6);

	/* 获取请求者付费模式 */
	GetBucketRequestPaymentRequest request7(BucketName);
	auto outcome7 = Client.GetBucketRequestPayment(request7);

	auto cc = outcome7.result().Payer();

	//释放网络等资源
	ShutdownSdk();

	return 0;
}