#include "Test.h"
//#include "Abandonable/SimpleAbandonable.h"

int32 OSSTest::MainOSS()
{
	return 0;
}
#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif
//void Print_F(const FString &NewStr)
//{
//	if (GEngine)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, NewStr);
//		
//	}
//}

//int32 OSSTest::MainOSS()
//{
//	FString URL = "https://helloworldd.oss-cn-beijing.aliyuncs.com/Splash.jpg";
//
//	FHTTPClient Client;
//	auto A = [=](FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue)
//	{
//		Print_F(HeaderName);
//		Print_F("-----------------------------");
//		Print_F(NewHeaderValue);
//	};
//
//	auto B = [=](FHttpRequestPtr HttpRequest, int32 BytesSent, int32 BytesReceived)
//	{
//		float ReceivedPercentage = HttpRequest->GetElapsedTime();
//
//		Print_F(FString::Printf(TEXT("ReceivedPercentage = %f,BytesSent = %i,BytesReceived = %i"), ReceivedPercentage, BytesSent, BytesReceived));
//	};
//
//	auto C = [=](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
//	{
//		if (HttpRequest.IsValid() &&
//			HttpResponse.IsValid() &&
//			bSucceeded &&
//			EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()))
//		{
//			// If it is a download request, save the file through this code
//			FString Filename = FPaths::GetCleanFilename(HttpRequest->GetURL());//"F:\\PJ\\TestOSS\\EEEEEEEEEE.jpg";
//
//			//	FString tempDir = Get()->SavePaths;//FPaths::ProjectContentDir();
//			FFileHelper::SaveArrayToFile(HttpResponse->GetContent(), *(TEXT("F:\\PJ\\TestOSS\\") + Filename)) /**(tempDir / Filename)*/;
//		}
//	};
//
//	FGetObjectRequest Request(URL);
//
//	Request 
//		<< FHttpRequestHeaderReceivedDelegate::CreateLambda(A) 
//		<< FHttpRequestProgressDelegate::CreateLambda(B) 
//		<< FHttpRequestCompleteDelegate::CreateLambda(C);
//
//	if (!Client.GetObject(Request))
//	{
//		Print_F("false");
//	}
//	
//	return 0;
//}
//int32 OSSTest::MainOSS()
//{
//	return 0;
//}

//int32 OSSTest::MainOSS()
//{
//	auto A = [=](FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue)
//	{
//		Print_F(HeaderName/NewHeaderValue);
//	};
//
//	auto B = [=](FHttpRequestPtr HttpRequest, int32 BytesSent, int32 BytesReceived)
//	{
//		/*float ReceivedPercentage = HttpRequest->GetElapsedTime();
//		Print_F(FString::Printf(TEXT("HttpRequest->GetResponse()->GetResponseCode() = %i"), HttpRequest->GetResponse()->GetResponseCode()));
//		Print_F(HttpRequest->GetResponse()->GetContentAsString());
//		Print_F(FString::Printf(TEXT("ReceivedPercentage = %f,BytesSent = %i,BytesReceived = %i"), ReceivedPercentage, BytesSent, BytesReceived));*/
//	};
//
//	auto C = [=](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
//	{
//		if (HttpRequest.IsValid() &&
//			HttpResponse.IsValid() &&
//			bSucceeded &&
//			EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()))
//		{
//			// If it is a download request, save the file through this code
//			FString Filename = FPaths::GetCleanFilename(HttpRequest->GetURL());//"F:\\PJ\\TestOSS\\EEEEEEEEEE.jpg";
//
//			//	FString tempDir = Get()->SavePaths;//FPaths::ProjectContentDir();
//			FFileHelper::SaveArrayToFile(HttpResponse->GetContent(), *(TEXT("F:\\PJ\\TestOSS\\") + Filename)) /**(tempDir / Filename)*/;
//		
//			Print_F(HttpResponse->GetContentAsString());
//		}
//	};	
//	FString URL = "https://helloworldd.oss-cn-beijing.aliyuncs.com";
//	FHTTPClient Client;
//	FGetObjectsInfoRequest Request(URL);
//	Request
//		<< FHttpRequestHeaderReceivedDelegate::CreateLambda(A)
//		<< FHttpRequestProgressDelegate::CreateLambda(B)
//		<< FHttpRequestCompleteDelegate::CreateLambda(C);
//
//	if (!Client.GetObjectInfo(Request))
//	{
//		Print_F("false");
//	}
//
//	return 0;
//}
#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif

#if 0
#include "alibabacloud/oss/OssClient.h"
#include "FileHelper.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

using namespace AlibabaCloud::OSS;

//通用的账号注册
std::string AccessKeyId = "XXXX";
std::string AccessKeySecret = "XXXX";// 保密;
std::string Endpoint = "oss-cn-beijing.aliyuncs.com";

//创建桶
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	/* 指定新创建bucket的名称、存储级别和ACL */
//	CreateBucketRequest Request(
//		BucketName, 
//		StorageClass::IA,
//		CannedAccessControlList::PublicReadWrite);
//
//	auto Outcome = Client.CreateBucket(Request);
//
//	if (!Outcome.isSuccess())
//	{
//		ShutdownSdk();
//		return -1;
//	}
//
//	/* 释放网络等资源 */
//	ShutdownSdk();
//
//	return 0;
//}

#include "alibabacloud/oss/client/RateLimiter.h"

