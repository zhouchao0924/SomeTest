// Fill out your copyright notice in the Description page of Project Settings.
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
/*
本插件的学习文档 ：

UE4插件名：SimpleOSS
版本：1.1
目录
初始化账户 ( InitAccounts )
初始化配置(InitConf)
创建桶API(CreateBucket)
列举桶(ListBuckets)
获取桶的地域(GetBucketLocation)
删除桶(DeleteBucket)
设置桶的访问权限(SetBucketAcl)
获取桶的读写权限(GetBucketAcl)
设置桶的第三方请求付费者模式(SetBucketRequestPayment)
获取请求者付费模式(GetBucketRequestPayment)
设置防盗链(SetBucketReferer)
获取防盗链(GetBucketReferer)
删除防盗链(DeleteBucketReferer)
上传(PutObject)
异步上传(AsyncPutObject)
断电续传(ResumableUploadObject)
异步断电续传(AsyncResumableUploadObject)
分片上传（UploadPart）
异步分片上传(AsyncUploadPart)
终止分片上传或者删除分片上传(AbortMultipartUpload)
下载OSS资源到内存(GetObjectToMemory)
下载OSS资源到本地磁盘(GetObjectToLocal)
异步下载OSS资源到本地磁盘(AsyncGetObjectToLocal)
断电下载(ResumableDownloadObject)
异步断电下载(AsyncResumableDownloadObject)
在OSS服务器执行复制对象(CopyObject)
异步在OSS服务器执行复制对象(AsyncCopyObject)
删除多个对象(DeleteObjects)
列举上传的所有分片(ListMultipartUploads)
获取对象的元数据(GetObjectMeta)
获取对象的权限(GetObjectAcl)
设置对象权限(SetObjectAcl)
判定桶是否存在(DoesBucketExist)
判定对象是否存在(DoesObjectExist)
递归列举桶下的所有对象(ListObjects)
根据输入的字符前缀来列举符合条件的对象(ListObjectsByPrefix)
获取对象的存储类型(GetObjectStorageType)
设置对象存储类型(SetObjectStorageType)
创建软连接(CreateSymlink)
获取软连接(GetSymlink)
对下载的图片进行缩放(GetImageObjectByResize)
对下载的图片进行裁剪(GetImageObjectByCrop)
对下载的图片进行锐化(GetImageObjectBySharpen)
对下载的图片进行水印(GetImageObjectByWaterMark)
对下载的图片进行旋转(GetImageObjectByRotate)
通过样式模板对下载的图片进行定义(GetImageObjectByStyle)
通过级联对下载的图片进行定义(GetImageObjectByCascade)
获取桶下的对象URL(GeneratePresignedUrl)
通过URL来获取对象资源(GetObjectByUrl)
异步通过URL来获取对象资源(AsyncGetObjectByUrl)
通过URL上传资源（PutObjectByUrl）
异步通过URL上传资源（AsyncPutObjectByUrl）
SimpleOSS源码接口
大家好，这节课我们来系统介绍一下SimpleOSS插件的使用技巧；

OSS是阿里云提供的对象存储系统，我们可以利用阿里云对象存储系统来存储我们的各种数据。

如果不太了解这个OSS到底是啥，可以看这个文章：

人宅：UE4套件-OSS对象存储 服务器入门课程
​
zhuanlan.zhihu.com
图标
OSS是基于 libcurl 的封装，而SimpleOSS是基于OSS SDK的封装。

SimpleOSS的优点在使用上更加简洁，只需要几句代码就可以完成OSS SDK好几行代码的工作；

我们举个栗子：



OSS SDK的上传数据到OSS服务端的代码(我们以C++为例)：

#include <alibabacloud/oss/OssClient.h>
using namespace AlibabaCloud::OSS;

int main(void)
{
	/*初始化OSS账号信息 
std::string AccessKeyId = "yourAccessKeyId";
std::string AccessKeySecret = "yourAccessKeySecret";
std::string Endpoint = "yourEndpoint";
std::string BucketName = "yourBucketName";
std::string ObjectName = "yourObjectName";
/* 初始化网络等资源 
InitializeSdk();

ClientConfiguration conf;
OssClient client(Endpoint, AccessKeyId, AccessKeySecret, conf);
std::shared_ptr<std::iostream> content = std::make_shared<std::stringstream>();
*content << "Thank you for using Aliyun Object Storage Service!";
PutObjectRequest request(BucketName, ObjectName, content);

/* 上传文件 
auto outcome = client.PutObject(request);

if (!outcome.isSuccess()) {
	/* 异常处理 
	std::cout << "PutObject fail" <<
		",code:" << outcome.error().Code() <<
		",message:" << outcome.error().Message() <<
		",requestId:" << outcome.error().RequestId() << std::endl;
	ShutdownSdk();
	return -1;
}

/* 释放网络等资源 
ShutdownSdk();
return 0;
}


我们可以发现，我们需要写非常多代码才能完成上传工作。

我们来看看UE4的插件 SimpleOSS

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);
	SIMPLE_OSS.PutObject("YourBucket", "F:/PJ/TestOSS/1C3C79E04D88B3DABBFBDD9D92379233.txt");
}
我们发现非常简洁，只需要一个接口就完成了 创建客户端->声明请求->定义请求->发送请求

最重要的是我们不需要手动关闭OSS SDK, SimpleOSS会随着引擎的关闭自动关闭，完全不用担心释放问题，只管使用。



好，现在我们来详细介绍一下关于SimpleOSS的使用技巧

首先需要保证已经包含了该模块：

"SimpleOSS"
1.初始化账户(InitAccounts)

在使用该插件的时候需要初始化账户，也就是说需要一个正确的账户来运行本套OSS

SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);
这里你需要三个参数，第一个是秘钥ID;

1.进入阿里云控制台;


2.鼠标放在头像上自然会有以下提示 选择秘钥


3.没有的话可以重新弄一个，按照操作即可，打开后点击以下提示：


4.下面会有提示，第一个就是秘钥ID, 第二个是秘钥，这个不要公开，当你点击显示，那么阿里云会以短信的方式来验证，通过会显示


5.最后一个参数是Endpoint，Endpoint是地域节点，首先需要打开阿里云对象，然后选择一个桶


此时有内外网，如果你购买了多台阿里云产品，希望产品和产品之间相互访问，就可以用内网的。如果希望其他人匿名或者通过其他途径访问可以使用外网。

以上就是我们初始化账户的使用方法；

2.初始化配置(InitConf)

我们提供了可以初始化配置的操作，在配置里面有非常多的方案可以选择我们就以限速为例

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

//限速200KB/s上传
void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	FClientConfiguration ClientConfiguration;
	ClientConfiguration.SendRateLimiter = 200;
	SIMPLE_OSS.InitConf(ClientConfiguration);

	SIMPLE_OSS.PutObject("YourBucket", "F:/PJ/TestOSS/1C3C79E04D88B3DABBFBDD9D92379233.txt");
}
------------------------------------------------------------------------------------------ -
//限速400KB/S下载
void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	FClientConfiguration ClientConfiguration;
	ClientConfiguration.RecvRateLimiter = 400;
	SIMPLE_OSS.InitConf(ClientConfiguration);

	SIMPLE_OSS.GetObjectToLocal("YourBucket", "YourObject", "F:/PJ/TestOSS/YourObjectName.xxx");
}
3.创建桶API(CreateBucket)

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);
	SIMPLE_OSS.CreateBucket("YourBucketName");
}
4.列举桶(ListBuckets)

我们可以列举OSS服务器上面关于我们创建的桶；

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void Print_F(const FString &Msg)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, *Msg);
	}
}

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	TArray<FString> InBucketName;
	SIMPLE_OSS.ListBuckets(InBucketName);
	for (auto &Tmp : InBucketName)
	{
		Print_F(Tmp);
	}
}
}
5.获取桶的地域(GetBucketLocation)

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	auto Str = SIMPLE_OSS.GetBucketLocation("YourBucketName");
	Print_F(Str);
}
6.删除桶(DeleteBucket)

该方法可以删除桶下面的所有对象和碎片，使用的时候要谨慎~

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	SIMPLE_OSS.DeleteBucket("YourBucketName");
}
7.设置桶的访问权限(SetBucketAcl)

桶的权限有私有，公共读，公共读写；

私有(PRIVATE) :的桶只面向自己，匿名用户其他阿里账号用户是不可以访问。

公共读(PUBLICREAD) : 可以匿名访问，也可以允许其他阿里云账户访问

公共读写(PUBLICREADWRITE)：这个要小心，因为匿名用户可能会有读写你桶的权限，也有可能会对你进行攻击；

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	SIMPLE_OSS.SetBucketAcl("YourBucketName", EAcl::PRIVATE);
}
8.获取桶的读写权限(GetBucketAcl)

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	EAcl InACL = SIMPLE_OSS.GetBucketAcl("YourBucketName");
	Print_F(FString::Printf(TEXT("%i"), InACL));
}
9.设置桶的第三方请求付费者模式(SetBucketRequestPayment)

这个可以将下载的费用由第三方来承担

BUCKETOWNER ：OSS产生的费用由创建者提供；

REQUESTER ：OSS产生的费用由请求者提供，这个请求者需要是阿里云用户；

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	SIMPLE_OSS.SetBucketRequestPayment("YourBucketName", ERequestPayerType::REQUESTER);
}
10.获取请求者付费模式(GetBucketRequestPayment)

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	ERequestPayerType RequestPayerType = SIMPLE_OSS.GetBucketRequestPayment("YourBucketName");
	Print_F(FString::Printf(TEXT("%i"), RequestPayerType));
}
11.设置防盗链(SetBucketReferer)

如果有其他非法网站盗用你的资源，用你的资源来做引流，如果没有设置防盗链，就相当于他获取了收益，却没有承担相应的费用，而你却承担了费用但是没有获取相应的流量，所有推出了防盗链系统：

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);


	TArray<FString> URLReferer;
	URLReferer.Add("https://www.renzhai.net/");//当前域名才可以访问，详细内容请参考AboutCG视频教程 UE4服务器入门 OSS套件开发
	SIMPLE_OSS.SetBucketReferer("YourBucketName", URLReferer, true);
}
12.获取防盗链(GetBucketReferer)

当前的API会返回一个FRefererList结构体

bAllowEmptyReferer : 是否允许非空引用

	RefererListNum ：防盗链的数量

	RequestId ：请求ID

#include "SimpleOSSManage.h"

	FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	auto RefererList = SIMPLE_OSS.GetBucketReferer("YourBucketName");

	Print_F(FString::Printf(TEXT("bAllowEmptyReferer = %i"), RefererList.bAllowEmptyReferer));
	Print_F(FString::Printf(TEXT("RefererListNum = %i"), RefererList.RefererListNum));
	Print_F(FString::Printf(TEXT("RequestId = %s"), *RefererList.RequestId));
}
13.删除防盗链(DeleteBucketReferer)

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	SIMPLE_OSS.DeleteBucketReferer("YourBucketName");
}
14.上传(PutObject)

我们在上传数据的时候我可以自定义上传数据的元数据，这个元数据是什么意思，元数据其实就是HTTP的协议头，不过我们可以自己去定义。该API有一个回调，该回调是为了兼容异步而设计的，在同步方案里面不需要回调，因为会阻塞当前的线程。

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	TMap<FString, FString> CustomMeta;
	CustomMeta.Add("Key1", "Value1");
	CustomMeta.Add("Key2", "Value2");
	SIMPLE_OSS.PutObject("YourBucketName", "F:/PJ/TestOSS/1C3C79E04D88B3DABBFBDD9D92379233.txt", "YourNewObjectName", nullptr, CustomMeta);
}
15.异步上传(AsyncPutObject)

在这里我们准备了异步上传，你可以调用此API来实现异步上传, 现在我们来演示一下如何将回调和本API相结合

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

//一个简单的回调函数
void CallFunction_Test(uint64 Increment, int64 Transfered, int64 Total, void* UserData)
{
	//呼叫主线程
	FGraphEventRef FenceHandle = FFunctionGraphTask::CreateAndDispatchWhenReady(
		[=]()
	{
		Print_F(FString::Printf(TEXT("increment = %i,transfered =%i ,total = %i"), Increment, Transfered, Total));
	}, TStatId(), nullptr, ENamedThreads::GameThread);
}

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	//异步上传
	SIMPLE_OSS.AsyncPutObject("YourBucketName", "F:/PJ/TestOSS/1C3C79E04D88B3DABBFBDD9D92379233.txt", "NewObjectName", CallFunction_Test);
}
16.断电续传(ResumableUploadObject)

如果在上传我们的数据的时候发生断电，导致我们上传的数据重新上传，这个时候可以采用断电上传方案来解决这个问题

它会在上次断电处进行上传，而不是重新上传；

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	SIMPLE_OSS.ResumableUploadObject("YourBucketName", "NewObjectName", "F:/PJ/TestOSS/1C3C79E04D88B3DABBFBDD9D92379233.txt");
}
17.异步断电续传(AsyncResumableUploadObject)

和我们异步上传用法是一

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	//异步上传
	//FPaths::ProjectSavedDir() 是缓存的存储点
	SIMPLE_OSS.AsyncResumableUploadObject("YourBucketName", "NewObjectName"， "F:/PJ/TestOSS/1C3C79E04D88B3DABBFBDD9D92379233.txt", FPaths::ProjectSavedDir(), CallFunction_Test);
}
18.分片上传（UploadPart）

什么是分片上传，分片上传是将一个大数据分成一块一块上传，一般用于于大文件上传。

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	FString InUploadId；//可以拿到ID,通过AbortMultipartUpload可以终止上传
	//1024 * 1024 是每个分片为1M大小
		SIMPLE_OSS.UploadPart(InUploadId，"YourBucketName", "YourNewObjectName", "F:/PJ/TestOSS/1C3C79E04D88B3DABBFBDD9D92379233.txt", 1024 * 1024);
}
19.异步分片上传(AsyncUploadPart)

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

//异步分片上传的回调稍微有些不同 详细请参照我们的视频教程 UE4服务器入门 OSS套件开发 
void CallFunction_Test2(FString ID, uint32 Index, uint64 Size, uint64 Total, bool bPart, bool bFinsh)
{
	Print_F(FString::Printf(TEXT("ID = %i,Index =%i,Size =%i ,total = %i,bPart =%i,bFinsh = %i"), *ID, Index, Size, Total, bPart, bFinsh));
}

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	FString InUploadId；//可以拿到ID,通过AbortMultipartUpload可以终止上传
	//1024 * 1024 是每个分片为1M大小
		SIMPLE_OSS.AsyncUploadPart(InUploadId，"YourBucketName", "YourNewObjectName", "F:/PJ/TestOSS/1C3C79E04D88B3DABBFBDD9D92379233.txt", 1024 * 1024，CallFunction_Test2);
}
20.终止分片上传或者删除分片上传(AbortMultipartUpload)

AbortMultipartUpload 接口可以取消分片上传。当我们的OSS服务器有相应的碎片我们也可以启动该函数来清除这些分片碎片


我们


我们来看看如何写这个代码

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	FString InUploadId；//当前的代码是通过UploadPart或者是AsyncUploadPart拿到的ID

		...

		SIMPLE_OSS.AbortMultipartUpload("YourBucketName", "YourObjectName", InUploadId);
}
21.下载OSS资源到内存(GetObjectToMemory)

这里的下载资源我们用字符串来存储这些资源，当前的资源只是下载到内存。

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	FString ContextString;//具体资源存储
	// FRange(0,6) 我们可以进行范围下载，从哪个字节到哪个字节的内容，我们都可以自己指定
	SIMPLE_OSS.GetObjectToMemory("YourBucketName", "YourObjectName", ContextString, FRange(0, 6));
}
22.下载OSS资源到本地磁盘(GetObjectToLocal)

我们可以将数据直接下载到本地磁盘，方便存储移动和管理

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	SIMPLE_OSS.GetObjectToLocal("YourBucketName", "YourObjectName", "F:/PJ/TestOSS/YourObject.txt");
}
23.异步下载OSS资源到本地磁盘(AsyncGetObjectToLocal)

我们还可以通过异步来操作

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	SIMPLE_OSS.AsyncGetObjectToLocal("YourBucketName", "YourObjectName", "F:/PJ/TestOSS/yourObject.txt", FRange(), CallFunction_Test);
}
24.断电下载(ResumableDownloadObject)

这个和上传差不多，是防止断电的。

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	SIMPLE_OSS.ResumableDownloadObject("YourBucketName", "YourObjectName", "F:/PJ/TestOSS/yourObject.txt");
}
25.异步断电下载(AsyncResumableDownloadObject)

在断电的基础上加入异步

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	SIMPLE_OSS.AsyncResumableDownloadObject("YourBucketName", "YourObjectName", "F:/PJ/TestOSS/yourobject.txt", FRange(), CallFunction_Test);
}
26.在OSS服务器执行复制对象(CopyObject)

如果需要将OSS桶里面的数据从这个桶复制到另一个桶，可以使用本操作

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	//从YourBucketName1的资源YourObjectName1拷贝到桶YourBucketName2去并且命名为YourObjectName2
	SIMPLE_OSS.CopyObject("YourBucketName1", "YourObjectName1", "YourBucketName2", "YourObjectName2");
}
27.异步在OSS服务器执行复制对象(AsyncCopyObject)

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	//当前的回调只出现两次 开始和结尾
	SIMPLE_OSS.AsyncCopyObject("YourBucketName1", "YourObjectName1", "YourBucketName2", "YourObjectName2", CallFunction_Test);
}
28.删除多个对象(DeleteObjects)

我们可以通过客户端直接删除OSS服务器对象资源

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	TArray<FString> InObjectName;
	InObjectName.Add("YourObjectName1");
	InObjectName.Add("YourObjectName2");
	InObjectName.Add("YourObjectName3");
	SIMPLE_OSS.DeleteObjects("YourBucketName", InObjectName);
}
29.列举上传的所有分片(ListMultipartUploads)

如果OSS服务器上有因为分片上传而产生的碎片可以通过这个接口来列举；

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	//列举相关桶内存在的碎片
	TArray<FMultipartPartsUploadsInfo> MultipartPartsUploadsInfo;
	SIMPLE_OSS.ListMultipartUploads("YourBucketName", MultipartPartsUploadsInfo);

	for (auto &Tmp : MultipartPartsUploadsInfo)
	{
		Print_F(Tmp.Key);
		Print_F(Tmp.Initiated);
		Print_F(Tmp.UploadId);
	}
}
30.获取对象的元数据(GetObjectMeta)

我们可以通过该接口来获取描述对象的元数据，这些元数据可以自定义。

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	FOSSObjectMeta OSSObjectInfo;
	SIMPLE_OSS.GetObjectMeta("YourBucketName", "YourObjectName", OSSObjectInfo);

	//打印元数据信息
	Print_F(OSSObjectInfo.ContentType);
	Print_F(FString::FromInt(OSSObjectInfo.ContentLength));
	Print_F(OSSObjectInfo.CacheControl);
	Print_F(OSSObjectInfo.ContentDisposition);
	Print_F(OSSObjectInfo.ContentEncoding);
	Print_F(OSSObjectInfo.ContentMd5);
	Print_F(OSSObjectInfo.ObjectType);
	Print_F(OSSObjectInfo.Tag);
	Print_F(OSSObjectInfo.LastModified);
	Print_F(OSSObjectInfo.ExpirationTime);
}
31.获取对象的权限(GetObjectAcl)

这个权限和获取我们桶的权限是差不多的

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	Print_F(FString::FromInt((int32)SIMPLE_OSS.GetObjectAcl("YourBucketName", "YourObjectName")));
}
32.设置对象权限(SetObjectAcl)

DEFAULT ：它的意思是默认继承桶的权限

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	SIMPLE_OSS.SetObjectAcl("YourBucketName", "YourObjectName", EAcl::PRIVATE);
}
33.判定桶是否存在(DoesBucketExist)

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	if (SIMPLE_OSS.DoesBucketExist("YourBucketName"))
	{
		Print_F("Bucket Exist");
	}
}
34.判定对象是否存在(DoesObjectExist)

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	if (SIMPLE_OSS.DoesObjectExist("YourBucketName", "YourObjectName"))
	{
		Print_F("Object Exist");
	}
}
35.递归列举桶下的所有对象(ListObjects)

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	TArray<FOSSObjectInfo> ObjectName;
	SIMPLE_OSS.ListObjects("YourBucketName", ObjectName);

	for (auto &tmp : ObjectName)
	{
		Print_F(tmp.Name);
		Print_F(FString::FromInt(tmp.Size));
		Print_F(tmp.LastmodifyTime);
	}
}
36.根据输入的字符前缀来列举符合条件的对象(ListObjectsByPrefix)

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	TArray<FOSSObjectInfo> ObjectName;
	SIMPLE_OSS.ListObjectsByPrefix("YourBucketName", "A");//递归列举凡是以A开头的对象

	for (auto &tmp : ObjectName)
	{
		Print_F(tmp.Name);
		Print_F(FString::FromInt(tmp.Size));
		Print_F(tmp.LastmodifyTime);
	}
}
37.获取对象的存储类型(GetObjectStorageType)

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	Print_F(FString::FromInt((int32)SIMPLE_OSS.GetObjectStorageType("YourBucketName", "YourObjectName")));
}
38.设置对象存储类型(SetObjectStorageType)

这里牵扯到桶归档类型的转换。

如果当前类型是归档类型，那这边就需要解冻，解冻时间是1分钟，也就是说在设置存储类型的时候如果遇到归档转其他类型，

那么就需要等待一分钟，此时线程会被 阻塞一分钟。

类型的设置建议不要放在主线程：

STANDARD : 标准存储类型，适合经常访问的数据。

	IA：低频存储类型，适合不经常访问的数据，比如半年访问一次的。

	ARCHIVE：归档存储类型，适合一年都不怎么访问的数据，如果访问需要解冻，解冻时间为1分钟。

#include "SimpleOSSManage.h"

	FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	SIMPLE_OSS.SetObjectStorageType("YourBucketName", "YourObjectName", EOSSStorageType::STANDARD);
}
39.创建软连接(CreateSymlink)

OSS的软连接就相当于Windows的快捷方式，我们可以通过快捷方式直接访问里面的数据

我们来看看如何创建它

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);
	//我们可以通过将原有的对象创建软连接，给它取名为YourNewObjectName
	SIMPLE_OSS.CreateSymlink("YourBucketName", "YourNewObjectName", "YourObjectName");
}
40.获取软连接(GetSymlink)

为你可以通过软连接的名字直接找到软连接连接的真正数据 名称

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	//YourLinkObjectName 它是快捷方式的名称
	Print_F(SIMPLE_OSS.GetSymlink("YourBucketName", "YourLinkObjectName"));
}
41.对下载的图片进行缩放(GetImageObjectByResize)

图像处理接口只对图像格式管用。

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);
	//FVector2D(100.f, 100.f) 将原来的图像缩放到100*100的大小
	SIMPLE_OSS.GetImageObjectByResize(FVector2D(100.f, 100.f), "YourBucketName", "YourImageObjectName", "F:/PJ/TestOSS/Image1.jpg");
}
42.对下载的图片进行裁剪(GetImageObjectByCrop)

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);
	//FVector4(100,100,100,100),1, ：裁剪的图片长100，宽100，x坐标100，y坐标100，比率为1
	SIMPLE_OSS.GetImageObjectByCrop(FVector4(100, 100, 100, 100), 1, "YourBucketName", "YourImageObjectName", "F:/PJ/TestOSS/Image2.jpg");
}
43.对下载的图片进行锐化(GetImageObjectBySharpen)

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);
	//200 锐化值
	SIMPLE_OSS.GetImageObjectBySharpen(200, "YourBucketName", "YourImageObjectName", "F:/PJ/TestOSS/Image3.jpg");
}
44.对下载的图片进行水印(GetImageObjectByWaterMark)

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	//mage_V2EucG5nP3gtb3NzLXByb2Nlc3M9aW1hZ2UvcmVzaXplLFBfMzgvYnJpZ2h0LDEx,g_se,t_76,x_10,y_10//这句是我们水印值，可以在样式设置位置去查看，或者定义。
	SIMPLE_OSS.GetImageObjectByWaterMark("image_V2EucG5nP3gtb3NzLXByb2Nlc3M9aW1hZ2UvcmVzaXplLFBfMzgvYnJpZ2h0LDEx,g_se,t_76,x_10,y_10", "YourBucketName", "YourImageObjectName", "F:/PJ/TestOSS/Image4.jpg");
}
45.对下载的图片进行旋转(GetImageObjectByRotate)

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	//对图片旋转90 注意这里是整数，不支持folat
	SIMPLE_OSS.GetImageObjectByRotate(90, "YourBucketName", "YourImageObjectName", "F:/PJ/TestOSS/Image5.jpg");
}
46.通过样式模板对下载的图片进行定义(GetImageObjectByStyle)


如上图，可以通过样式来定义自己喜欢的图像样式，并且设置为模板

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	SIMPLE_OSS.GetImageObjectByStyle("StyleName", "YourBucketName", "YourImageObjectName", "F:/PJ/TestOSS/Image6.jpg");
}
47.通过级联对下载的图片进行定义(GetImageObjectByCascade)

我们可以通过多个命令来定义下载的图片应该张什么样子；

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	TArray<FString> CascadeCommand;
	CascadeCommand.Add("sharpen,399");//锐化 399
	CascadeCommand.Add("rotate,20");//对图片旋转90
	CascadeCommand.Add("blur,r_5,s_4");//模糊值 模糊半径5 水平模糊 4
	CascadeCommand.Add("quality,q_50");//图片质量 90
	SIMPLE_OSS.GetImageObjectByCascade(CascadeCommand, "YourBucketName", "YourImageObjectName", "F:/PJ/TestOSS/Image6.jpg");
}
48.获取桶下的对象URL(GeneratePresignedUrl)

OSS桶的权限必须是只读

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	Print_F(SIMPLE_OSS.GeneratePresignedUrl("YourBucketName", "YourObjectName"));
}
49.通过URL来获取对象资源(GetObjectByUrl)

OSS桶的权限必须是只读

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	SIMPLE_OSS.GetObjectByUrl("URL", "YourLocalPaths");
}
50.异步通过URL来获取对象资源(AsyncGetObjectByUrl)

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	SIMPLE_OSS.AsyncGetObjectByUrl("URL", "YourLocalPaths", CallFunction_Test);
}
51.通过URL上传资源（PutObjectByUrl）

OSS桶的权限必须是读写

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	SIMPLE_OSS.PutObjectByUrl("URL", "YourLocalPaths", CallFunction_Test);
}
52.异步通过URL上传资源（AsyncPutObjectByUrl）

OSS桶的权限必须是读写

#include "SimpleOSSManage.h"

FString AccessKeyId = "yourAccessKeyId";
FString AccessKeySecret = "yourAccessKeySecret";
FString Endpoint = "yourEndpoint";

void main()
{
	SIMPLE_OSS.InitAccounts(AccessKeyId, AccessKeySecret, Endpoint);

	SIMPLE_OSS.AsyncPutObjectByUrl("URL", "YourLocalPaths", CallFunction_Test);
}
以上是我们的接口，该接口会根据需求以后还会继续扩展。
*/

