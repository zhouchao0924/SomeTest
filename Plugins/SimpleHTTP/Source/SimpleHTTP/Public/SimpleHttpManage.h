/*
下面的网址讲解了如何使用本套插件
 DocURL：			https://zhuanlan.zhihu.com/p/82195344

 下面的网址讲解如何开发当前的插件
 MarketplaceURL :   https://www.aboutcg.org/courseDetails/682/introduce

 如果想了解更多关于UE4教程请参考：
 URL :				https://zhuanlan.zhihu.com/p/60117613

 如果想系统了解人宅系列教程以及相关插件迭代更新 可以关注我的博客
 URL :				http://renzhai.net/

 如果想了解我们下一节的课程安排可以 可以在微信公众号搜所 人宅 关注即可 我们会推送很多富有技术的文章

 新浪博客            https://weibo.com/BZRZ/profile?s=6cm7D0  //这个博客从16年到现在 有三四年没怎么用过 以后说不定可以重新启用 先发个连接
 */
 /*
  SimpleHTTP学习文档
  UE4插件名： SimpleHTTP
 版本 1.1
 目录
 前置工作
 上传资源
 下载资源
 删除资源
 SimpleHTTP源码接口
 哈喽大家好，我叫人宅，这节课我们来讲解一下SimpleHTTP使用技巧；

 该插件目前提供了对web服务器进行上传下载和删除的函数接口，分别是：

		bool GetObject(const FString &URL, const FString &SavePaths);
	bool PutObject(const FString &URL, TArray<uint8> &Data);
	bool PutObject(const FString &URL, const FString &LocalPaths);
	bool PutObject(const FString &URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream);
	bool DeleteObject(const FString &URL);
 该插件使用上也是非常方便，无需繁杂的注册环节，即可使用：



 1.前置工作
 在使用该插件需要包含头文件

 #include "SimpleHttpManage.h"
 在你需要包含的模块.Build.cs

 SimpleHTTP
 一切准备就绪 就可以大胆的使用本插件了；

 2.上传资源
 我们拿阿里云OSS服务端做例子

		//域名 wersdfd.oss-cn-beijing.aliyuncs.com
	//格式 https://<域名>/<对象名>/<对象名>/...
	//https://wersdfd.oss-cn-beijing.aliyuncs.com/Hello.zip
	FString URL = "https://wersdfd.oss-cn-beijing.aliyuncs.com/Hello.zip";
	FString LocalPaths = "F:/PJ/TestOSS/SimpleHTTP.zip";
	if (!FSimpleHttpManage::Get()->PutObject(URL, LocalPaths))
	{
		check(0);
	}
 3.下载资源
	FString URL = "https://wersdfd.oss-cn-beijing.aliyuncs.com/Hello.zip";
	FString LocalPaths = "F:/PJ/TestOSS/WWWEEEERRR.zip";
	if (!FSimpleHttpManage::Get()->GetObject(URL, LocalPaths))
	{
		check(0);
	}
 4.删除资源
		FString URL = "https://wersdfd.oss-cn-beijing.aliyuncs.com/Hello.zip";

		if (!FSimpleHttpManage::Get()->DeleteObject(URL))
	{
		check(0);
	}
 5.SimpleHTTP源码接口
 #pragma once

 #include "CoreMinimal.h"
 #include "Interfaces/IHttpRequest.h"
 #include "Interfaces/IHttpResponse.h"

 class SIMPLEHTTP_API FSimpleHttpManage
 {
 public:
	FSimpleHttpManage();
	~FSimpleHttpManage();

	static FSimpleHttpManage *Get();
	static void Destroy();

	bool GetObject(const FString &URL, const FString &SavePaths);
	bool PutObject(const FString &URL, TArray<uint8> &Data);
	bool PutObject(const FString &URL, const FString &LocalPaths);
	bool PutObject(const FString &URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream);
 private:

	void HttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void HttpRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived);
	void HttpRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue);

	void Print(const FString &Msg,float Time = 10.f,FColor Color = FColor::Red);
 private:
	static FSimpleHttpManage	*SimpleHttp;
	FCriticalSection			Mutex;
	FString						TmpSavePaths;
 };


 可以看得出非常简单，而且易上手，好，那本次的分享到此结束，谢谢。

  */
#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
/**
 * 
 */
class SIMPLEHTTP_API FSimpleHttpManage
{
public:
	FSimpleHttpManage();
	~FSimpleHttpManage();

	static FSimpleHttpManage *Get();
	static void Destroy();

	bool GetObject(const FString &URL, const FString &SavePaths);
	bool PutObject(const FString &URL, TArray<uint8> &Data);
	bool PutObject(const FString &URL, const FString &LocalPaths);
	bool PutObject(const FString &URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream);
	bool DeleteObject(const FString &URL);

private:
	void HttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void HttpRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived);
	void HttpRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue);

	void Print(const FString &Msg, float Time = 10.f, FColor Color = FColor::Red);
private:
	static FSimpleHttpManage	*SimpleHttp;
	FString						TmpSavePaths;
};