using namespace std;
class FTestRateLimiter : public RateLimiter
{
public:
	FTestRateLimiter()
		:M_Rate(0)
	{}
	~FTestRateLimiter() {};
	virtual void setRate(int rate) { M_Rate = rate; };
	virtual int Rate() const { return M_Rate; };
private:
	int32 M_Rate;
};
//
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//
//	//设置超时时间
//	{
//		conf.maxConnections = 17;
//		conf.requestTimeoutMs = 8000;
//		conf.connectTimeoutMs = 8000;
//	}
//	//限速处理
//	{
//		auto sendrateLimiter = make_shared<FTestRateLimiter>();
//		auto recvrateLimiter = make_shared<FTestRateLimiter>();
//		conf.sendRateLimiter = sendrateLimiter;
//		conf.recvRateLimiter = recvrateLimiter;
//	}
//
//	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//	//限速处理
//	{ 
//		/* 设置下载限速（单位KB/s） */
//		conf.recvRateLimiter->setRate(400);
//		/* 设置上传限速(单位KB/s） */
//		conf.sendRateLimiter->setRate(400);
//	}
//
//	auto outcome = Client.PutObject(BucketName,"TestOSS.zip","F:/PJ/TestOSS/TestOSS.zip");
//	{
//		/* 设置上传限速(单位KB/s） */
//		conf.sendRateLimiter->setRate(600);
//	}
//	
//	/* 释放网络等资源 */
//	ShutdownSdk();
//
//	return 0;
//}
//
//#include "alibabacloud/oss/client/RetryStrategy.h"
//class FTestRetryStrategy : public RetryStrategy
//{
//public:
//	FTestRetryStrategy(long maxRetries = 3, long scaleFactor = 300) 
//		:m_ScaleFactor(scaleFactor)
//		, m_MaxRetries(maxRetries)
//	{}
//
//	/* 用户可自定义shouldRetry函数，用于判断是否进行重试 */
//	bool shouldRetry(const Error & error, long attemptedRetries) const;
//
//	/* 用户可自定义calcDelayTimeMs函数，用于计算重试的延迟等待时间 */
//	long calcDelayTimeMs(const Error & error, long attemptedRetries) const;
//private:
//	long m_ScaleFactor;//重试等待时间的尺度因子
//	long m_MaxRetries;//最多重试次数
//};
//
//bool FTestRetryStrategy::shouldRetry(const Error & error, long attemptedRetries) const
//{
//	if (attemptedRetries >= m_MaxRetries)
//	{
//		return false;
//	}
//
//	long responseCode = error.Status();
//
//	//100-199 用于指定客户端应相应的某些动作。
//	//200-299 用于表示请求成功。
//	//300-399 用于已经移动的文件并且常被包含在定位头信息中指定新的地址信息。
//	//400-499 用于指出客户端的错误。
//	//500-599 用于支持服务器错误。
//	//http code
//	if ((responseCode == 403 && error.Message().find("RequestTimeTooSkewed")) ||
//		(responseCode > 499 && responseCode < 599))
//	{
//		return true;
//	}
//	else
//	{
//		switch (responseCode)
//		{
//			//curl error code
//		case CURLE_COULDNT_CONNECT:  //无法通过 connect() 连接至主机或代理服务器。
//		case CURLE_PARTIAL_FILE: //文件传输尺寸小于或大于预期。 当服务器先报告了一个预期的传输尺寸，然后所传送的数据与先前指定尺寸不相符时，就会发生此错误。
//		case CURLE_WRITE_ERROR: //在向本地文件写入所收到的数据时发生错误，或由写入回调(write callback) 向 libcurl 返回了一个错误。
//		case CURLE_OPERATION_TIMEDOUT: //操作超时。 已达到根据相应情况指定的超时时间。 请注意： 自 Urchin 6.6.0.2 开始，超时时间可以自行更改。 要指定远程日志下载超时，请打开 urchin.conf 文件，取消以下行的注释标记：
//		case CURLE_GOT_NOTHING: //服务器未返回任何数据，在相应情况下，未返回任何数据就属于出现错误。
//		case CURLE_SEND_ERROR: //无法发送网络数据。
//		case CURLE_RECV_ERROR: //接收网络数据失败。
//			return true;
//		default:
//			break;
//		}
//	}
//
//	return false;
//}
///* 用户可自定义calcDelayTimeMs函数，用于计算重试的延迟等待时间 */
//long FTestRetryStrategy::calcDelayTimeMs(const Error & error, long attemptedRetries) const
//{
//	UNUSED_PARAM(error);
//	return (1 << attemptedRetries) * m_ScaleFactor;
//}
//
////重试
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//
//	/* 设置失败请求重试次数，默认为3次 */
//	auto defaultRetryStrategy = std::make_shared<FTestRetryStrategy>(8);
//	conf.retryStrategy = defaultRetryStrategy;
//
//	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//	
//
//	/* 释放网络等资源 */
//	ShutdownSdk();
//
//	return 0;
//}

//打印
void Print_F(const std::string &NewStr)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, NewStr.c_str());
	}
}
void Print_F(const FString &NewStr)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, NewStr);
	}
}
//
////列举桶
//int32 OSSTest::MainOSS()
//{
////	std::string BucketName = "helloworldd";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//
//	/* 列举存储空间 */
//	ListBucketsRequest request;
//
//	auto Outcome = Client.ListBuckets(request);
//
//	if (!Outcome.isSuccess())
//	{
//		ShutdownSdk();
//		return -1;
//	}
//
//	for (auto result : Outcome.result().Buckets())
//	{
//		Print_F(result.Name());
//	}
//
//	/* 释放网络等资源 */
//	ShutdownSdk();
//
//	return 0;
//}


////列举桶
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//
//		/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	/* 设置存储空间访问权限 */
//	SetBucketAclRequest request(BucketName, CannedAccessControlList::PublicRead);
//	auto Outcome = Client.SetBucketAcl(request);
//
//	if (!Outcome.isSuccess())
//	{
//		ShutdownSdk();
//		return -1;
//	}
//
//	/* 释放网络等资源 */
//	ShutdownSdk();
//
//	return 0;
//}

////获取地域
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	/* 获取存储空间地域 */
//	GetBucketLocationRequest request(BucketName);
//	auto Outcome = Client.GetBucketLocation(request);
//
//	//是否存在这个桶
//	if (Client.DoesBucketExist(BucketName))
//	{
//
//	}
//
//	if (!Outcome.isSuccess())
//	{
//		ShutdownSdk();
//		return -1;
//	}
//
//	Print_F(Outcome.result().Location());
//
//	/* 释放网络等资源 */
//	ShutdownSdk();
//
//	return 0;
//}


////ListMultipartUploads
////AbortMultipartUpload
////删除桶
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "10eer1";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient Client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	/* 指定要删除的bucket名称 */
//	DeleteBucketRequest request(BucketName);
//
//	/* 删除bucket */
//	auto outcome = Client.DeleteBucket(request);
//
//	if (!outcome.isSuccess())
//	{
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//
//		ShutdownSdk();
//		return -1;
//	}
//
//	/* 释放网络等资源 */
//	ShutdownSdk();
//
//	return 0;
//}

//设置请求者付费模式
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "10eer";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	/* 设置请求者付费模式 */
//	SetBucketRequestPaymentRequest request(BucketName);
//
//	request.setRequestPayer(RequestPayer::Requester);
//
//	auto outcome = client.SetBucketRequestPayment(request);
//	if (!outcome.isSuccess()) 
//	{
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//
//		ShutdownSdk();
//		return -1;
//	}
//
//	ShutdownSdk();
//	return 0;
//}

//获取请求者付费模式
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "10eer";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	/* 获取请求者付费模式 */
//	GetBucketRequestPaymentRequest request(BucketName);
//	auto outcome = client.GetBucketRequestPayment(request);
//
//	if (!outcome.isSuccess())
//	{
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//
//		ShutdownSdk();
//		return -1;
//	}
//
//	RequestPayer CC = outcome.result().Payer();
//
//	ShutdownSdk();
//	return 0;
//}