#pragma once

#include "CoreMinimal.h"
#include <string>
#include "SimpleOSSType.h"
#include "Misc/Paths.h"

#if PLATFORM_WINDOWS
#include "Windows/WindowsCriticalSection.h"
#else 
#if PLATFORM_LINUX
#include "Linux/LinuxCriticalSection.h"
#else
#if PLATFORM_MAC
#include "Mac/MacCriticalSection.h"
#endif
#endif
#endif

using namespace std;

class SIMPLEOSS_API FSimpleOSSManage
{
public:
	FSimpleOSSManage();
	~FSimpleOSSManage();

	static FSimpleOSSManage *Get();
	static void Destroy();

public:

	//封装着最基础API系统
	struct SIMPLEOSS_API FOSS
	{
		friend class FSimpleOSSManage;

		//初始化
		//////////////////////////////////////////////////////////////////////////
#pragma region InitOSS
		/**
		* 初始化账户
		* @InAccessKeyId		访问的密钥ID
		* @InAccessKeySecret	密钥
		* @InEndpoint			地域节点
		*/
		void InitAccounts(const FString &InAccessKeyId, const FString &InAccessKeySecret, const FString &InEndpoint);
	
		/**
		* 初始化配置
		* InConf  初始化配置
		*/
		void InitConf(const FClientConfiguration &InConf);
#pragma endregion InitOSS
		
