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

#pragma once

#include "CoreMinimal.h"
#include "SimpleOSSType.h"
#include "Misc/Paths.h"
#include <string>
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

	//OSS API
	struct SIMPLEOSS_API FOSS
	{
		friend class FSimpleOSSManage;

		/*End Point*/
		FString GetEndpoint();

		//初始化
		//////////////////////////////////////////////////////////////////////////
#pragma region InitOSS
		/**
		* Initialize oss account ,it can be used as a temporary account or written into the code directly 
		* @InAccessKeyId		Key ID accessed 
		* @InAccessKeySecret	Secret
		* @InEndpoint			Geographical node 
		*/
		void InitAccounts(const FString &InAccessKeyId, const FString &InAccessKeySecret, const FString &InEndpoint);

		/**
		* Initialize configuration 
		* InConf OSS configuration 
		*/
		void InitConf(const FClientConfiguration &InConf);
#pragma endregion InitOSS
		
		///**
		//* 获取对象的URL
		//* @InBucketName 桶名字
		//* @InObjectName	对象名
		//*/
		//FString GeneratePresignedUrl(const FString &InBucketName, const FString &InObjectName);
		//
		///**
		//* 通过URL上传数据 OSS桶的权限必须是读写
		//* @ InURL URL
		//* @ InFilePaths	本地路径
		//* @ InProgressCallback 回调
		//*/
		//bool PutObjectByUrl(const FString& InURL, const FString& InFilePaths, ProgressCallback InProgressCallback = nullptr) const;
		//
		///**
		//* 通过URL下载数据 OSS桶的权限必须是只读
		//* @ InURL URL
		//* @ InFilePaths	本地路径
		//* @ InProgressCallback 回调
		//*/
		//bool GetObjectByUrl(const FString& InURL, const FString& InFilePaths, ProgressCallback InProgressCallback = nullptr) const;
		//
		///**
		//* 通过URL下载数据 OSS桶的权限必须是读写
		//* @ InURL URL
		//* @ InFilePaths	本地路径
		//* @ InProgressCallback 回调
		//*/
		//void AsyncPutObjectByUrl(const FString& InURL, const FString& InFilePaths, ProgressCallback InProgressCallback = nullptr) const;
	
		///**
		//* 通过URL下载数据 OSS桶的权限必须是读写
		//* @ InURL URL
		//* @ InFilePaths	本地路径
		//* @ InProgressCallback 回调
		//*/
		//void AsyncGetObjectByUrl(const FString& InURL, const FString& InFilePaths, ProgressCallback InProgressCallback = nullptr) const;
		
		//桶
		//////////////////////////////////////////////////////////////////////////
#pragma region Bucket
		/**
		*Whether the bucket exists  
		*@ inbucketname bucket name
		*/
		bool DoesBucketExist(const FString &InBucketName);

		/**
		* Create bucket 
		* @ InBucketName		Bucket name defined 
		* @ OSSStorageType	Storage type 
		* @ Acl				Jurisdiction 
		*/
		bool CreateBucket(const FString &InBucketName,EOSSStorageType OSSStorageType = EOSSStorageType::STANDARD,EAcl Acl = EAcl::PRIVATE);

		/**
		* Enumerated bucket 
		* @InBucketName Can get bucket name set 
		*/
		bool ListBuckets(TArray<FString> &InBucketName);

		/**
		* Get bucket information 
		* @ InBucketName			Bucket name
		* @ InBucketInfo			Can get the information of the bucket 
		*/
		bool GetBucketsInfo(const FString &InBucketName,FBucketInfo &InBucketInfo);

		/*
		 * @brief  get oss bucket storage capacity
		 * @param[in]   bucket        the oss bucket name
		 * return -1 is flase
		 */
		//int64 GetBucketsStorageCapacity(const FString &InBucketName);

		/**
		* Location to get buckets 
		* @ InBucketName			Bucket name
		*/
		FString GetBucketLocation(const FString &InBucketName);

		/**
		* Delete bucket
		* @InBucketName				Bucket name
		*/
		bool DeleteBucket(const FString &InBucketName);

		/**
		* Set bucket permission 
		* @ InBucketName			Bucket name
		* @BucketAcl				permission
		*/
		bool SetBucketAcl(const FString &InBucketName, EAcl BucketAcl);

		/**
		* Get bucket permission 
		* @InBucketName				Bucket name
		*/
		EAcl GetBucketAcl(const FString InBucketName);
//		
//		/**
//		* Set the requester payment mode of bucket 
//		* @ InBucketName			Bucket name
//		* @RequestPayer 请求者模式
//		*/
//		bool SetBucketRequestPayment(const FString &InBucketName, ERequestPayerType InRequestPayer);
//
//		/**
//		* 获取桶的请求者付费模式
//		* @ InBucketName			Bucket name
//		*/
//		ERequestPayerType GetBucketRequestPayment(const FString &InBucketName);

		/**
		* Setting up the anti-theft chain 
		* @ InBucketName			Bucket name
		* @ URLReferer				Referer	list
		* @ bAllowEmptyRefere		Allow empty Referer	 
		*/
		bool SetBucketReferer(const FString &InBucketName, const TArray<FString> &URLReferer, bool bAllowEmptyRefere = false);

		/**
		* Get Bucket Referer
		* @ InBucketName			Bucket name
		* @ BucketReferers			Anti theft chain reference 
		*/
		bool GetBucketReferer(const FString &InBucketName, TArray<FString> &BucketReferers);

		/**
		* Remove the anti-theft chain
		* @ InBucketName			Bucket name
		* @ URLReferer				List of anti-theft chains
		* @ bAllowEmptyRefere		Allow empty chain
		*/
		bool DeleteBucketReferer(const FString &InBucketName, bool bAllowEmptyRefere = true);
#pragma endregion Bucket

		//Object manipulation 
		//////////////////////////////////////////////////////////////////////////
#pragma region Object
		/**
		* Get metadata information 
		* @ InBucketName			Bucket name
		* @ InObjectName			Object name
		* @OSSObjectInfo			Object metadata information 
		*/
		bool GetObjectMeta(const FString &InBucketName, const FString &InObjectName, FOSSObjectMeta &OSSObjectInfo);

		/**
		* Get read and write permissions for objects 
		* @ InBucketName			Bucket name
		* @ InObjectName			Object name
		*/
		EAcl GetObjectAcl(const FString &InBucketName, const FString &InObjectName);

		/**
		* Set read and write permissions for objects 
		* @ InBucketName			Bucket name
		* @ InObjectName			Object name
		* @ InAcl					read-write permission 
		*/
		bool SetObjectAcl(const FString &InBucketName, const FString &InObjectName, EAcl InAcl);

		/**
		* Whether the object exists 
		* @ InBucketName			Bucket name
		* @ InObjectName			Object name
		*/
		bool DoesObjectExist(const FString &InBucketName, const FString &InObjectName);

		/**
		* Enumerating objects are recursive 
		* @ InBucketName			Bucket name
		* @ InObjectName			Object name
		* @ InKeyPrefix				List prefixes such as folder names 
		* @ InMarker 				List files after the specified marker 
		* @ MaxNumber				Maximum number of enumerations 
		*/
		bool ListObjects(const FString &InBucketName, TArray<FOSSObjectInfo> &ObjectName,int32 MaxNumber = 100);

		/**
		* We can list our files by prefixes. The files stored in the server do not have folders like those stored in windows,
		* but are prefixes of files, so prefixes are folders 
		* @ InBucketName			Bucket name
		* @ Prefix					Prefix
		* @ InObjectName			Object name
		* @ InKeyPrefix				List prefixes such as folder names
		* @ InMarker 				List files after the specified marker
		* @ MaxNumber				Maximum number of enumerations
		*/
		bool ListObjectsByPrefix(const FString &InBucketName,const FString &Prefix, TArray<FOSSObjectInfo> &ObjectName,int32 MaxNumber = 100);

		/**
		* Can get children 
		* @ InBucketName			Bucket name
		* @ Prefix					Prefix
		* @ InObjectName			Object name
		* @ InKeyPrefix				List prefixes such as folder names
		* @ InMarker 				List files after the specified marker
		* @ MaxNumber				Maximum number of enumerations
		*/
		bool RecursiveListObjectsByPrefix(const FString &InBucketName, const FString &Prefix, TArray<FOSSObjectInfo> &ObjectName, int32 MaxNumber = 100);
		//
		///**
		//* Set the storage type of the object 
		//* @ InBucketName			Bucket name
		//* @ InObjectName			Object name
		//* @OSSStorageType			Storage Type		
		//*/
		//bool SetObjectStorageType(const FString &InBucketName, const FString &InObjectName, EOSSStorageType OSSStorageType);
		//
		///**
		//* Get the storage type of the object 
		//* @ InBucketName			Bucket name
		//* @ InObjectName			Object name
		//*/
		//EOSSStorageType GetObjectStorageType(const FString &InBucketName, const FString &InObjectName);

		/**
		*Create a soft connect shortcut
		* @ InBucketName			Bucket name
		* @ InObjectName			Shortcut name 
		* @ InLinkObjectName		Where data to connect 
		*/
		bool CreateSymlink(const FString &InBucketName, const FString &InObjectName, const FString &InLinkObjectName);
		
		/**
		*Get the file content pointed to by the soft link
		* @ InBucketName			Bucket name
		* @ InObjectName			Object name
		*/
		FString GetSymlink(const FString &InBucketName, const FString &InLinkName);

#pragma endregion Object

		//Resource download 
		//////////////////////////////////////////////////////////////////////////
#pragma region GetObjects
		/**
		* Download Memory
		* @ InBucketName			Bucket name
		* @ InObjectName			Object name
		* @ ContextString			Data
		* @ PartSize				Part Size
		* @ Range					Download scope
		* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
		*/
		bool GetObjectToMemory(const FString &InBucketName, const FString &InObjectName, FString &ContextString, FRange Range = FRange(), ProgressCallback InProgressCallback = nullptr);

		/**
		* Download locl
		* @ InBucketName			Bucket name
		* @ InObjectName			Object name
		* @ InLocalPaths			Local path
		* @ PartSize				Part Size
		* @ Range					Download scope
		* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
		*/
		bool GetObjectToLocal(const FString &InBucketName, const FString &InObjectName, const FString &InLocalPaths, FRange Range = FRange(), ProgressCallback InProgressCallback = nullptr);

		/**
		* Resume Download 
		* @ InBucketName			Bucket name
		* @ InObjectName			Object name
		* @ InLocalPaths			Local path
		* @ PartSize				Part Size
		* @ Range					Download scope 
		* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
		*/
		bool ResumableDownloadObject(const FString &InBucketName, const FString &InObjectName, const FString &InDownloadFilePath, int32 PartSize = 1024 * 1024 * 10,ProgressCallback InProgressCallback = nullptr/*,const FString &InCheckpointFilePath = FPaths::ProjectSavedDir()*/);
		
#pragma endregion GetObjects

		/**
		* Copy object from source bucket to target bucket 
		* @ InSourceBucketName			Source bucket to copy
		* @ InSourceObjectName			Source object to copy
		* @ InCopyBucketName			The bucket copied from the original bucket to the object yes
		* @ InCopyObjectName			Copy from the object of the original bucket to the object name under the corresponding bucket
		* @ InProgressCallback			Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
		*/
		bool CopyObject(const FString &InSourceBucketName, const FString &InSourceObjectName, const FString &InCopyBucketName, const FString &InCopyObjectName);
	
		/**
		* Copy part from source bucket to target bucket 
		* @ UploadID					Upload ID
		* @ InSourceBucketName			Source bucket to copy 
		* @ InSourceObjectName			Source object to copy 
		* @ InCopyBucketName			The bucket copied from the original bucket to the object yes 
		* @ InCopyObjectName			Copy from the object of the original bucket to the object name under the corresponding bucket 
		* @ InProgressCallback			Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
		*/
	//	bool CopyPart(FString &UploadID, const FString &InSourceBucketName, const FString &InSourceObjectName, const FString &InCopyBucketName, const FString &InCopyObjectName);
		
		/**
		* Delete a single resource 
		* @ InBucketName			Bucket name
		* @ InObjectName			Object name
		*/
		bool DeleteObject(const FString &InBucketName, const FString &InObjectName);

		/**
		* Delete individual resources by prefix 
		* @ InBucketName			Bucket name
		* @ InObjectName			Object name
		*/
		bool DeleteObjectByPrefix(const FString &InBucketName,const FString &InPrefix);

		/**
		* Delete multiple resources 
		* @ InBucketName			Bucket name
		* @ InObjectName			Multiple objects 
		*/
		bool DeleteObjects(const FString &InBucketName, const TArray<FString> &InObjectName);
		
		/**
		* List upload segments 
		* @ InBucketName			Bucket name
		* @ MultipartPartsInfo		List upload segments 
		* @ MaxParts				Maximum number of segments listed 
		*/
		bool ListMultipartUploads(const FString &InBucketName, TArray<FMultipartPartsUploadsInfo> &MultipartPartsInfo, int32 MaxParts = 1000);
		
#pragma region Async
		/**
		* Asynchronous upload object 
		* @ InBucketName			Bucket name
		* @ InObjectName			Object name
		* @ InLocalPaths			Local path
		* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
		* @ OSSMeta					OSS metadata can be customized or used. Please refer to oss_define.c for details
		*/
		void AsyncPutObject(const FString &InBucketName, const FString &InLocalPaths, const FString &InObjectName = FString(), ProgressCallback InProgressCallback = nullptr, const TMap<FString, FString> &OSSMeta = TMap<FString, FString>());
		
		/**
		* Asynchronous upload object by Memory string
		* @ InBucketName			Bucket name
		* @ InObjectName			Object name
		* @ Data					Data to upload, which exists in memory.
		* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
		* @ OSSMeta					OSS metadata can be customized or used. Please refer to oss_define.c for details
		*/
		void AsyncPutObjectByMemory(const FString &InBucketName, const FString &Data, const FString &InObjectName = FString(), ProgressCallback InProgressCallback = nullptr, const TMap<FString, FString> &OSSMeta = TMap<FString, FString>());
		
		/**
		* Asynchronous version resumable continuation prevents breakpoints. Next upload will connect 
		* @ InBucketName			Bucket name
		* @ InObjectName			Object name
		* @ InLocalPaths			Local path
		* @ InCheckpointFilePath	Resumable cache storage path 
		* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
		* @ OSSMeta					OSS metadata can be customized or used. Please refer to oss_define.c for details
		*/
		void AsyncResumableUploadObject(const FString &InBucketName, const FString &InObjectName, const FString &InUploadFilePath, int64 PartSize, ProgressCallback InProgressCallback = nullptr, const TMap<FString, FString> &OSSMeta = TMap<FString, FString>());
		
		/**
		* The scheme of asynchronous fragment upload for large files 
		* @ InUploadId				InUpload ID
		* @ InBucketName			Bucket name
		* @ InObjectName			Object name
		* @ InLocalPaths			Local path
		* @ PartSize				Part Size
		* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
		* @ OSSMeta					OSS metadata can be customized or used. Please refer to oss_define.c for details
		*/
		FString AsyncUploadPart(const FString &InBucketName, const FString &InObjectName, const FString &InLocalPaths, int32 PartSize = 1024 * 1024 * 10, CallbackUploadPart InCallbackUploadPart = nullptr, const TMap<FString, FString> &OSSMeta = TMap<FString, FString>());
		
		/**
		* Asynchronous download to local 
		* @ InBucketName			Bucket name
		* @ InObjectName			Object name
		* @ InLocalPaths			Local path
		* @Range					Download scope 
		* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
		*/
		void AsyncGetObjectToLocal(const FString &InBucketName, const FString &InObjectName, const FString &InLocalPaths, FRange Range = FRange(), ProgressCallback InProgressCallback = nullptr);

		/**
		* Asynchronous breakpoint resume Download 
		* @ InBucketName			Bucket name
		* @ InObjectName			Object name
		* @ InLocalPaths			Local path
		* @ Range					Download scope 
		* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
		* @InCheckpointFilePath		Resumable cache storage path 
		*/
		void AsyncResumableDownloadObject(const FString &InBucketName, const FString &InObjectName, const FString &InDownloadFilePath, int32 PartSize = 1024 * 1024 * 10, ProgressCallback InProgressCallback = nullptr);
		
#pragma endregion Async
		//Resource uploading 
		//////////////////////////////////////////////////////////////////////////
#pragma region PutObject

		/**
		* Upload resources locally 
		* @ InBucketName			Bucket name
		* @ InLocalPaths			Local path
		* @ InObjectName			Object name
		* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
		* @ OSSMeta					OSS metadata can be customized or used. Please refer to oss_define.c for details
		*/
		bool PutObject(const FString &InBucketName, const FString &InLocalPaths, const FString &InObjectName = FString(), ProgressCallback InProgressCallback = nullptr, const TMap<FString, FString> &OSSMeta = TMap<FString, FString>());

		/**
		* Upload resources memory
		* @ InBucketName			Bucket name
		* @ Data					Data to upload, which exists in memory.
		* @ InObjectName			Object name
		* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
		* @ OSSMeta					OSS metadata can be customized or used. Please refer to oss_define.c for details
		*/
		bool PutObjectByMemory(const FString &InBucketName,const FString &Data, const FString &InObjectName = FString(), ProgressCallback InProgressCallback = nullptr, const TMap<FString, FString> &OSSMeta = TMap<FString, FString>());

		///**
		//* From local apend data to oss server
		//* @ InBucketName			Bucket name
		//* @ InLocalPaths			Local path
		//* @ InObjectName			Object name
		//*/
		//bool AppendObject(const FString &InBucketName, const FString &InLocalPaths, const FString &InObjectName = FString());
		
		/**
		* Resumable continue to prevent breakpoint, next upload will connect 
		* @ InBucketName			Bucket name
		* @ InObjectName			Object name
		* @ InLocalPaths			Local path
		* @ InCheckpointFilePath	Resumable cache storage path 
		* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
		* @ OSSMeta					OSS metadata can be customized or used. Please refer to oss_define.c for details
		*/
		bool ResumableUploadObject(const FString &InBucketName, const FString &InObjectName, const FString &InUploadFilePath, int64 PartSize = 1024 *1024 *10, ProgressCallback InProgressCallback = nullptr, const TMap<FString, FString> &OSSMeta = TMap<FString, FString>());

		/**
		* Upload scheme for large files by segment upload 
		* @ InUploadId				Upload ID
		* @ InBucketName			Bucket name
		* @ InObjectName			Object name
		* @ InLocalPaths			Local path 
		* @ PartSize				Size of segments 
		* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests 
		* @ OSSMeta					OSS metadata can be customized or used. Please refer to oss_define.c for details
		*/
		bool UploadPart(FString &InUploadId, const FString &InBucketName, const FString &InObjectName, const FString &InLocalPaths, int32 PartSize = 1024 * 1024 * 10,const TMap<FString, FString> &OSSMeta = TMap<FString, FString>());

		/**
		* Cancel fragment upload 
		* @ InBucketName			Bucket name 
		* @ InObjectName			Object name
		* @ InUploadId				Upload ID 
		*/
		bool AbortMultipartUpload(const FString &InBucketName, const FString &InObjectName, const FString &InUploadId);
		
#pragma endregion PutObject
		//Image processing 
		//////////////////////////////////////////////////////////////////////////
#pragma region ImageProcessing

		/**
		* Zoom downloaded pictures 
		* @ Size				Scaled picture size 
		* @ InBucketName		Bucket name
		* @ URLReferer			List of anti-theft chains
		* @ bAllowEmptyRefere	Allow empty chain
		*/
		bool GetImageObjectByResize(const FVector2D &Size, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);
		
		/**
		* Crop downloaded pictures 
		* @ Size				Cropped picture information 
		* @ InBucketName		Bucket name
		* @ URLReferer			List of anti-theft chains
		* @ bAllowEmptyRefere	Allow empty chain
		*/
		bool GetImageObjectByCrop(const FVector4 &Size, int32 Ratio, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);

		/**
		* Sharpen download picture 
		* @ Sharpen				Sharpen picture factor 
		* @ InBucketName		Bucket name
		* @ URLReferer			List of anti-theft chains
		* @ bAllowEmptyRefere	Allow empty chain
		*/
		bool GetImageObjectBySharpen(const int32 Sharpen, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);

		/**
		* Watermark download picture
		* @ WaterMarkObjectName	Watermark image name 
		* @ InBucketName		Bucket name
		* @ URLReferer			List of anti-theft chains
		* @ bAllowEmptyRefere	Allow empty chain
		*/
		bool GetImageObjectByWaterMark(const FString WaterMarkObjectName, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);

		/**
		* Rotate downloaded pictures 
		* @ Rotate				Euler angle of picture rotation 
		* @ InBucketName		Bucket name
		* @ URLReferer			List of anti-theft chains
		* @ bAllowEmptyRefere	Allow empty chain
		*/
		bool GetImageObjectByRotate(const int32 Rotate, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);
		
		/**
		* Use OSS template style 
		* @ OSSStyleName		OSS style name 
		* @ InBucketName		Bucket name
		* @ URLReferer			List of anti-theft chains
		* @ bAllowEmptyRefere	Allow empty chain
		*/
		bool GetImageObjectByStyle(const FString OSSStyleName, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);
		
		/**
		* Cascade processing
		* @ CascadeCommand		Multilayer command 
		* @ InBucketName		Bucket name 
		* @ URLReferer			List of anti-theft chains 
		* @ bAllowEmptyRefere	Allow empty chain
		*/
		bool GetImageObjectByCascade(const TArray<FString> &CascadeCommand, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);

		/**
		* Get picture information 
		* @InBucketName			Bucket name 
		* @ URLReferer			List of anti-theft chains 
		* @ bAllowEmptyRefere	Allow empty chain
		*/
		bool GetImageInfo( const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);
private:
		/**
		* General interface for processing downloaded images 
		* @InBucketName Bucket name 
		* @InObjectName	Object name
		* @InFileNametoSave Storage path, local disk 
		* @Command How to process the image 
		*/
		bool GetImageObject(const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave, const FString &Command);
	
		/**
		* General interface for processing downloaded images Internal
		* @InBucketName Bucket name 
		* @InObjectName	Object name
		* @InFileNametoSave Storage path, local disk 
		* @Command How to process the image 
		*/
		bool GetImageObject_Internal(const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave, const FString &Command);
#pragma endregion ImageProcessing
private:
	/**
	* Provide a local initialization account function, which needs to be configured in the project configuration.
	* For details, please refer to " ../../../Projectname/Config/OssLoadAccountsSettings.ini"
	*/
	void InitLocalAccounts();

	//Save accounts info
	void SaveAccounts();

	//Initialization ,only provides internal calls 
	void init_options(void *options);

	//Convert string to utf8 format , only provides internal calls 
	ANSICHAR *multibyte_to_utf8(const ANSICHAR * ch, ANSICHAR *str, int32 size);

	/*Upload part Internal*/
	bool UploadPart_Internal(void *pool, void *oss_client_options, const FString &InUploadId,const FString &InBucketName,const FString &InObjectName,const FString &InLocalPaths,int32 PartSize /*= 1024 * 1024 * 10*/,CallbackUploadPart InCallbackUploadPart);

	/*List Objects Internal*/
	bool ListObjects_Internal(const FString &InBucketName,const FString &Delimiter, const FString &Prefix,TArray<FOSSObjectInfo> &ObjectName, int32 MaxNumber = 100);
private:

		//There is no way for us to change the behavior of ansichar type through ue4fstring, 
		//because curl is prone to errors, which can be solved by STD string 
		std::string AccessKeyId;
		std::string AccessKeySecret;
		std::string Endpoint;

		FCriticalSection Mutex;			
		FClientConfiguration Conf;
	};

	FORCEINLINE FOSS &GetOSS() { return OSS; }
private:
	static FSimpleOSSManage	*SimpleOSS;

	FOSS OSS;
};

#define SIMPLE_OSS FSimpleOSSManage::Get()->GetOSS()