//授权
//#include <fstream>
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "pooowdsfawdsadw";
//	std::string PayerAccessKeyId = "LTAI4FqU8PLRK6nZi318Fz63";
//	std::string PayerAccessKeySecret = "*****************************";
//	std::string PayerEndpoint = "oss-cn-beijing.aliyuncs.com";
//
//	std::string ObjectName = "1C3C79E04D88B3DABBFBDD9D92379233";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);//旧账户
//	OssClient payerClient(PayerEndpoint, PayerAccessKeyId, PayerAccessKeySecret, conf);//新账户
//
//	/* 创建bucket */
//	CreateBucketOutcome outCome = client.CreateBucket(CreateBucketRequest(BucketName));
//
//	/* 设置请求者付费模式 */
//	SetBucketRequestPaymentRequest request(BucketName);
//	request.setRequestPayer(RequestPayer::Requester);
//	auto payoutcome = client.SetBucketRequestPayment(request);
//
//	/* 上传文件，并设置请求者付费 */
//	std::shared_ptr<std::iostream> content = std::make_shared<std::fstream>(
//		"F://PJ//TestOSS//1C3C79E04D88B3DABBFBDD9D92379233",
//		std::ios::in | std::ios::binary);
//	PutObjectRequest putrequest(BucketName, ObjectName, content);
//	putrequest.setRequestPayer(RequestPayer::Requester);
//	auto putoutcome = payerClient.PutObject(putrequest);
//
//	/* 获取文件到本地内存，并设置请求者付费 */
//	GetObjectRequest getrequest(BucketName, ObjectName);
//	getrequest.setRequestPayer(RequestPayer::Requester);
//	auto getoutcome = payerClient.GetObject(getrequest);
//
//	/* 删除文件，并设置请求者付费 */
//	DeleteObjectRequest delrequest(BucketName, ObjectName);
//	delrequest.setRequestPayer(RequestPayer::Requester);
//	auto deloutcome = payerClient.DeleteObject(delrequest);
//
//	ShutdownSdk();
//	return 0;
//}

//
////从内存上传
//int32 OSSTest::MainOSS()
//{
//	//先读进内存
//	TArray<uint8> StringArray;
//	FFileHelper::LoadFileToArray(StringArray, *FString("F:/PJ/TestOSS/WWW.txt"));
//
//	string BucketName = "helloworldd";
//	std::string ObjectName = "WWW.txt";
//
//	InitializeSdk();
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//	std::shared_ptr<std::iostream> content = std::make_shared<std::stringstream>();
//	*content << "hello~";
////	cout << "sdsd" << endl;
//	PutObjectRequest request(BucketName, ObjectName, content);
//
//	auto outcome = client.PutObject(request);
//	if (!outcome.isSuccess())
//	{
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//
//		ShutdownSdk();
//		return -1;
//	}
//
//	ShutdownSdk();
//	return 0;
//}

////本地上传
//int32 OSSTest::MainOSS()
//{
//	string BucketName = "helloworldd";
//	std::string ObjectName = "EEEWWW.txt";
//
//	InitializeSdk();
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	auto outcome = client.PutObject(BucketName,ObjectName,"F:/PJ/TestOSS/EEE.txt");
//
//	if (!outcome.isSuccess())
//	{
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//
//		ShutdownSdk();
//		return -1;
//	}
//
//	ShutdownSdk();
//	return 0;
//}

//断点续传
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string ObjectName = "1-1StoneDefenc2e.mp4";
//	std::string UploadFilePath = "F:\\PJ\\TestOSS\\1-1StoneDefenc2e.mp4";
//	std::string CheckpointFilePath = "F:\\PJ\\TestOSS\\Hello";
//
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	UploadObjectRequest request(BucketName, ObjectName, UploadFilePath, CheckpointFilePath);
//	auto outcome = client.ResumableUploadObject(request);
//
//	if (!outcome.isSuccess())
//	{
//		/* 异常处理 */
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//
//		ShutdownSdk();
//		return -1;
//	}
//
//	ShutdownSdk();
//	return 0;
//}