		/**
		* 获取对象的URL
		* @InBucketName 桶名字
		* @InObjectName	对象名
		*/
		FString GeneratePresignedUrl(const FString &InBucketName, const FString &InObjectName);
		
		/**
		* 通过URL上传数据 OSS桶的权限必须是读写
		* @ InURL URL
		* @ InFilePaths	本地路径
		* @ InProgressCallback 回调
		*/
		bool PutObjectByUrl(const FString& InURL, const FString& InFilePaths, ProgressCallback InProgressCallback = nullptr) const;
		
		/**
		* 通过URL下载数据 OSS桶的权限必须是只读
		* @ InURL URL
		* @ InFilePaths	本地路径
		* @ InProgressCallback 回调
		*/
		bool GetObjectByUrl(const FString& InURL, const FString& InFilePaths, ProgressCallback InProgressCallback = nullptr) const;
		
		/**
		* 通过URL下载数据 OSS桶的权限必须是读写
		* @ InURL URL
		* @ InFilePaths	本地路径
		* @ InProgressCallback 回调
		*/
		void AsyncPutObjectByUrl(const FString& InURL, const FString& InFilePaths, ProgressCallback InProgressCallback = nullptr) const;
	
		/**
		* 通过URL下载数据 OSS桶的权限必须是读写
		* @ InURL URL
		* @ InFilePaths	本地路径
		* @ InProgressCallback 回调
		*/
		void AsyncGetObjectByUrl(const FString& InURL, const FString& InFilePaths, ProgressCallback InProgressCallback = nullptr) const;
		
