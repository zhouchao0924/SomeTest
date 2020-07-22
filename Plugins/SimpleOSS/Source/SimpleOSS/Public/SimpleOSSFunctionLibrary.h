// Copyright (C) RenZhai.2019.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleOSSType.h"
#include "SimpleOSSFunctionLibrary.generated.h"
 
UCLASS()
class SIMPLEOSS_API USimpleOSSFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


	/*End Point*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS | InitOSS")
	static FString GetEndpoint();

	/**
	* Initialize oss account
	* @InAccessKeyId		Key ID accessed
	* @InAccessKeySecret	Secret
	* @InEndpoint			Geographical node
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS | InitOSS")
	static void InitAccounts(const FString &InAccessKeyId, const FString &InAccessKeySecret, const FString &InEndpoint);
	
	/**
	* Initialize configuration
	* InConf OSS configuration
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS | InitOSS")
	static void InitConf(const FClientConfiguration &InConf);

	/**
	*Whether the bucket exists
	*@ inbucketname bucket name
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS | Bucket")
	static bool DoesBucketExist(const FString &InBucketName);

	/**
	* Create bucket
	* @ InBucketName		Bucket name defined
	* @ OSSStorageType	Storage type
	* @ Acl				Jurisdiction
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS | Bucket")
	static bool CreateBucket(const FString &InBucketName, EOSSStorageType OSSStorageType = EOSSStorageType::STANDARD, EAcl Acl = EAcl::PRIVATE);

	/**
	* Enumerated bucket
	* @InBucketName Can get bucket name set
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS | Bucket")
	static bool ListBuckets(TArray<FString> &InBucketName);

	/**
	* Get bucket information
	* @ InBucketName			Bucket name
	* @ InBucketInfo			Can get the information of the bucket
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS | Bucket")
	static bool GetBucketsInfo(const FString &InBucketName, FBucketInfo &InBucketInfo);

	/**
	* Location to get buckets
	* @ InBucketName			Bucket name
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS | Bucket")
	static FString GetBucketLocation(const FString &InBucketName);

	/*
	 * @brief  get oss bucket storage capacity
	 * @param[in]   bucket        the oss bucket name
	 * return -1 is flase 
	 */
	//UFUNCTION(BlueprintPure, Category = "SimpleOSS | Bucket")
	//static int64 GetBucketsStorageCapacity(const FString &InBucketName);

	/**
	* Delete bucket
	* @InBucketName				Bucket name
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS | Bucket")
	static bool DeleteBucket(const FString &InBucketName);

	/**
	* Set bucket permission
	* @ InBucketName			Bucket name
	* @BucketAcl				permission
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS | Bucket")
	static bool SetBucketAcl(const FString &InBucketName, EAcl BucketAcl);

	/**
	* Get bucket permission
	* @InBucketName				Bucket name
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS | Bucket")
	static EAcl GetBucketAcl(const FString InBucketName);

	/**
	* Setting up the anti-theft chain
	* @ InBucketName			Bucket name
	* @ URLReferer				Referer	list
	* @ bAllowEmptyRefere		Allow empty Referer
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS | Bucket")
	static bool SetBucketReferer(const FString &InBucketName, const TArray<FString> &URLReferer, bool bAllowEmptyRefere = false);

	/**
	* Get Bucket Referer
	* @ InBucketName			Bucket name
	* @ BucketReferers			Anti theft chain reference
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS | Bucket")
	static bool GetBucketReferer(const FString &InBucketName, TArray<FString> &BucketReferers);

	/**
	* Remove the anti-theft chain 
	* @ InBucketName			Bucket name
	* @ URLReferer				List of anti-theft chains
	* @ bAllowEmptyRefere		Allow empty chain
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS | Bucket")
	static bool DeleteBucketReferer(const FString &InBucketName, bool bAllowEmptyRefere = true);

	/**
	* Get metadata information
	* @ InBucketName			Bucket name
	* @ InObjectName			Object name
	* @OSSObjectInfo			Object metadata information
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS | Objects")
	static bool GetObjectMeta(const FString &InBucketName, const FString &InObjectName, FOSSObjectMeta &OSSObjectInfo);

	/**
	* Get read and write permissions for objects
	* @ InBucketName			Bucket name
	* @ InObjectName			Object name
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS | Objects")
	static EAcl GetObjectAcl(const FString &InBucketName, const FString &InObjectName);

	/**
	* Set read and write permissions for objects
	* @ InBucketName			Bucket name
	* @ InObjectName			Object name
	* @ InAcl					read-write permission
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS | Objects")
	static bool SetObjectAcl(const FString &InBucketName, const FString &InObjectName, EAcl InAcl);

	/**
	* Whether the object exists
	* @ InBucketName			Bucket name
	* @ InObjectName			Object name
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS | Objects")
	static bool DoesObjectExist(const FString &InBucketName, const FString &InObjectName);

	/**
	* Enumerating objects are recursive
	* @ InBucketName			Bucket name
	* @ InObjectName			Object name
	* @ InKeyPrefix				List prefixes such as folder names
	* @ InMarker 				List files after the specified marker
	* @ MaxNumber				Maximum number of enumerations
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS | Objects")
	static bool ListObjects(const FString &InBucketName, TArray<FOSSObjectInfo> &ObjectName, int32 MaxNumber = 100);

	/**
	* The relative path of the server needs to be specified 
	* @ InBucketName			Bucket name
	* @ ServerFileRelativePaths	Server file relative path
	* @ InObjectName			Object name
	* @ InKeyPrefix				List prefixes such as folder names
	* @ InMarker 				List files after the specified marker
	* @ MaxNumber				Maximum number of enumerations
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS | Objects")
	static bool ListObjectsByFilePaths(const FString &InBucketName, const FString &ServerFileRelativePaths, TArray<FOSSObjectInfo> &ObjectName, int32 MaxNumber = 100);

	/**
	* Can get children
	* @ InBucketName			Bucket name
	* @ Prefix					Prefix
	* @ InObjectName			Object name
	* @ InKeyPrefix				List prefixes such as folder names
	* @ InMarker 				List files after the specified marker
	* @ MaxNumber				Maximum number of enumerations
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS | Objects")
	static bool RecursiveListObjectsByFilePaths(const FString &InBucketName, const FString &ServerFileRelativePaths, TArray<FOSSObjectInfo> &ObjectName, int32 MaxNumber = 100);

	///**
	//* Set the storage type of the object
	//* @ InBucketName			Bucket name
	//* @ InObjectName			Object name
	//* @OSSStorageType			Storage Type
	//*/
	//UFUNCTION(BlueprintCallable, Category = "SimpleOSS | Objects")
	//static bool SetObjectStorageType(const FString &InBucketName, const FString &InObjectName, EOSSStorageType OSSStorageType);//Error

	///**
	//* Get the storage type of the object
	//* @ InBucketName			Bucket name
	//* @ InObjectName			Object name
	//*/
	//UFUNCTION(BlueprintPure, Category = "SimpleOSS | Objects")
	//static EOSSStorageType GetObjectStorageType(const FString &InBucketName, const FString &InObjectName);//Error

	/**
	*Create a soft connect shortcut
	* @ InBucketName			Bucket name
	* @ InObjectName			Shortcut name
	* @ InLinkObjectName		Where data to connect
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS | Objects")
	static bool CreateSymlink(const FString &InBucketName, const FString &InObjectName, const FString &InLinkObjectName);

	/**
	*Get the file content pointed to by the soft link
	* @ InBucketName			Bucket name
	* @ InObjectName			Object name
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS | Objects")
	static FString GetSymlink(const FString &InBucketName, const FString &InLinkName);

	/**
	* Copy object from source bucket to target bucket
	* @ InSourceBucketName			Source bucket to copy
	* @ InSourceObjectName			Source object to copy
	* @ InCopyBucketName			The bucket copied from the original bucket to the object yes
	* @ InCopyObjectName			Copy from the object of the original bucket to the object name under the corresponding bucket
	* @ InProgressCallback			Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS|Objects")
	static bool CopyObject(const FString &InSourceBucketName, const FString &InSourceObjectName, const FString &InCopyBucketName, const FString &InCopyObjectName);

	///**
	//* Copy part from source bucket to target bucket
	//* @ UploadID					Upload ID
	//* @ InSourceBucketName			Source bucket to copy
	//* @ InSourceObjectName			Source object to copy
	//* @ InCopyBucketName			The bucket copied from the original bucket to the object yes
	//* @ InCopyObjectName			Copy from the object of the original bucket to the object name under the corresponding bucket
	//* @ InProgressCallback			Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
	//*/
	//UFUNCTION(BlueprintPure, Category = "SimpleOSS|Objects")
	//static bool CopyPart(FString &UploadID, const FString &InSourceBucketName, const FString &InSourceObjectName, const FString &InCopyBucketName, const FString &InCopyObjectName);
	//
	/**
	* Delete a single resource
	* @ InBucketName			Bucket name
	* @ InObjectName			Object name
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS|Objects")
	static bool DeleteObject(const FString &InBucketName, const FString &InObjectName);

	/**
	* Delete individual resources by prefix
	* @ InBucketName			Bucket name
	* @ InObjectName			Object name
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS|Objects")
	static bool DeleteObjectByPrefix(const FString &InBucketName, const FString &InPrefix);

	/**
	* Delete all files under the file path specified by the server 
	* @ InBucketName			Bucket name
	* @ InServerFilePath			Object name
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS|Objects")
	static bool DeleteObjectByFilePath(const FString &InBucketName, const FString &InServerFilePath);

	/**
	* Delete multiple resources
	* @ InBucketName			Bucket name
	* @ InObjectName			Multiple objects
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS|Objects")
	static bool DeleteObjects(const FString &InBucketName, const TArray<FString> &InObjectName);

	/**
	* List upload segments
	* @ InBucketName			Bucket name
	* @ MultipartPartsInfo		List upload segments
	* @ MaxParts				Maximum number of segments listed
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS|Objects")
	static bool ListMultipartUploads(const FString &InBucketName, TArray<FMultipartPartsUploadsInfo> &MultipartPartsInfo, int32 MaxParts = 1000);

	/**
	* Download locl
	* @ InBucketName			Bucket name
	* @ InObjectName			Object name
	* @ InLocalPaths			Local path
	* @ PartSize				Part Size
	* @ Range					Download scope
	* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS|Get")
	static bool GetObjectToLocal(FString InBucketName, FString InObjectName, FString InLocalPaths, FRange Range);

	/**
	* Download Memory
	* @ InBucketName			Bucket name
	* @ InObjectName			Object name
	* @ ContextString			Data
	* @ PartSize				Part Size
	* @ Range					Download scope
	* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS|Get")
	static bool GetObjectToMemory(const FString &InBucketName, const FString &InObjectName, FString &ContextString, FRange Range);

	/**
	* Resume Download
	* @ InBucketName			Bucket name
	* @ InObjectName			Object name
	* @ InLocalPaths			Local path
	* @ PartSize				Part Size
	* @ Range					Download scope
	* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS|Get")
	static bool ResumableDownloadObject(const FString &InBucketName, const FString &InObjectName, const FString &InDownloadFilePath, int32 PartSize = 10485760);

	/**
	* Upload resources locally
	* @ InBucketName			Bucket name
	* @ InLocalPaths			Local path
	* @ InObjectName			Object name
	* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
	* @ OSSMeta					OSS metadata can be customized or used. Please refer to oss_define.c for details
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS|Put")
	static bool PutObject(const FString &InBucketName, const FString &InLocalPaths, const FString &InObjectName, const TMap<FString, FString> &OSSMeta);

	/**
	* Upload resources memory
	* @ InBucketName			Bucket name
	* @ Data					Data to upload, which exists in memory.
	* @ InObjectName			Object name
	* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
	* @ OSSMeta					OSS metadata can be customized or used. Please refer to oss_define.c for details
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS|Put")
	static bool PutObjectByMemory(const FString &InBucketName, const FString &Data, const FString &InObjectName, const TMap<FString, FString> &OSSMeta);
	
	///**
	//* From local apend data to oss server
	//* @ InBucketName			Bucket name
	//* @ InLocalPaths			Local path
	//* @ InObjectName			Object name
	//*/
	//UFUNCTION(BlueprintCallable, Category = "SimpleOSS|Put")
	//static bool AppendObject(const FString &InBucketName, const FString &InLocalPaths, const FString &InObjectName ="");

	/**
	* Resumable continue to prevent breakpoint, next upload will connect
	* @ InBucketName			Bucket name
	* @ InObjectName			Object name
	* @ InLocalPaths			Local path
	* @ InCheckpointFilePath	Resumable cache storage path
	* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
	* @ OSSMeta					OSS metadata can be customized or used. Please refer to oss_define.c for details
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS|Put")
	static bool ResumableUploadObject(const FString &InBucketName, const FString &InObjectName, const FString &InUploadFilePath, int64 PartSize, const TMap<FString, FString> &OSSMeta);

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
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS|Put")
	static bool UploadPart(FString &InUploadId, const FString &InBucketName, const FString &InObjectName, const FString &InLocalPaths, int32 PartSize,const TMap<FString, FString> &OSSMeta);

	/**
	* Cancel fragment upload
	* @ InBucketName			Bucket name
	* @ InObjectName			Object name
	* @ InUploadId				Upload ID
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS|Put")
	static bool AbortMultipartUpload(const FString &InBucketName, const FString &InObjectName, const FString &InUploadId);

	/**
	* Asynchronous upload object
	* @ InBucketName			Bucket name
	* @ InObjectName			Object name
	* @ InLocalPaths			Local path
	* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
	* @ OSSMeta					OSS metadata can be customized or used. Please refer to oss_define.c for details
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS|Put|Async") //Asynchronous upload can not get the real-time transmission of bytes, this is the Ali Cloud Library problem
	static void AsyncPutObject(const FString &InBucketName, const FString &InObjectName , const FString &InLocalPaths,FProgressCallbackDelegate InProgressCallback, const TMap<FString, FString> &OSSMeta);

	/**
	* Asynchronous upload object by Memory string
	* @ InBucketName			Bucket name
	* @ InObjectName			Object name
	* @ Data					Data to upload, which exists in memory.
	* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
	* @ OSSMeta					OSS metadata can be customized or used. Please refer to oss_define.c for details
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS|Put|Async")
	static void AsyncPutObjectByMemory(const FString &InBucketName, const FString &Data, const FString &InObjectName, FProgressCallbackDelegate InProgressCallback , const TMap<FString, FString> &OSSMeta);

	/**
	* Asynchronous version resumable continuation prevents breakpoints. Next upload will connect
	* @ InBucketName			Bucket name
	* @ InObjectName			Object name
	* @ InLocalPaths			Local path
	* @ InCheckpointFilePath	Resumable cache storage path
	* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
	* @ OSSMeta					OSS metadata can be customized or used. Please refer to oss_define.c for details
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS|Put|Async")
	static void AsyncResumableUploadObject(const FString &InBucketName, const FString &InObjectName, const FString &InUploadFilePath, int64 PartSize, FProgressCallbackDelegate InProgressCallback , const TMap<FString, FString> &OSSMeta);

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
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS|Put|Async")
	static FString AsyncUploadPart( const FString &InBucketName, const FString &InObjectName, const FString &InLocalPaths, int32 PartSize, FCallbackUploadPartDelegate InProgressCallback, const TMap<FString, FString> &OSSMeta);

	/**
	* Asynchronous download to local
	* @ InBucketName			Bucket name
	* @ InObjectName			Object name
	* @ InLocalPaths			Local path
	* @Range					Download scope
	* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS|Get|Async")
	static void AsyncGetObjectToLocal(FString InBucketName, FString InObjectName, FString InLocalPaths, FRange Range, FProgressCallbackDelegate InProgressCallback);

	/**
	* Asynchronous breakpoint resume Download
	* @ InBucketName			Bucket name
	* @ InObjectName			Object name
	* @ InLocalPaths			Local path
	* @ Range					Download scope
	* @ InProgressCallback		Callback is used for asynchrony. No callback is required for synchronization. This interface is for compatibility with asynchronous requests
	* @InCheckpointFilePath		Resumable cache storage path
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleOSS|Get|Async")
	static void AsyncResumableDownloadObject(const FString &InBucketName, const FString &InObjectName, const FString &InDownloadFilePath, int32 PartSize,  FProgressCallbackDelegate InProgressCallback);

	/**
	* Zoom downloaded pictures
	* @ Size				Scaled picture size
	* @ InBucketName		Bucket name
	* @ URLReferer			List of anti-theft chains
	* @ bAllowEmptyRefere	Allow empty chain
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS|ImageProcessing")
	static bool GetImageObjectByResize(const FVector2D &Size, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);

	/**
	* Crop downloaded pictures
	* @ Size				Cropped picture information
	* @ InBucketName		Bucket name
	* @ URLReferer			List of anti-theft chains
	* @ bAllowEmptyRefere	Allow empty chain
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS|ImageProcessing")
	static bool GetImageObjectByCrop(const FVector4 &Size, int32 Ratio = 1, const FString &InBucketName = "", const FString &InObjectName = "", const FString &InFileNametoSave = "");

	/**
	* Sharpen download picture
	* @ Sharpen				Sharpen picture factor
	* @ InBucketName		Bucket name
	* @ URLReferer			List of anti-theft chains
	* @ bAllowEmptyRefere	Allow empty chain
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS|ImageProcessing")
	static bool GetImageObjectBySharpen(const int32 Sharpen, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);

	/**
	* Watermark download picture
	* @ WaterMarkObjectName	Watermark image name
	* @ InBucketName		Bucket name
	* @ URLReferer			List of anti-theft chains
	* @ bAllowEmptyRefere	Allow empty chain
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS|ImageProcessing")
	static bool GetImageObjectByWaterMark(const FString WaterMarkObjectName, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);

	/**
	* Rotate downloaded pictures
	* @ Rotate				Euler angle of picture rotation
	* @ InBucketName		Bucket name
	* @ URLReferer			List of anti-theft chains
	* @ bAllowEmptyRefere	Allow empty chain
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS|ImageProcessing")
	static bool GetImageObjectByRotate(const int32 Rotate, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);

	/**
	* Use OSS template style
	* @ OSSStyleName		OSS style name
	* @ InBucketName		Bucket name
	* @ URLReferer			List of anti-theft chains
	* @ bAllowEmptyRefere	Allow empty chain
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS|ImageProcessing")
	static bool GetImageObjectByStyle(const FString OSSStyleName, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);

	/**
	* Cascade processing
	* @ CascadeCommand		Multilayer command
	* @ InBucketName		Bucket name
	* @ URLReferer			List of anti-theft chains
	* @ bAllowEmptyRefere	Allow empty chain
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS|ImageProcessing")
	static bool GetImageObjectByCascade(const TArray<FString> &CascadeCommand, const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);

	/**
	* Get picture information
	* @InBucketName			Bucket name
	* @ URLReferer			List of anti-theft chains
	* @ bAllowEmptyRefere	Allow empty chain
	*/
	UFUNCTION(BlueprintPure, Category = "SimpleOSS|ImageProcessing")
	static bool GetImageInfo(const FString &InBucketName, const FString &InObjectName, const FString &InFileNametoSave);
};