//中断上传
#include <fstream>
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string ObjectName = "1-1StoneDefenc2e.mp4";
//
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	std::vector<PutObjectOutcomeCallable> Callables;
//	for (int32 i = 0; i < 4; i++)
//	{
//		std::shared_ptr<std::iostream> 
//			content = std::make_shared<std::fstream>(
//			"F:\\PJ\\TestOSS\\1-1StoneDefenc2e.mp4", 
//				std::ios::in | std::ios::binary);
//
//		PutObjectRequest request(BucketName, ObjectName, content);
//		auto outcomeCallable = client.PutObjectCallable(request);
//		Callables.emplace_back(std::move(outcomeCallable));
//	}
//
//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//
//	/* 中断上传 */
//	client.DisableRequest();
//	for (int32 i = 0; i < Callables.size(); i++)
//	{
//		auto outcome = Callables[i].get();
//		if (outcome.error().Code() == "ClientError:100002" ||
//			outcome.error().Code() == "ClientError:200042")
//		{
//			Print_F("disable putobject success");
//		}
//	}
//
//	ShutdownSdk();
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
//分片上传
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string ObjectName = "1-1StoneDefenc2e.mp4";
//
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	InitiateMultipartUploadRequest initUploadRequest(BucketName, ObjectName);
//	auto uploadIdResult = client.InitiateMultipartUpload(initUploadRequest);
//	auto uploadId = uploadIdResult.result().UploadId();
//	std::string fileToUpload = "F:\\PJ\\TestOSS\\1-1StoneDefenc2e.mp4";
//	int64_t partSize = 1024 * 1024 * 10;
//	PartList partETagList;
//
//	auto fileSize = getFileSize(fileToUpload);
//	int32 partCount = static_cast<int32> (fileSize / partSize);
//
//	/* 计算分片个数 */
//	if (fileSize % partSize != 0) 
//	{
//		partCount++;
//	}
//
//	for (int32 i = 1; i <= partCount; i++)
//	{
//		auto skipBytes = partSize * (i - 1);
//		auto size = (partSize < fileSize - skipBytes) ? partSize : (fileSize - skipBytes);
//		std::shared_ptr<std::iostream> content = std::make_shared<std::fstream>(fileToUpload, std::ios::in | std::ios::binary);
//		
//		//std::ios::beg std::ios::end
//		content->seekg(skipBytes, std::ios::beg);
//
//		UploadPartRequest uploadPartRequest(BucketName, ObjectName, content);
//		uploadPartRequest.setContentLength(size);
//		uploadPartRequest.setUploadId(uploadId);
//		uploadPartRequest.setPartNumber(i);
//
//		auto uploadPartOutcome = client.UploadPart(uploadPartRequest);
//		if (uploadPartOutcome.isSuccess()) 
//		{
//			Part part(i, uploadPartOutcome.result().ETag());
//			partETagList.push_back(part);
//		}
//		else
//		{
//			Print_F(uploadPartOutcome.error().Code());
//			Print_F(uploadPartOutcome.error().Message());
//			Print_F(uploadPartOutcome.error().RequestId());
//		}
//	}
//	//列举我们的分片
//	//{
//	//	/* 列举已上传分片,默认列举1000个分片 */
//	//	ListPartsRequest listuploadrequest(BucketName, ObjectName);
//	//	listuploadrequest.setMaxParts(2);
//	//	listuploadrequest.setUploadId(uploadId);
//	//	ListPartsOutcome listuploadresult;
//
//	//	do 
//	//	{
//	//		listuploadresult = client.ListParts(listuploadrequest);
//	//		if (!listuploadresult.isSuccess())
//	//		{
//	//			/* 异常处理 */
//	//			Print_F(listuploadresult.error().Code());
//	//			Print_F(listuploadresult.error().Message());
//	//			Print_F(listuploadresult.error().RequestId());
//	//			break;
//	//		}
//	//		else
//	//		{
//	//			for (const auto& part : listuploadresult.result().PartList())
//	//			{
//	//				Print_F(std::to_string(part.PartNumber()));
//	//				Print_F(std::to_string(part.Size()));
//	//				Print_F(part.ETag());
//	//				Print_F(part.LastModified());
//	//			}
//	//		}
//	//			
//	//		listuploadrequest.setPartNumberMarker(listuploadresult.result().NextPartNumberMarker());
//	//	} while (listuploadresult.result().IsTruncated());
//	//}
//
//	//列举分片上传事件
//	//{
//	//	ListMultipartUploadsRequest listmultiuploadrequest(BucketName);
//	//	listmultiuploadrequest.setMaxUploads(2);
//	//	ListMultipartUploadsOutcome listresult;
//	//	do 
//	//	{
//	//		listresult = client.ListMultipartUploads(listmultiuploadrequest);
//	//		if (!listresult.isSuccess())
//	//		{
//	//			/* 异常处理 */
//	//			Print_F(listresult.error().Code());
//	//			Print_F(listresult.error().Message());
//	//			Print_F(listresult.error().RequestId());
//	//			break;
//	//		}
//	//		else
//	//		{
//	//			for (const auto& part : listresult.result().MultipartUploadList())
//	//			{
//	//				Print_F(part.Key);
//	//				Print_F(part.UploadId);
//	//				Print_F(part.Initiated);
//	//			}
//	//		}
//	//		listmultiuploadrequest.setKeyMarker(listresult.result().NextKeyMarker());
//	//		listmultiuploadrequest.setUploadIdMarker(listresult.result().NextUploadIdMarker());
//	//	} while (listresult.result().IsTruncated());
//	//}
//
//	//取消分片上传
//	{
//		AbortMultipartUploadRequest  abortUploadRequest(BucketName, ObjectName, uploadId);
//		auto abortUploadIdResult = client.AbortMultipartUpload(abortUploadRequest);
//		if (!abortUploadIdResult.isSuccess())
//		{
//			Print_F(abortUploadIdResult.error().Code());
//			Print_F(abortUploadIdResult.error().Message());
//			Print_F(abortUploadIdResult.error().RequestId());
//			ShutdownSdk();
//			return -1;
//		}
//		ShutdownSdk();
//		return 0;
//	}
//
//	CompleteMultipartUploadRequest request(BucketName, ObjectName);
//	request.setUploadId(uploadId);
//	request.setPartList(partETagList);
//	auto outcome = client.CompleteMultipartUpload(request);
//	if (!outcome.isSuccess())
//	{
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//
//	ShutdownSdk();
//	return 0;
//}

void ProgressCallback(size_t increment, int64_t transfered, int64_t total, void* userData)
{
	Print_F(FString::Printf(TEXT("increment = %i,transfered =%i ,total = %i"), increment, transfered, total));
}
//进度上传
#include "Core/ThreadCoreMacro.h"
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string ObjectName = "1-1StoneDefenc2e.mp4";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	std::shared_ptr<std::iostream> content = std::make_shared<std::fstream>("F:\\PJ\\TestOSS\\1-1StoneDefenc2e.mp4", std::ios::in | std::ios::binary);
//	PutObjectRequest request(BucketName, ObjectName, content);
//	TransferProgress progressCallback = { ProgressCallback , nullptr };
//	request.setTransferProgress(progressCallback);
//
//	auto OutLamada = [=]()
//	{
//		auto outcome = client.PutObject(request);
//		if (!outcome.isSuccess())
//		{
//			Print_F(outcome.error().Code());
//			Print_F(outcome.error().Message());
//			Print_F(outcome.error().RequestId());
//		}
//		ShutdownSdk();
//	};
//
//	ASYNCTASK_Lambda(OutLamada);
//
//	return 0;
//}
//下载到本地
//#include <memory>
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string ObjectName = "EEEWWW.txt";
//
//	std::string FileNametoSave = "F:\\PJ\\TestOSS\\EEEEEEEEEE.txt";
//
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	GetObjectRequest request(BucketName, ObjectName);
//	request.setResponseStreamFactory([=]() {return std::make_shared<std::fstream>(FileNametoSave, std::ios_base::out | std::ios_base::in | std::ios_base::trunc | std::ios_base::binary); });
//	
//	auto outcome = client.GetObject(request);
//	if (outcome.isSuccess())
//	{
//		Print_F(to_string(outcome.result().Metadata().ContentLength()));
//	}
//	else
//	{
//		/*异常处理*/
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//
//	ShutdownSdk();
//	return 0;
//}
//下载到本地内存
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string ObjectName = "EEEWWW.txt";
//
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	GetObjectRequest request(BucketName, ObjectName);
//
//	auto outcome = client.GetObject(request);
//	if (outcome.isSuccess())
//	{
//		Print_F(to_string(outcome.result().Metadata().ContentLength()));
//		
//		/*打印下载内容*/
//		std::string content;
//		*(outcome.result().Content()) >> content;
//		Print_F(content);
//	}
//	else
//	{
//		/*异常处理*/
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//
//	ShutdownSdk();
//	return 0;
//}