		//桶
		//////////////////////////////////////////////////////////////////////////
#pragma region Bucket
		/**
		* 该桶是否存在
		* @InBucketName 桶名字
		*/
		bool DoesBucketExist(const FString &InBucketName);

		/**
		* 创建桶
		* @InBucketName		定义的桶名字
		* @OSSStorageType	存储类型
		* @Acl				权限
		*/
		bool CreateBucket(const FString &InBucketName,EOSSStorageType OSSStorageType = EOSSStorageType::IA,EAcl Acl = EAcl::PUBLICREAD);

		/**
		* 列举桶
		* @InBucketName 可以获取桶名字集合
		*/
		bool ListBuckets(TArray<FString> &InBucketName);

		/**
		* 获取桶的地域
		* @InBucketName 桶名字
		*/
		FString GetBucketLocation(const FString &InBucketName);

		/**
		* 删除桶
		* @InBucketName 桶名字
		*/
		bool DeleteBucket(const FString &InBucketName);

		/**
		* 设置桶的权限
		* @InBucketName 桶名字
		* @BucketAcl    桶权限
		*/
		bool SetBucketAcl(const FString &InBucketName, EAcl BucketAcl);

		/**
		* 获取桶的权限
		* @InBucketName 桶名字
		*/
		EAcl GetBucketAcl(const FString InBucketName);
		
