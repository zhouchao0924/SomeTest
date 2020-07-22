// Copyright (C) RenZhai.2019.All Rights Reserved.
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

#include "SimpleOSSLog.h"

#pragma once 

#define SIMPLEOSS_MACRO_ERROR(Outcome) \
AuxiliaryTools.Print(Outcome##.error().Code()); \
AuxiliaryTools.Print(Outcome##.error().Message()); \
AuxiliaryTools.Print(Outcome##.error().RequestId()); 

#define SIMPLEOSS_MACRO_TYPE1(Request) \
SIMPLEOSS_MACRO_META(Request) \
SIMPLEOSS_MACRO_PROGRESSCALLBACK(Request)

#define SIMPLEOSS_MACRO_META(Request) \
/* 设置HTTP header */ \
ObjectMetaData meta; \
{ \
	if (!OSSObjectMeta.ContentType.IsEmpty())\
	{\
		meta.setContentType(TCHAR_TO_UTF8(*OSSObjectMeta.ContentType)); \
	}\
	if (!OSSObjectMeta.CacheControl.IsEmpty()) \
	{\
		meta.setCacheControl(TCHAR_TO_UTF8(*OSSObjectMeta.CacheControl));\
	}\
	if (!OSSObjectMeta.ContentDisposition.IsEmpty())\
	{\
		meta.setCacheControl(TCHAR_TO_UTF8(*OSSObjectMeta.ContentDisposition));\
	}\
	if (!OSSObjectMeta.ContentEncoding.IsEmpty())\
	{\
		meta.setCacheControl(TCHAR_TO_UTF8(*OSSObjectMeta.ContentEncoding));\
	}\
	if (!OSSObjectMeta.ContentMd5.IsEmpty())\
	{\
		meta.setCacheControl(TCHAR_TO_UTF8(*OSSObjectMeta.ContentMd5));\
	}\
}\
/*自定义元数据*/ \
for (auto &Tmp : CustomMeta)\
{\
	meta.UserMetaData()[TCHAR_TO_UTF8(*Tmp.Key)] = TCHAR_TO_UTF8(*Tmp.Value);\
}\
Request.MetaData() = meta; 

#define SIMPLEOSS_MACRO_PROGRESSCALLBACK(Request) \
TransferProgress progressCallback = { InProgressCallback , nullptr }; \
Request.setTransferProgress(progressCallback);

#define CREATE_CONFIGURATION_AND_STORAGE \
/* 用于内存管理的内存池（pool），等价于apr_pool_t。其实现代码在apr库中。*/ \
aos_pool_t *pool;  \
/* 重新创建一个内存池，第二个参数是NULL，表示没有继承其它内存池。*/ \
aos_pool_create(&pool, NULL); \
/* 创建并初始化options，该参数包括endpoint、access_key_id、acces_key_secret、is_cname、 curl等全局配置信息。*/ \
oss_request_options_t *oss_client_options; \
/* 在内存池中分配内存给options。*/ \
oss_client_options = oss_request_options_create(pool); \
/* 初始化Client的选项oss_client_options。*/ \
init_options(oss_client_options);

#define STRING_TO_AOS_STRING_T(ObjectName,InObjectName) \
aos_string_t ObjectName;\
aos_str_null(&ObjectName); \
ANSICHAR* ObjectName##_name = TCHAR_TO_UTF8(*InObjectName); \
/*Assign data of type char * to the storage space of type AOS ﹣ string ﹣ t */ \
aos_str_set(&ObjectName, ObjectName##_name);

#define UTF8_STRING_TO_AOS_STRING_T(ObjectName,InObjectName) \
aos_string_t ObjectName;\
ANSICHAR* InObjectName##_Char = TCHAR_TO_UTF8(*InObjectName);\
/*Utf8 code the multi byte Chinese name.*/ \
ANSICHAR InObjectName##buf[1024];\
ANSICHAR *Tmp_##ObjectName = multibyte_to_utf8(InObjectName##_Char, InObjectName##buf, 1024);\
/*Assign data of type char * to the storage space of type AOS ﹣ string ﹣ t */  \
aos_str_set(&ObjectName, Tmp_##ObjectName);

#define STATEMENT_VISITSUCCESS \
bool bVisitSuccess = false;

#define AOS_POOL_DESTROY_IT(pool) \
aos_pool_destroy(pool);

#define OSS_SDK_RESPONSE_MSG \
if (bVisitSuccess) \
{\
	OSS_SUCCESS_MSG("ErrorCode[%i] ErrorCodeChar[%s] ReqID[%s] ErrorMsg[%s]  ", resp_status->code, resp_status->error_code, resp_status->error_msg, resp_status->req_id)\
}\
else \
{\
	OSS_ERROR_MSG("ErrorCode[%i] ErrorCodeChar[%s] ReqID[%s] ErrorMsg[%s]  ", resp_status->code, resp_status->error_code, resp_status->error_msg, resp_status->req_id)\
}

#define CHECK_OSS_RESULT(Code_True,Code_False) \
STATEMENT_VISITSUCCESS \
if (aos_status_is_ok(resp_status)){bVisitSuccess = true; Code_True }\
else {bVisitSuccess = false; Code_False} \
AOS_POOL_DESTROY_IT(pool); 

#define CHECK_OSS_RESULT_NOBOOL(Code_True,Code_False) \
if (aos_status_is_ok(resp_status)){bVisitSuccess = true; Code_True }\
else {bVisitSuccess = false; Code_False} 

#define RETURN_FALSE \
return false;

#define PRING_LOG(Log) TEXT(Log); 

#define RETURN_TRUE \
return true;

#define DECLARATION_CALLBACK_DELEGATE(Func) \
FProgressCallbackDelegate Func##_Delegate; \
void Func##_Local(int64 ConsumedBytes, int64 TotalBytes) \
{ \
	FScopeLock ScopeLock(&Mutex); \
	auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()\
	{\
		Func##_Delegate.ExecuteIfBound(ConsumedBytes, TotalBytes);\
	}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

#define DECLARATION_CALLBACK_DELEGATE_PART(Func) \
FCallbackUploadPartDelegate Func##_Delegate; \
void Func##_Local(FString UploadId, int64  PartCount, int64 Size, int64 InTotal, bool bPart, bool  bComplete) \
{ \
	FScopeLock ScopeLock(&Mutex); \
	auto EventRef = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()\
	{\
		Func##_Delegate.ExecuteIfBound(UploadId, PartCount,Size,InTotal,bPart,bComplete);\
	}, TStatId(), nullptr, ENamedThreads::GameThread);\
/*	FTaskGraphInterface::Get().WaitUntilTaskCompletes(EventRef);*/\
}

#define OSS_REGISTRATION_CONFIGURATION \
IPlatformFile &PlatformFile = FPlatformFileManager::Get().GetPlatformFile(); \
FString AccountsSettingsPaths = FPaths::ProjectConfigDir() / TEXT("OSSLoaclAccountsSettings.ini"); \
if (!FPaths::FileExists(AccountsSettingsPaths))\
{\
	if (!FPaths::DirectoryExists(FPaths::ProjectConfigDir()))\
	{\
		if (!PlatformFile.CreateDirectoryTree(*FPaths::ProjectConfigDir()))\
		{\
			return;\
		}\
	}\
	FString StringContent = " ";\
	FFileHelper::SaveStringToFile(StringContent, *AccountsSettingsPaths, FFileHelper::EEncodingOptions::ForceUTF8); \
}

#define CLAIM_METADATA(OSSMeta) \
if (OSSMeta##.Num()) \
{\
	headers = aos_table_make(pool, OSSMeta##.Num()); \
	for (auto &Tmp : OSSMeta) \
	{ \
		apr_table_set(headers, TCHAR_TO_UTF8(*Tmp.Key), TCHAR_TO_UTF8(*Tmp.Value)); \
	}\
}

#define OSS_INFO_MSG(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_CYAN); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	UE_LOG(LogSimpleOSS, Log, TEXT("%s"), *Msg); \
	CLEAR_WARN_COLOR(); \
}

#define OSS_DEBUG_MSG(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_CYAN); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	UE_LOG(LogSimpleOSS, Log, TEXT("[DEBUG] %s"), *Msg); \
	CLEAR_WARN_COLOR(); \
}

#define OSS_SCREENDE_DEBUG_MSG(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_CYAN); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	UE_LOG(LogSimpleOSS, Log, TEXT("[SUCCESS] %s"), *Msg); \
	CLEAR_WARN_COLOR(); \
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::White, Msg); \
}

#define OSS_SUCCESS_MSG(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_GREEN); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	UE_LOG(LogSimpleOSS, Log, TEXT("[SUCCESS] %s"), *Msg); \
	CLEAR_WARN_COLOR(); \
}

#define OSS_SCREENDE_SUCCESS_MSG(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_GREEN); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	UE_LOG(LogSimpleOSS, Log, TEXT("[SUCCESS] %s"), *Msg); \
	CLEAR_WARN_COLOR(); \
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Green, Msg); \
}