//范围下载
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string ObjectName = "EEEWWW.txt";
//
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	GetObjectRequest request(BucketName, ObjectName);
//	request.setRange(0, 2);
//	auto outcome = client.GetObject(request);
//	if (outcome.isSuccess())
//	{
//		Print_F(to_string(outcome.result().Metadata().ContentLength()));
//
//		/*打印下载内容*/
//		std::string content;
//		*(outcome.result().Content()) >> content;
//		Print_F(content);
//	}
//	else
//	{
//		/*异常处理*/
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//
//	ShutdownSdk();
//	return 0;
//}

////分片下载
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "10eer";
//	std::string ObjectName = "11B9DA17474BEB54CD82248028D8B607";
//
//	std::string DownloadFilePath = "F://PJ//TestOSS//11B9DA17474BEB54CD82248028D8B607";
//	std::string CheckpointFilePath = "F://PJ//TestOSS//Hello";
//
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	DownloadObjectRequest request(BucketName, ObjectName, DownloadFilePath, CheckpointFilePath);
//	auto outcome = client.ResumableDownloadObject(request);
//	if (!outcome.isSuccess())
//	{
//		/*异常处理*/
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//
//	ShutdownSdk();
//	return 0;
//}

//下载显示进度条
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "10eer";
//	std::string ObjectName = "11B9DA17474BEB54CD82248028D8B607";
//
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	GetObjectRequest request(BucketName, ObjectName);
//	TransferProgress progressCallback = { ProgressCallback , nullptr };
//	request.setTransferProgress(progressCallback);
//
//	request.setResponseStreamFactory([=]() 
//	{
//		return std::make_shared<std::fstream>(
//			"F:\\PJ\\TestOSS\\ERTDGGR", 
//			std::ios_base::out | 
//			std::ios_base::in | 
//			std::ios_base::trunc |
//			std::ios_base::binary);
//	});
//
//	auto OutLamada = [=]()
//	{
//		auto outcome = client.GetObject(request);
//		if (!outcome.isSuccess())
//		{
//			Print_F(outcome.error().Code());
//			Print_F(outcome.error().Message());
//			Print_F(outcome.error().RequestId());
//		}
//		ShutdownSdk();
//	};
//
//	ASYNCTASK_Lambda(OutLamada);
//
//	return 0;
//}


//设置文件读写权限
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string ObjectName = "EEE.txt";
//
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	SetObjectAclRequest request(BucketName, ObjectName);
//	request.setAcl(CannedAccessControlList::Private);
//
//	auto outcome = client.SetObjectAcl(request);
//
//	if (!outcome.isSuccess())
//	{
//		/*异常处理*/
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//		
//	ShutdownSdk();
//	return 0;
//}

//获取读写权限
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string ObjectName = "EEE.txt";
//
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	GetObjectAclRequest request(BucketName, ObjectName);
//	auto outcome = client.GetObjectAcl(request);
//
//	if (!outcome.isSuccess())
//	{
//		/*异常处理*/
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//	else
//	{
//		Print_F(to_string(outcome.result().Acl()));
//	}
//
//	ShutdownSdk();
//	return 0;
//}

//定义元数据
//Content-Type
//https://help.aliyun.com/knowledge_detail/39522.html?spm=a2c4g.11186623.4.2.6386370c1sfuf4
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string ObjectName = "EEE.txt";
//
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	auto meta = ObjectMetaData();
//	meta.setContentType("text/plain");
//	meta.setCacheControl("max-age=3");
//
//	//meta.setContentDisposition()
//	//meta.setContentEncoding()
//	//meta.setContentLength()
//	//meta.setExpirationTime()
//
//	meta.UserMetaData()["Hello"] = "No-value";
//	std::shared_ptr<std::iostream> content = std::make_shared<std::stringstream>();
//	*content << "Thank you for using hello mate Storage Service!";
//
//	auto outcome = client.PutObject(BucketName, ObjectName, content, meta);
//	if (!outcome.isSuccess())
//	{
//		/*异常处理*/
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//
//	ShutdownSdk();
//	return 0;
//}

//获取元数据
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string ObjectName = "EEE.txt";
//
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	/* 获取文件的部分元信息 */
//	auto outcome = client.GetObjectMeta(BucketName, ObjectName);
//	
//	if (!outcome.isSuccess())
//	{
//		/*异常处理*/
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//	else
//	{
//		auto metadata = outcome.result();
//		Print_F(metadata.ETag());
//		Print_F(metadata.LastModified());
//		Print_F(to_string(metadata.ContentLength()));
//	}
//
//	/* 获取文件的全部元信息 */
//	outcome = client.HeadObject(BucketName, ObjectName);
//	if (!outcome.isSuccess())
//	{
//		/*异常处理*/
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//	else
//	{
//		auto headMeta = outcome.result();
//		Print_F(headMeta.ContentType());
//		Print_F(to_string(headMeta.ContentLength()));
//		Print_F(headMeta.CacheControl());
//		Print_F(headMeta.ContentDisposition());
//		Print_F(headMeta.ContentEncoding());
//		Print_F(headMeta.ContentMd5());
//	}
//
//	ShutdownSdk();
//	return 0;
//}

//简单列举文件
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	ListObjectsRequest request(BucketName);
//	auto outcome = client.ListObjects(request);
//	if (!outcome.isSuccess())
//	{
//		/*异常处理*/
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//	else
//	{
//		for (const auto& object : outcome.result().ObjectSummarys())
//		{
//			Print_F(TEXT("------------------------"));
//			Print_F(object.Key());
//			Print_F(to_string(object.Size()));
//			Print_F(object.LastModified());
//		}
//	}
//
//	ShutdownSdk();
//	return 0;
//}

////列举指定个数的文件
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	ListObjectsRequest request(BucketName);
//	request.setMaxKeys(1);
//	auto outcome = client.ListObjects(request);
//	if (!outcome.isSuccess())
//	{
//		/*异常处理*/
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//	else
//	{
//		for (const auto& object : outcome.result().ObjectSummarys())
//		{
//			Print_F(TEXT("------------------------"));
//			Print_F(object.Key());
//			Print_F(to_string(object.Size()));
//			Print_F(object.LastModified());
//		}
//	}
//
//	ShutdownSdk();
//	return 0;
//}