		/**
		* 设置桶的请求者付费模式
		* @InBucketName 桶名字
		* @RequestPayer 请求者模式
		*/
		bool SetBucketRequestPayment(const FString &InBucketName, ERequestPayerType InRequestPayer);

		/**
		* 获取桶的请求者付费模式
		* @InBucketName 桶名字
		*/
		ERequestPayerType GetBucketRequestPayment(const FString &InBucketName);

		/**
		* 设置防盗链
		* @InBucketName			桶名字
		* @URLReferer			防盗链列表
		* @bAllowEmptyRefere	是否允许空的链
		*/
		bool SetBucketReferer(const FString &InBucketName, const TArray<FString> &URLReferer, bool bAllowEmptyRefere = false);

		/**
		* 获取防盗链
		* @InBucketName 桶名字
		*/
		FRefererList GetBucketReferer(const FString &InBucketName);

		/**
		* 删除防盗链
		* @InBucketName 桶名字
		* @URLReferer	防盗链列表
		* @bAllowEmptyRefere 是否允许空的链
		*/
		bool DeleteBucketReferer(const FString &InBucketName, bool bAllowEmptyRefere = true);
#pragma endregion Bucket

		//对象操作
		//////////////////////////////////////////////////////////////////////////
#pragma region Object
		/**
		* 获取元数据信息
		* @InBucketName 桶名字
		* @InObjectName	对象名
		* @OSSObjectInfo 对象元数据信息
		*/
		bool GetObjectMeta(const FString &InBucketName, const FString &InObjectName, FOSSObjectMeta &OSSObjectInfo);

