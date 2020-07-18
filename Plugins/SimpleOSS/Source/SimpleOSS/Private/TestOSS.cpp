// Fill out your copyright notice in the Description page of Project Settings.


#include "TestOSS.h"
#include "string"
#include "OSSThirdParty/include/alibabacloud/oss/OssClient.h"

using namespace AlibabaCloud::OSS;

TestOSS::TestOSS()
{
}

TestOSS::~TestOSS()
{
}

int32 OSSTest::MainOSS()
{
	std::string AccessKeyId = "LTAI4G3rcebBgb94ddzFiApX";
	std::string AccessKeySecret = "ZEkp9OesH1cmJZClwepjkwVdFhqbk1";
	std::string EndPoint = "oss-cn-beijing.aliyuncs.com";
	std::string BucketName = "smartuilnewoss";

	//初始化网络资源
	InitializeSdk();

	ClientConfiguration conf;
	OssClient Client(EndPoint, AccessKeyId, AccessKeySecret, conf);

	//指定新创建bucket的名称、存储级别和ACL
	CreateBucketRequest Request(BucketName, StorageClass::IA, CannedAccessControlList::PublicReadWrite);

	auto Outcome = Client.CreateBucket(Request);

	if (!Outcome.isSuccess())
	{
		/* 异常处理 */
		std::cout << "CreateBucket fail" <<
			",code:" << Outcome.error().Code() <<
			",message:" << Outcome.error().Message() <<
			",requestId:" << Outcome.error().RequestId() << std::endl;
		ShutdownSdk();
		return -1;
	}

	//释放网络等资源
	ShutdownSdk();

	return 0;
}