//列举指定目录下的文件和子目录
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string keyPrefix = "UU/";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	std::string nextMarker = "";
//	ListObjectOutcome outcome;
//	do
//	{
//		/* 列举文件 */
//		ListObjectsRequest request(BucketName);
//		/* 设置正斜线（/）为文件夹的分隔符 */
//		request.setDelimiter("/");
//		request.setPrefix(keyPrefix);
//		request.setMarker(nextMarker);
//
//		outcome = client.ListObjects(request);
//		if (!outcome.isSuccess())
//		{
//			/*异常处理*/
//			Print_F(outcome.error().Code());
//			Print_F(outcome.error().Message());
//			Print_F(outcome.error().RequestId());
//			break;
//		}
//
//		for (const auto& object : outcome.result().ObjectSummarys())
//		{	
//			Print_F(object.Key());
//			Print_F(to_string(object.Size()));
//			Print_F(object.LastModified());
//			Print_F(TEXT("-----------object-------------"));
//		}
//
//		for (const auto& commonPrefix : outcome.result().CommonPrefixes())
//		{	
//			Print_F(commonPrefix);
//			Print_F(TEXT("------------commonPrefix------------"));
//		}
//		nextMarker = outcome.result().NextMarker();
//	} while (outcome.result().IsTruncated());
//
//	ShutdownSdk();
//	return 0;
//}

////列举指定 marker 之后的文件
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string nextMarker = "UU/";
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	ListObjectOutcome outcome;
//	do
//	{
//		/* 列举文件 */
//		ListObjectsRequest request(BucketName);
//		request.setMarker(nextMarker);
//
//		outcome = client.ListObjects(request);
//		if (!outcome.isSuccess())
//		{
//			/*异常处理*/
//			Print_F(outcome.error().Code());
//			Print_F(outcome.error().Message());
//			Print_F(outcome.error().RequestId());
//			break;
//		}
//
//		for (const auto& object : outcome.result().ObjectSummarys())
//		{	
//			Print_F(object.Key());
//			Print_F(to_string(object.Size()));
//			Print_F(object.LastModified());
//			Print_F(TEXT("-----------object-------------"));
//		}
//
//		for (const auto& commonPrefix : outcome.result().CommonPrefixes())
//		{	
//			Print_F(commonPrefix);
//			Print_F(TEXT("------------commonPrefix------------"));
//		}
//		nextMarker = outcome.result().NextMarker();
//	} while (outcome.result().IsTruncated());
//
//	ShutdownSdk();
//	return 0;
//}

//单引号（'）
//双引号（"）
//and符号（&）
//尖括号（< >）
//顿号（、）
//中文
//指定文件名称编码
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string nextMarker = "";
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	ListObjectOutcome outcome;
//	do
//	{
//		/* 列举文件 */
//		ListObjectsRequest request(BucketName);
//		request.setMarker(nextMarker);
//		request.setEncodingType("url");
//
//		outcome = client.ListObjects(request);
//		if (!outcome.isSuccess())
//		{
//			/*异常处理*/
//			Print_F(outcome.error().Code());
//			Print_F(outcome.error().Message());
//			Print_F(outcome.error().RequestId());
//			break;
//		}
//
//		for (const auto& object : outcome.result().ObjectSummarys())
//		{
//			Print_F(object.Key());
//			Print_F(to_string(object.Size()));
//			Print_F(object.LastModified());
//			Print_F(TEXT("-----------object-------------"));
//		}
//
//		nextMarker = outcome.result().NextMarker();
//	} while (outcome.result().IsTruncated());
//
//	ShutdownSdk();
//	return 0;
//}
//判断当前对象是否存在
void DoesObjectExist(const OssClient &client, const std::string &BucketName, const std::string &ObjectName)
{
	/* 判断文件是否存在 */
	if (client.DoesObjectExist(BucketName, ObjectName))
	{
		Print_F(TEXT("Y"));
	}
	else
	{
		Print_F(TEXT("N"));
	}
}
//删除单个
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string ObjectName = "WWW.txt";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	DeleteObjectRequest request(BucketName, ObjectName);
//
//	DoesObjectExist(client, BucketName, ObjectName);
//
//	/* 删除文件 */
//	auto outcome = client.DeleteObject(request);
//
//	if (!outcome.isSuccess())
//	{
//		/*异常处理*/
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//
//	DoesObjectExist(client, BucketName, ObjectName);
//
//	ShutdownSdk();
//	return 0;
//}

//删除多个对象
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string ObjectName1 = "EEEWWW.txt";
//	std::string ObjectName2 = "UU/EEEWe.txt";
//	std::string ObjectName3 = "UU/TT/EEEWWWWEERW.txt";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	DeleteObjectsRequest request(BucketName);
//	request.addKey(ObjectName1);
//	request.addKey(ObjectName2);
//	request.addKey(ObjectName3);
//
//	/* 删除文件 */
//	auto outcome = client.DeleteObjects(request);
//
//	if (!outcome.isSuccess())
//	{
//		/*异常处理*/
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//
//	ShutdownSdk();
//	return 0;
//}

//拷贝小文件
//int32 OSSTest::MainOSS()
//{
//	std::string SourceBucketName = "helloworldd";
//	std::string CopyBucketName = "helloworldd";
//	std::string SourceObjectName = "UU/EEEWe.txt";
//	std::string CopyObjectName = "EEEWe.txt";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	CopyObjectRequest request(CopyBucketName, CopyObjectName);
//	request.setCopySource(SourceBucketName, SourceObjectName);
//
//	auto outcome = client.CopyObject(request);
//
//	if (!outcome.isSuccess())
//	{
//		/*异常处理*/
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//
//	ShutdownSdk();
//	return 0;
//}