		/**
		* 获取对象的读写权限
		* @InBucketName 桶名字
		* @InObjectName	对象名
		*/
		EAcl GetObjectAcl(const FString &InBucketName, const FString &InObjectName);

		/**
		* 设置对象的读写权限
		* @InBucketName 桶名字
		* @InObjectName	对象名
		* @InAcl 读写权限
		*/
		bool SetObjectAcl(const FString &InBucketName, const FString &InObjectName, EAcl InAcl);

		/**
		* 是否存在对象
		* @InBucketName 桶名字
		* @InObjectName	对象名
		*/
		bool DoesObjectExist(const FString &InBucketName, const FString &InObjectName);

		/**
		* 列举对象 属于递归式的
		* @InBucketName 桶名字
		* @ObjectName	对象信息
		* @MaxNumber	最大列举数量
		*/
		bool ListObjects(const FString &InBucketName, TArray<FOSSObjectInfo> &ObjectName, int32 MaxNumber = 100);

		/**
		* 通过前缀来列举对象
		* @InBucketName 桶名字
		* @InKeyPrefix	列举的前缀 比如文件夹名
		* @ObjectName	对象信息
		*/
		bool ListObjectsByPrefix(const FString &InBucketName, const FString &InKeyPrefix, TArray<FOSSObjectInfo> &ObjectName);
		
		/**
		* 设置对象的存储类型
		* @InBucketName 桶名字
		* @InObjectName	对象名
		* @OSSStorageType	存储类型
		*/
		bool SetObjectStorageType(const FString &InBucketName, const FString &InObjectName, EOSSStorageType OSSStorageType);
		
		/**
		* 获取对象的存储类型
		* @InBucketName 桶名字
		* @InObjectName	对象名
		*/
		EOSSStorageType GetObjectStorageType(const FString &InBucketName, const FString &InObjectName);

		/**
		* 创建软连接 快捷方式
		* @InBucketName 桶名字
		* @InObjectName	快捷方式的名字
		* @InLinkObjectName	要连接哪里数据
		*/
		bool CreateSymlink(const FString &InBucketName, const FString &InObjectName, const FString &InLinkObjectName);
		
		/**
		* 获取软链接指向的文件内容
		* @InBucketName 桶名字
		* @InObjectName	对象名
		*/
		FString GetSymlink(const FString &InBucketName, const FString &InLinkName);
		
#pragma endregion Object

		//资源下载
		//////////////////////////////////////////////////////////////////////////
#pragma region GetObjects
		/**
		* 下载到客户端内存
		* @InBucketName			桶名字
		* @InObjectName			对象名
		* @ContextString		具体数据
		* @Range				下载范围
		* @InProgressCallback	回调
		*/
		bool GetObjectToMemory(const FString &InBucketName, const FString &InObjectName, FString &ContextString, SimpleOSS::OSS::FRange Range = FRange(), ProgressCallback InProgressCallback = nullptr);

		/**
		* 下载到本地
		* @InBucketName			桶名字
		* @InObjectName			对象名
		* @InLocalPaths			本地路径
		* @Range				下载范围
		* @InProgressCallback	回调
		*/
		bool GetObjectToLocal(const FString &InBucketName, const FString &InObjectName, const FString &InLocalPaths, SimpleOSS::OSS::FRange Range = FRange(), ProgressCallback InProgressCallback = nullptr);

		/**
		* 断点续传下载
		* @InBucketName			桶名字
		* @InObjectName			对象名
		* @InDownloadFilePath	本地路径
		* @Range				下载范围
		* @InProgressCallback	回调
		* @InCheckpointFilePath 缓存存储点
		*/
		bool ResumableDownloadObject(const FString &InBucketName, const FString &InObjectName, const FString &InDownloadFilePath,SimpleOSS::OSS::FRange Range = FRange(),ProgressCallback InProgressCallback = nullptr,const FString &InCheckpointFilePath = FPaths::ProjectSavedDir());
		
#pragma endregion GetObjects

