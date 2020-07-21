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

#pragma once

#include "CoreMinimal.h"
#include "SimpleHTTPType.generated.h"

UCLASS(BlueprintType)
class SIMPLEHTTP_API USimpleHttpContent :public UObject
{
	GENERATED_BODY()

public:
	TArray<uint8> *Content;

	UFUNCTION(BlueprintPure, Category = "SimpleHTTP|HttpContent")
	TArray<uint8> &GetContent();

	UFUNCTION(BlueprintCallable, Category = "SimpleHTTP|HttpContent")
	bool Save(const FString &LocalPath);
};

enum class EHTTPRequestType
{
	SINGLE,
	MULTPLE,
};

UENUM(BlueprintType)
enum class FSimpleHttpStarte :uint8
{
	NotStarted,//ProcessRequest()
	Processing,
	Failed,
	Failed_ConnectionError,
	Succeeded,
};

USTRUCT(BlueprintType)
struct SIMPLEHTTP_API FSimpleHttpBase
{
	GENERATED_USTRUCT_BODY()

	FSimpleHttpBase()
		:ContentLength(INDEX_NONE)
	{}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "SimpleHttpBase|SimpleHttpBase")
	FString URL;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpleHttpBase|SimpleHttpBase")
	TArray<FString> AllHeaders;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpleHttpBase|SimpleHttpBase")
	FString ContentType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpleHttpBase|SimpleHttpBase")
	int32 ContentLength;
};

USTRUCT(BlueprintType)
struct SIMPLEHTTP_API FSimpleHttpRequest :public FSimpleHttpBase
{
	GENERATED_USTRUCT_BODY()

	FSimpleHttpRequest()
		:Super()
		, ElapsedTime(INDEX_NONE)
	{}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpleHttpBase|SimpleHttpRequest")
	FString Verb;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpleHttpBase|SimpleHttpRequest")
	float ElapsedTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpleHttpBase|SimpleHttpRequest")
	FSimpleHttpStarte Status;
};

USTRUCT(BlueprintType)
struct SIMPLEHTTP_API FSimpleHttpResponse :public FSimpleHttpBase
{
	GENERATED_USTRUCT_BODY()

	FSimpleHttpResponse()
		:Super()
		,ResponseCode(INDEX_NONE)
		,Content(NewObject<USimpleHttpContent>())
	{}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpleHttpBase|SimpleHttpResponse")
	int32 ResponseCode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpleHttpBase|SimpleHttpResponse")
	FString ResponseMessage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpleHttpBase|SimpleHttpResponse")
	USimpleHttpContent *Content;
};

//BP
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FSimpleHttpSingleRequestCompleteDelegate,const FSimpleHttpRequest ,Request,const FSimpleHttpResponse , Response,bool ,bConnectedSuccessfully);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FSimpleHttpSingleRequestProgressDelegate,const FSimpleHttpRequest , Request, int32, BytesSent, int32, BytesReceived);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FSimpleHttpSingleRequestHeaderReceivedDelegate, const FSimpleHttpRequest , Request, const FString , HeaderName, const FString , NewHeaderValue);
DECLARE_DYNAMIC_DELEGATE(FAllRequestCompleteDelegate);

//C++
DECLARE_DELEGATE_ThreeParams(FSimpleSingleCompleteDelegate, const FSimpleHttpRequest &, const FSimpleHttpResponse &, bool);
DECLARE_DELEGATE_ThreeParams(FSimpleSingleRequestProgressDelegate, const FSimpleHttpRequest &, int32, int32);
DECLARE_DELEGATE_ThreeParams(FSimpleSingleRequestHeaderReceivedDelegate, const FSimpleHttpRequest &, const FString &, const FString &);

USTRUCT(BlueprintType)
struct SIMPLEHTTP_API FSimpleHTTPBPResponseDelegate
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpleHTTP|HTTPResponseDelegate")
	FSimpleHttpSingleRequestCompleteDelegate			SimpleHttpRequestCompleteDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpleHTTP|HTTPResponseDelegate")
	FSimpleHttpSingleRequestProgressDelegate			SimpleHttpRequestProgressDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpleHTTP|HTTPResponseDelegate")
	FSimpleHttpSingleRequestHeaderReceivedDelegate		SimpleHttpRequestHeaderReceivedDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SimpleHTTP|HTTPResponseDelegate")
	FAllRequestCompleteDelegate							AllRequestCompleteDelegate;
};

struct SIMPLEHTTP_API FSimpleHTTPResponseDelegate
{
	FSimpleSingleCompleteDelegate						SimpleCompleteDelegate;
	FSimpleSingleRequestProgressDelegate				SimpleSingleRequestProgressDelegate;
	FSimpleSingleRequestHeaderReceivedDelegate			SimpleSingleRequestHeaderReceivedDelegate;
	FSimpleDelegate										AllTasksCompletedDelegate;
};