//分片拷贝
//int32 OSSTest::MainOSS()
//{
//	std::string SourceBucketName = "10eer";
//	std::string CopyBucketName = "helloworldd";
//	std::string SourceObjectName = "1A25C5164E5CEC4C41B88588F5201B84";
//	std::string CopyObjectName = "WWWW";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	auto getObjectMetaReq = GetObjectMetaRequest(SourceBucketName, SourceObjectName);
//	auto getObjectMetaResult = client.GetObjectMeta(getObjectMetaReq);
//	if (!getObjectMetaResult.isSuccess())
//	{
//		/*异常处理*/
//		Print_F(getObjectMetaResult.error().Code());
//		Print_F(getObjectMetaResult.error().Message());
//		Print_F(getObjectMetaResult.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//	/* 获取被拷贝文件大小 */
//	auto objectSize = getObjectMetaResult.result().ContentLength();
//
//	/* 拷贝大文件 */
//	InitiateMultipartUploadRequest initUploadRequest(CopyBucketName, CopyObjectName);
//
//	/* 初始化分片拷贝事件 */
//	auto uploadIdResult = client.InitiateMultipartUpload(initUploadRequest);
//
//	auto uploadId = uploadIdResult.result().UploadId();
//	int64_t partSize = 1024 * 1024*10;
//	int partCount = static_cast<int>(objectSize / partSize);//30
//
//	 /* 计算分片个数*/
//	if (objectSize % partSize != 0)
//	{
//		partCount++;
//	}
//	PartList partETagList;
//	/* 对每一个分片进行拷贝 */
//	for (int i = 1; i <= partCount; i++)
//	{
//		auto skipBytes = partSize * (i - 1);
//		auto size = (partSize < objectSize - skipBytes) ? partSize : (objectSize - skipBytes);
//		auto uploadPartCopyReq = UploadPartCopyRequest(CopyBucketName, CopyObjectName, SourceBucketName, SourceObjectName, uploadId, i + 1);
//		uploadPartCopyReq.setCopySourceRange(skipBytes, skipBytes + size - 1);
//		auto uploadPartOutcome = client.UploadPartCopy(uploadPartCopyReq);
//		if (uploadPartOutcome.isSuccess()) 
//		{
//			Part part(i, uploadPartOutcome.result().ETag());
//			partETagList.push_back(part);
//		}
//		else
//		{
//			/*异常处理*/
//			Print_F(getObjectMetaResult.error().Code());
//			Print_F(getObjectMetaResult.error().Message());
//			Print_F(getObjectMetaResult.error().RequestId());
//		}
//	}
//
//	CompleteMultipartUploadRequest request(CopyBucketName, CopyObjectName, partETagList, uploadId);
//	auto outcome = client.CompleteMultipartUpload(request);
//	if (!outcome.isSuccess())
//	{
//		/*异常处理*/
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//	
//	ShutdownSdk();
//	return 0;
//}
//将Object的存储类型从标准或低频访问转换为归档类型：
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string ObjectName = "EEEWe.txt";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	ObjectMetaData objectMeta;
//	objectMeta.addHeader("x-oss-storage-class", "Archive");//Standard、IA、Archive
//	CopyObjectRequest request(BucketName, BucketName, objectMeta);
//	request.setCopySource(BucketName, ObjectName);
//
//	/* 修改为上述指定的文件存储类型 */
//	auto outcome = client.CopyObject(request);
//	if (!outcome.isSuccess())
//	{
//		/*异常处理*/
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//			
//	ShutdownSdk();
//	return 0;
//}

//
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string ObjectName = "EEEWe.txt";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	/* 检查目标文件是否为归档类型。如果是，则需要先解冻才能更改存储类型 */
//	auto restoreOutcome = client.RestoreObject(BucketName, ObjectName);
//	if (!restoreOutcome.isSuccess())
//	{
//		/*异常处理*/
//		Print_F(restoreOutcome.error().Code());
//		Print_F(restoreOutcome.error().Message());
//		Print_F(restoreOutcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//
//	std::string onGoingRestore("ongoing-request=\"false\"");
//	int32 maxWaitTimeInSeconds = 600;
//	while (maxWaitTimeInSeconds > 0)
//	{
//		auto meta = client.HeadObject(BucketName, ObjectName);
//		std::string restoreStatus = meta.result().HttpMetaData()["x-oss-restore"];
//		std::transform(restoreStatus.begin(), restoreStatus.end(), restoreStatus.begin(), ::tolower);//toupper
//		if (!restoreStatus.empty() &&
//			restoreStatus.compare(0, onGoingRestore.size(), onGoingRestore) == 0)
//		{	
//			/* 成功解冻归档文件*/
//			Print_F(restoreStatus);
//		
//			break;
//		}
//
//		std::this_thread::sleep_for(std::chrono::seconds(10));
//		maxWaitTimeInSeconds--;
//	}
//
//	ObjectMetaData objectMeta;
//	objectMeta.addHeader("x-oss-storage-class", "IA");//Standard、IA、Archive
//	CopyObjectRequest request(BucketName, BucketName, objectMeta);
//	request.setCopySource(BucketName, ObjectName);
//
//	/* 修改为上述指定的文件存储类型 */
//	auto outcome = client.CopyObject(request);
//	if (!outcome.isSuccess())
//	{
//		/*异常处理*/
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//
//	ShutdownSdk();
//	return 0;
//}

//创建软链接
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string ObjectName = "EEEWe.txt";
//	std::string LinkObjectName = "WE.txt";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	/* 设置HTTP header */
//	auto meta = ObjectMetaData();
//	meta.setContentType("text/plain");
//
//	/* 设置自定义文件元信息 */
//	meta.UserMetaData()["meta"] = "meta-value";
//
//	/* 创建软链接 */
//	CreateSymlinkRequest request(BucketName, ObjectName, meta);
//	request.SetSymlinkTarget(LinkObjectName);
//	auto outcome = client.CreateSymlink(request);
//	if (!outcome.isSuccess())
//	{
//		/* 异常处理 */
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//	}
//
//	/* 释放网络等资源 */
//	ShutdownSdk();
//	return 0;
//}

//获取软链接指向的文件内容
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string LinkName = "EEEWe.txt";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	GetSymlinkRequest request(BucketName, LinkName);
//	auto outcome = client.GetSymlink(request);
//
//	if (!outcome.isSuccess())
//	{
//		/* 异常处理 */
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//	}
//	else
//	{
//		Print_F(outcome.result().SymlinkTarget());
//	}
//
//	/* 释放网络等资源 */
//	ShutdownSdk();
//	return 0;
//}

//设置防盗链
//int32 OSSTest::MainOSS()
//{
//	/* 初始化OSS账号信息 */
//	std::string BucketName = "helloworldd";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	/* 设置防盗链 */
//	SetBucketRefererRequest request(BucketName);
//	request.addReferer("http://www.renzhai.net");
//	request.addReferer("http://www.*.net");
//	request.setAllowEmptyReferer(false);
//
//	auto outcome = client.SetBucketReferer(request);
//	if (!outcome.isSuccess())
//	{
//		/* 异常处理 */
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//
//	ShutdownSdk();
//	return 0;
//}

//获取防盗链
//int32 OSSTest::MainOSS()
//{
//	/* 初始化OSS账号信息 */
//	std::string BucketName = "helloworldd";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//	
//	/* 获取防盗链信息 */
//	GetBucketRefererRequest request(BucketName);
//	auto outcome = client.GetBucketReferer(request);
//	if (!outcome.isSuccess())
//	{
//		/* 异常处理 */
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//	else
//	{
//		Print_F(to_string(outcome.result().AllowEmptyReferer()));
//		Print_F(to_string(outcome.result().RefererList().size()));
//	}
//
//	ShutdownSdk();
//	return 0;
//}