		/**
		* 复制对象
		* @ InSourceBucketName			原桶的名字
		* @ InSourceObjectName			原桶内的对象
		* @ InCopyBucketName			从原桶复制到对象的桶是的名字
		* @ InCopyObjectName			从原桶的对象复制到相应桶下的对象名
		* @ InProgressCallback			回调
		*/
		bool CopyObject(const FString &InSourceBucketName, const FString &InSourceObjectName, const FString &InCopyBucketName, const FString &InCopyObjectName, ProgressCallback InProgressCallback = nullptr);
		
		/**
		* 删除单个资源
		* @ InBucketName			桶名字
		* @ InObjectName			对象名字
		* @ InCopyBucketName		回调
		*/
		bool DeleteObject(const FString &InBucketName, const FString &InObjectName, ProgressCallback InProgressCallback = nullptr);

		/**
		* 删除多个资源
		* @ InBucketName			桶名字
		* @ InObjectName			多个对象
		* @ InCopyBucketName		回调
		*/
		bool DeleteObjects(const FString &InBucketName, const TArray<FString> &InObjectName, ProgressCallback InProgressCallback = nullptr);
		
		/**
		* 列举上传分片
		* @ MultipartPartsInfo		列举分片的信息
		* @ InBucketName			桶名字
		* @ InObjectName			多个对象 MaxParts
		* @ MaxParts				列举的最大分片数
		*/
		bool ListMultipartUploads(const FString &InBucketName, TArray<FMultipartPartsUploadsInfo> &MultipartPartsInfo, int32 MaxParts = 1000);
		
#pragma region Async
	/**
		* 异步上传对象
		* @ InBucketName		桶名字
		* @ InLocalPaths		本地路径
		* @ InObjectName		对象名
		* @ InProgressCallback	回调  用于异步，同步不需要回调，该接口是为了兼容异步请求
		* @ CustomMeta			自定义的元数据
		* @ OSSObjectMeta		OSS默认的元数据
		* @ DebugMsg			为了继续调试的打印信息
		*/
		void AsyncPutObject(const FString &InBucketName, const FString &InLocalPaths, const FString &InObjectName = FString(), ProgressCallback InProgressCallback = nullptr, const TMap<FString, FString> &CustomMeta = TMap<FString, FString>(), const FOSSObjectMeta &OSSObjectMeta = FOSSObjectMeta());

		/**
		* 异步版本断点续传 防止断点，下次上传会接上
		* @ InBucketName			桶名字
		* @ InObjectName			对象名
		* @ InUploadFilePath		本地路径
		* @ InCheckpointFilePath	断点缓存存储路径
		* @ InProgressCallback		回调  用于异步，同步不需要回调，该接口是为了兼容异步请求
		* @ CustomMeta				自定义的元数据
		* @ OSSObjectMeta			OSS默认的元数据
		* @ DebugMsg				为了继续调试的打印信息
		*/
		void AsyncResumableUploadObject(const FString &InBucketName, const FString &InObjectName, const FString &InUploadFilePath, const FString &InCheckpointFilePath = FPaths::ProjectSavedDir(), ProgressCallback InProgressCallback = nullptr, const TMap<FString, FString> &CustomMeta = TMap<FString, FString>(), const FOSSObjectMeta &OSSObjectMeta = FOSSObjectMeta(), const FString &DebugMsg = TEXT("Thank you for using Oss Storage Service!"));
		
		/**
		* 异步分片上传 对于大型文件的上传方案
		* @ InUploadId				上传ID
		* @ InBucketName			桶名字
		* @ InObjectName			对象名
		* @ InLocalPaths			本地路径
		* @ PartSize				分片的大小尺寸
		* @ InProgressCallback		回调  用于异步，同步不需要回调，该接口是为了兼容异步请求
		* @ CustomMeta				自定义的元数据
		* @ OSSObjectMeta			OSS默认的元数据
		* @ DebugMsg				为了继续调试的打印信息
		*/
		void AsyncUploadPart(FString &InUploadId, const FString &InBucketName, const FString &InObjectName, const FString &InLocalPaths, int32 PartSize = 1024 * 1024 * 10, CallbackUploadPart InCallbackUploadPart = nullptr, const TMap<FString, FString> &CustomMeta = TMap<FString, FString>(), const FOSSObjectMeta &OSSObjectMeta = FOSSObjectMeta());
		
		/**
		* 异步下载到本地
		* @InBucketName			桶名字
		* @InObjectName			对象名
		* @InLocalPaths			本地路径
		* @Range				下载范围
		* @InProgressCallback	回调
		*/
		void AsyncGetObjectToLocal(const FString &InBucketName, const FString &InObjectName, const FString &InLocalPaths, SimpleOSS::OSS::FRange Range = FRange(), ProgressCallback InProgressCallback = nullptr);

		/**
		* 异步断点续传下载
		* @InBucketName			桶名字
		* @InObjectName			对象名
		* @InDownloadFilePath	本地路径
		* @Range				下载范围
		* @InProgressCallback	回调
		* @InCheckpointFilePath 缓存存储点
		*/
		void AsyncResumableDownloadObject(const FString &InBucketName, const FString &InObjectName, const FString &InDownloadFilePath, SimpleOSS::OSS::FRange Range = FRange(), ProgressCallback InProgressCallback = nullptr, const FString &InCheckpointFilePath = FPaths::ProjectSavedDir());
		
		/**
		* 异步复制对象
		* @ InSourceBucketName			原桶的名字
		* @ InSourceObjectName			原桶内的对象
		* @ InCopyBucketName			从原桶复制到对象的桶是的名字
		* @ InCopyObjectName			从原桶的对象复制到相应桶下的对象名
		* @ InProgressCallback			回调
		*/
		void AsyncCopyObject(const FString &InSourceBucketName, const FString &InSourceObjectName, const FString &InCopyBucketName, const FString &InCopyObjectName, ProgressCallback InProgressCallback = nullptr);

		/**
		* 异步删除多个资源
		* @ InBucketName			桶名字
		* @ InObjectName			多个对象
		* @ InCopyBucketName		回调
		*/
		void AsyncDeleteObjects(const FString &InBucketName, const TArray<FString> &InObjectName, ProgressCallback InProgressCallback = nullptr);
#pragma endregion Async
		//资源上传
		//////////////////////////////////////////////////////////////////////////
#pragma region PutObject