#define OSS_WARNING_MSG(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_YELLOW); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	UE_LOG(LogSimpleOSS, Warning, TEXT("[WARNING] %s"), *Msg); \
	CLEAR_WARN_COLOR(); \
}

#define OSS_SCREEN_WARNING_MSG(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_YELLOW); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	const FString NewMsg = FString::Printf(TEXT("[WARNING] %s"), *Msg); \
	UE_LOG(LogSimpleOSS, Warning, TEXT("%s"), *NewMsg); \
	CLEAR_WARN_COLOR(); \
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, NewMsg); \
}

#define OSS_ERROR_MSG(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_RED); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	UE_LOG(LogSimpleOSS, Error, TEXT("[ERROR] %s"), *Msg); \
	CLEAR_WARN_COLOR(); \
}

#define OSS_SCREEN_ERROR_MSG(Format, ...) \
{ \
	SET_WARN_COLOR(COLOR_RED); \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	const FString NewMsg = FString::Printf(TEXT("[ERROR] %s"), *Msg); \
	UE_LOG(LogSimpleOSS, Error, TEXT("%s"), *NewMsg); \
	CLEAR_WARN_COLOR(); \
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, NewMsg); \
}

#define OSS_SCREENDE_MESSAGE(Format, ...) \
{ \
	const FString Msg = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::White, Msg); \
}