//删除防盗链
//int32 OSSTest::MainOSS()
//{
//	/* 初始化OSS账号信息 */
//	std::string BucketName = "helloworldd";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	/* 清空防盗链,防盗链不能直接清空，需要新建一个允许空referer的规则来覆盖之前的规则 */
//	SetBucketRefererRequest request(BucketName);
//	request.setAllowEmptyReferer(true);
//
//	auto outcome = client.SetBucketReferer(request);
//
//	if (!outcome.isSuccess())
//	{
//		/* 异常处理 */
//		Print_F(outcome.error().Code());
//		Print_F(outcome.error().Message());
//		Print_F(outcome.error().RequestId());
//		ShutdownSdk();
//		return -1;
//	}
//
//	ShutdownSdk();
//	return 0;
//}

//OSS图像处理基础操作
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string ObjectName = "Splash.jpg";
//
//	string FileNametoSave = "F:\\PJ\\TestOSS\\EEEEEEEEEE.jpg";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	/* 缩放 */
//	std::string Process = "image/resize,m_fixed,w_100,h_100";
//	GetObjectRequest request(BucketName, ObjectName);
//	request.setResponseStreamFactory([=]() {return std::make_shared<std::fstream>(FileNametoSave, std::ios_base::out | std::ios_base::in | std::ios_base::trunc | std::ios_base::binary); });
//	request.setProcess(Process);
//	auto outcome = client.GetObject(request);
//
//	/* 裁剪 */
//	Process = "image/crop,w_100,h_100,x_100,y_100,r_1";
//	request.setResponseStreamFactory([=]() {return std::make_shared<std::fstream>(FileNametoSave, std::ios_base::out | std::ios_base::in | std::ios_base::trunc | std::ios_base::binary); });
//	request.setProcess(Process);
//	outcome = client.GetObject(request);
//
//	/* 旋转 */
//	Process = "image/rotate,90";
//	request.setResponseStreamFactory([=]() {return std::make_shared<std::fstream>(FileNametoSave, std::ios_base::out | std::ios_base::in | std::ios_base::trunc | std::ios_base::binary); });
//	request.setProcess(Process);
//	outcome = client.GetObject(request);
//
//	/* 锐化 */
//	Process = "image/sharpen,229";
//	request.setProcess(Process);
//	request.setResponseStreamFactory([=]() {return std::make_shared<std::fstream>(FileNametoSave, std::ios_base::out | std::ios_base::in | std::ios_base::trunc | std::ios_base::binary); });
//	outcome = client.GetObject(request);
//
//	/* 水印 */
//	Process = "image/watermark,text_SGVsbG8g5Zu-54mH5pyN5YqhIQ";
//	request.setProcess(Process);
//	request.setResponseStreamFactory([=]() {return std::make_shared<std::fstream>(FileNametoSave, std::ios_base::out | std::ios_base::in | std::ios_base::trunc | std::ios_base::binary); });
//	outcome = client.GetObject(request);
//
//	/*自定义的样式 */
//	Process = "image/auto-orient,1/bright,55/contrast,-42/sharpen,318/blur,r_1,s_13/watermark,text_aGVsbG8gd29ybGQ,color_ff0000,size_48,rotate_48,shadow_34,g_north,t_51,x_10,y_10";
//	request.setProcess(Process);
//	request.setResponseStreamFactory([=]() {return std::make_shared<std::fstream>(FileNametoSave, std::ios_base::out | std::ios_base::in | std::ios_base::trunc | std::ios_base::binary); });
//	outcome = client.GetObject(request);
//
//	/* 释放网络等资源 */
//	ShutdownSdk();
//	return 0;
//}

//OSS图像处理的自定义样式
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string ObjectName = "Splash.jpg";
//
//	string FileNametoSave = "F:\\PJ\\TestOSS\\EEEEEEEEEE.jpg";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	std::string Process = "style/MyHello";
//	GetObjectRequest request(BucketName, ObjectName);
//	request.setProcess(Process);
//	request.setResponseStreamFactory([=]() {return std::make_shared<std::fstream>(FileNametoSave, std::ios_base::out | std::ios_base::in | std::ios_base::trunc | std::ios_base::binary); });
//	auto outcome = client.GetObject(request);
//
//	/* 释放网络等资源 */
//	ShutdownSdk();
//	return 0;
//}

//级联处理 
//http://<yourBucketName>.<yourEndpoint>/<yourObjectName>?x-oss-process=image/<yourAction1>,<yourParamValue1>/<yourAction2>,<yourParamValue2>/...
//自定义样式 
//http://<yourBucketName>.<yourEndpoint>/<yourObjectName>?x-oss-process=style/<yourStyleName>
//int32 OSSTest::MainOSS()
//{
//	std::string BucketName = "helloworldd";
//	std::string ObjectName = "Splash.jpg";
//
//	string FileNametoSave = "F:\\PJ\\TestOSS\\EEEEEEEEEE.jpg";
//
//	/* 初始化网络等资源 */
//	InitializeSdk();
//
//	ClientConfiguration conf;
//	OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
//
//	std::string Process = "image/resize,m_fixed,w_100,h_100/rotate,90";
//	GetObjectRequest request(BucketName, ObjectName);
//	request.setProcess(Process);
//	request.setResponseStreamFactory([=]() {return std::make_shared<std::fstream>(FileNametoSave, std::ios_base::out | std::ios_base::in | std::ios_base::trunc | std::ios_base::binary); });
//	auto outcome = client.GetObject(request);
//
//	/* 释放网络等资源 */
//	ShutdownSdk();
//	return 0;
//}

void DSFSFSDS(int32 A, int64 V, int64 C, void* D)
{
	Print_F(FString::Printf(TEXT("increment = %i,transfered =%i ,total = %i"), A, V, C));
}

//int32 OSSTest::MainOSS()
//{
//	//std::string AccessKeyId = "LTAIeU8z1jgOd0gq";
//	//std::string AccessKeySecret = "xE77upb6oPWjXYXY6Nr4X6HtBB5ITz";// ;
//	//std::string Endpoint = "oss-cn-beijing.aliyuncs.com";
//	TSharedPtr<FOSS> MyOSS = MakeShareable(new FOSS);
//	auto OutLamada = [=]()
//	{
//		MyOSS->PutObjectByLocal(FString("helloworldd"), FString("F:/SynchronizedUpdate/Engine421/aliyun-oss-cpp-sdk-master.zip"), FString("aliyun-oss-cpp-sdk-master.zip"), DSFSFSDS);
//	};
//
//	MyOSS->InitAccounts("LTAIeU8z1jgOd0gq","xx","oss-cn-beijing.aliyuncs.com");
//
////	ASYNCTASK_Lambda(OutLamada, MyOSS);
//	(new FAutoDeleteAsyncTask<FSimpleAbandonable>(FSimpleDelegate::CreateLambda(OutLamada)))->StartBackgroundTask();
//	return 0;
//}
#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif
#endif // 