		/**
		* 从本地上传资源
		* @ InBucketName		桶名字
		* @ InLocalPaths		本地路径
		* @ InObjectName		对象名
		* @ InProgressCallback	回调  用于异步，同步不需要回调，该接口是为了兼容异步请求
		* @ CustomMeta			自定义的元数据
		* @ OSSObjectMeta		OSS默认的元数据
		* @ DebugMsg			为了继续调试的打印信息
		*/
		bool PutObject(const FString &InBucketName, const FString &InLocalPaths, const FString &InObjectName = FString(), ProgressCallback InProgressCallback = nullptr, const TMap<FString, FString> &CustomMeta = TMap<FString, FString>(), const FOSSObjectMeta &OSSObjectMeta = FOSSObjectMeta());

		/**
		* 断点续传 防止断点，下次上传会接上
		* @ InBucketName			桶名字
		* @ InObjectName			对象名
		* @ InUploadFilePath		本地路径
		* @ InCheckpointFilePath	断点缓存存储路径
		* @ InProgressCallback		回调  用于异步，同步不需要回调，该接口是为了兼容异步请求
		* @ CustomMeta				自定义的元数据
		* @ OSSObjectMeta			OSS默认的元数据
		* @ DebugMsg				为了继续调试的打印信息
		*/
		bool ResumableUploadObject(const FString &InBucketName, const FString &InObjectName, const FString &InUploadFilePath, const FString &InCheckpointFilePath = FPaths::ProjectSavedDir(), ProgressCallback InProgressCallback = nullptr, const TMap<FString, FString> &CustomMeta = TMap<FString, FString>(), const FOSSObjectMeta &OSSObjectMeta = FOSSObjectMeta(), const FString &DebugMsg = TEXT("Thank you for using Oss Storage Service!"));

		/**
		* 分片上传 对于大型文件的上传方案
		* @ InUploadId				上传ID
		* @ InBucketName			桶名字
		* @ InObjectName			对象名
		* @ InLocalPaths			本地路径
		* @ PartSize				分片的大小尺寸
		* @ InProgressCallback		回调  用于异步，同步不需要回调，该接口是为了兼容异步请求
		* @ CustomMeta				自定义的元数据
		* @ OSSObjectMeta			OSS默认的元数据
		* @ DebugMsg				为了继续调试的打印信息
		*/
		bool UploadPart(FString &InUploadId, const FString &InBucketName, const FString &InObjectName, const FString &InLocalPaths, int32 PartSize = 1024 * 1024 * 10, CallbackUploadPart InCallbackUploadPart = nullptr, const TMap<FString, FString> &CustomMeta = TMap<FString, FString>(), const FOSSObjectMeta &OSSObjectMeta = FOSSObjectMeta());

		/**
		* 取消分片上传
		* @ InBucketName			桶名字
		* @ InObjectName			对象名
		* @ InUploadId				上传ID
		*/
		bool AbortMultipartUpload(const FString &InBucketName, const FString &InObjectName, const FString &InUploadId);
#pragma endregion PutObject
		//图像处理
		//////////////////////////////////////////////////////////////////////////
#pragma region ImageProcessing

		/**
		* 缩放下载的图片
		* @ Size				缩放的图片尺寸
		* @ InBucketName		桶名字
		* @ URLReferer			防盗链列表
		* @ bAllowEmptyRefere	是否允许空的链
		*/
		bool GetImageObjectByResize(const FVector2D &Size, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);
		
		/**
		* 裁剪下载的图片
		* @ Size				裁剪的图片信息
		* @ InBucketName		桶名字
		* @ URLReferer			防盗链列表
		* @ bAllowEmptyRefere	是否允许空的链
		*/
		bool GetImageObjectByCrop(const FVector4 &Size, int32 Ratio, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);

		/**
		* 锐化下载图片
		* @ Sharpen				锐化图片的系数
		* @ InBucketName		桶名字
		* @ URLReferer			防盗链列表
		* @ bAllowEmptyRefere	是否允许空的链
		*/
		bool GetImageObjectBySharpen(const int32 Sharpen, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);

		/**
		* 水印下载图片
		* @ WaterMarkObjectName	水印图片的名字
		* @ InBucketName		桶名字
		* @ URLReferer			防盗链列表
		* @ bAllowEmptyRefere	是否允许空的链
		*/
		bool GetImageObjectByWaterMark(const FString WaterMarkObjectName, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);

		/**
		* 旋转下载的图片
		* @ Rotate				图片旋转的欧拉角
		* @ InBucketName		桶名字
		* @ URLReferer			防盗链列表
		* @ bAllowEmptyRefere	是否允许空的链
		*/
		bool GetImageObjectByRotate(const int32 Rotate, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);
		
		/**
		* 使用OSS模板样式
		* @ OSSStyleName		OSS样式名称
		* @ InBucketName		桶名字
		* @ URLReferer			防盗链列表
		* @ bAllowEmptyRefere	是否允许空的链
		*/
		bool GetImageObjectByStyle(const FString OSSStyleName, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);
		
		/**
		* 级联处理 
		* @ CascadeCommand		多层命令
		* @ InBucketName		桶名字
		* @ URLReferer			防盗链列表
		* @ bAllowEmptyRefere	是否允许空的链
		*/
		bool GetImageObjectByCascade(const TArray<FString> &CascadeCommand, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);
private:
		/**
		* 对下载的图像进行处理的通用接口
		* @InBucketName 桶名字
		* @InObjectName	对象名
		* @InFileNametoSave 存储路径，本地磁盘
		* @Command 对图像进行怎样的处理
		*/
		bool GetImageObject(const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave, const FString &Command);
	
#pragma endregion ImageProcessing

	private:
		string AccessKeyId;
		string AccessKeySecret;
		string Endpoint;
		FCriticalSection Mutex;			
		FClientConfiguration Conf;
	};

	FORCEINLINE FOSS &GetOSS() { return OSS; }
private:
	static FSimpleOSSManage	*SimpleOSS;

	FOSS OSS;
};

#define SIMPLE_OSS FSimpleOSSManage::Get()->GetOSS()