#include "PakFileUpload.h"
#include "SimpleOSSManage.h"
#include "HAL/FileManager.h"
#include "Core/ThreadCoreMacro.h"
#include "HTTP/SimpleHttpActionRequest.h"
#include "HTTP/SimpleHttpActionMultpleRequest.h"

#define ONE_KB 1024
#define ONE_MB 1048576
#define ONE_GB 1073741824
#define ONE_TB 1099511627776
#define ONE_PB 1.1259e+15
#define ONE_EB 1.1529e+18

int64 GetFileSize(const TCHAR* FIleName, uint32 Flags = 0)
{
	FScopedLoadingState ScopedLoadingState(FIleName);

	FArchive* Reader = IFileManager::Get().CreateFileReader(FIleName, Flags);
	if (!Reader)
	{
		return INDEX_NONE;
	}

	int64 TotalSize = Reader->TotalSize();
	Reader->Close();
	delete Reader;
	return TotalSize;
}

void PakFileUploadByOSS_Internal(
	const FString &InAccessKeyID,
	const FString &InAccessKeyIDSecret,
	const FString &InEndPoint,
	const FString &InBucketName,
	const FString &InLocalPaths,
	TArray<FString> &Error,
	FPakEditorUploadDelegate PakEditorUploadDelegate = FPakEditorUploadDelegate())
{
	SIMPLE_OSS.InitAccounts(InAccessKeyID, InAccessKeyIDSecret, InEndPoint);
	if (!SIMPLE_OSS.DoesBucketExist(InBucketName))
	{
		SIMPLE_OSS.CreateBucket(InBucketName);
	}

	//¹ýÂËÂ·¾¶
	TArray<FString> AllPaths;
	IFileManager::Get().FindFilesRecursive(AllPaths, *InLocalPaths, TEXT("*"), true, true);

	auto CallMainThread = [=](int32 i, const FString &Mess)
	{
		PakEditorUploadDelegate.ExecuteIfBound(AllPaths.Num() - (i + 1), AllPaths.Num(), Mess);
	};

	for (int32 i = 0; i < AllPaths.Num(); i++)
	{
		FString ObjectName = FPaths::GetCleanFilename(AllPaths[i]);
		if (!SIMPLE_OSS.DoesObjectExist(InBucketName, ObjectName))
		{
			int64 FileSize = GetFileSize(*AllPaths[i]);
			if (FileSize != INDEX_NONE)
			{
				if (FileSize >= ONE_GB)
				{
					if (FileSize >= (ONE_GB * 5))
					{
						if (SIMPLE_OSS.ResumableUploadObject(InBucketName, ObjectName, AllPaths[i]))
						{
							Error.Add(FString::Printf(TEXT("The file [%s] was uploaded succcessful through resumable upload object"), *ObjectName));
						}
						else
						{
							Error.Add(FString::Printf(TEXT("The [%s] resumable upload object failed"), *ObjectName));
						}
						if (Error.Num() > 0)
						{
							auto ThreadHandle = CALL_THREAD_Lambda(nullptr, ENamedThreads::GameThread, CallMainThread, i, Error[Error.Num() - 1]);
							WAITING_OTHER_THREADS_COMPLETED(ThreadHandle);
						}

					}
					else
					{
						FString InUploadID;
						int32 PartSize = FileSize / 10;
						if (SIMPLE_OSS.UploadPart(InUploadID, InBucketName, ObjectName, AllPaths[i], PartSize))
						{
							Error.Add(FString::Printf(TEXT("The file [%s] was uploaded succcessful through upload part"), *ObjectName));
						}
						else
						{
							Error.Add(FString::Printf(TEXT("The [%s] upload part failed"), *ObjectName));
						}
						if (Error.Num() > 0)
						{
							auto ThreadHandle = CALL_THREAD_Lambda(nullptr, ENamedThreads::GameThread, CallMainThread, i, Error[Error.Num() - 1]);
							WAITING_OTHER_THREADS_COMPLETED(ThreadHandle);
						}
					}
				}
				else
				{
					if (SIMPLE_OSS.PutObject(InBucketName, AllPaths[i], ObjectName))
					{
						Error.Add(FString::Printf(TEXT("The file [%s] was uploaded succcessful through put object"), *ObjectName));
					}
					else
					{
						Error.Add(FString::Printf(TEXT("The [%s] put object upload failed"), *ObjectName));
					}
					if (Error.Num() > 0)
					{
						auto ThreadHandle = CALL_THREAD_Lambda(nullptr, ENamedThreads::GameThread, CallMainThread, i, Error[Error.Num() - 1]);
						WAITING_OTHER_THREADS_COMPLETED(ThreadHandle);
					}
				}
			}
			else
			{
				Error.Add(FString::Printf(TEXT("[%s] Error Reading file size"), *ObjectName));
				if (Error.Num() > 0)
				{
					auto ThreadHandle = CALL_THREAD_Lambda(nullptr, ENamedThreads::GameThread, CallMainThread, i, Error[Error.Num() - 1]);
					WAITING_OTHER_THREADS_COMPLETED(ThreadHandle);
				}
			}
		}
		else
		{
			Error.Add(FString::Printf(TEXT("The [%s] already exists in the server OSS"), *ObjectName));

			if (Error.Num() > 0)
			{
				auto ThreadHandle = CALL_THREAD_Lambda(nullptr, ENamedThreads::GameThread, CallMainThread, i, Error[Error.Num() - 1]);
				WAITING_OTHER_THREADS_COMPLETED(ThreadHandle);
			}
		}
	}
}

bool SimplePakEditor::PakFileDeleteBucketByOSS(
	const FString &InAccessKeyID, 
	const FString &InAccessKeyIDSecret, 
	const FString &InEndPoint, 
	const FString &InBucketName,
	TArray<FString> &Error)
{
	SIMPLE_OSS.InitAccounts(InAccessKeyID, InAccessKeyIDSecret, InEndPoint);
	if (SIMPLE_OSS.DoesBucketExist(InBucketName))
	{
		Error.Add(FString::Printf(TEXT("The bucket [%s] exists."), *InBucketName));
		return SIMPLE_OSS.DeleteBucket(InBucketName);
	}
	Error.Add(FString::Printf(TEXT("The bucket [%s] doesn't exists."), *InBucketName));
	return false;
}

bool SimplePakEditor::PakFileDeleteObjectByOSS(
	const FString &InAccessKeyID, 
	const FString &InAccessKeyIDSecret, 
	const FString &InEndPoint, 
	const FString &InBucketName, 
	const TArray<FString> &InObjectName,
	TArray<FString> &Error)
{
	SIMPLE_OSS.InitAccounts(InAccessKeyID, InAccessKeyIDSecret, InEndPoint);
	if (SIMPLE_OSS.DoesBucketExist(InBucketName))
	{
		Error.Add(FString::Printf(TEXT("The bucket [%s] exists."), *InBucketName));
		return SIMPLE_OSS.DeleteObjects(InBucketName, InObjectName);
	}
	Error.Add(FString::Printf(TEXT("The bucket [%s] doesn't exists."), *InBucketName));
	return false;
}

void SimplePakEditor::PakFileUploadByOSS(
	const FString &InAccessKeyID, 
	const FString &InAccessKeyIDSecret, 
	const FString &InEndPoint, 
	const FString &InBucketName, 
	const FString &InLocalPaths,
	TArray<FString> &Error)
{
	PakFileUploadByOSS_Internal(
		InAccessKeyID, 
		InAccessKeyIDSecret, 
		InEndPoint, 
		InBucketName, 
		InLocalPaths,
		Error
		);
}

void SimplePakEditor::AsynchPakFileUploadByOSS(
	const FString &InAccessKeyID, 
	const FString &InAccessKeyIDSecret, 
	const FString &InEndPoint, 
	const FString &InBucketName, 
	const FString &InLocalPaths, 
	FPakEditorUploadDelegate PakEditorUploadDelegate)
{
	auto Lamada = [=]()
	{
		TArray<FString> Error;
		PakFileUploadByOSS_Internal(
			InAccessKeyID,
			InAccessKeyIDSecret,
			InEndPoint,
			InBucketName,
			InLocalPaths,
			Error,
			PakEditorUploadDelegate
		);
	};

	ASYNCTASK_Lambda(Lamada);
}

void PAKEDITOR_API SimplePakEditor::PakFileUploadByHTTP(
	const FString &InURL,
	const FString &InLocalPaths, 
	FSimpleHttpSingleRequestCompleteDelegate SimpleHttpRequestCompleteDelegate /*= FSimpleHttpSingleRequestCompleteDelegate()*/, 
	FSimpleHttpSingleRequestProgressDelegate SimpleHttpRequestProgressDelegate /*= FSimpleHttpSingleRequestProgressDelegate()*/, 
	FSimpleHttpSingleRequestHeaderReceivedDelegate SimpleHttpRequestHeaderReceivedDelegate /*= FSimpleHttpSingleRequestHeaderReceivedDelegate() */)
{
	FSimpleHttpActionMultpleRequest *NewRequest = new FSimpleHttpActionMultpleRequest();
	{
		NewRequest->SimpleHttpRequestCompleteDelegate = SimpleHttpRequestCompleteDelegate;
		NewRequest->SimpleHttpRequestHeaderReceivedDelegate = SimpleHttpRequestHeaderReceivedDelegate;
		NewRequest->SimpleHttpRequestProgressDelegate = SimpleHttpRequestProgressDelegate;
	}
	NewRequest->PutObject(InURL, InLocalPaths);
}

void PAKEDITOR_API SimplePakEditor::PakFileDeleteByHTTP(const FString &InURL, FSimpleHttpSingleRequestCompleteDelegate SimpleHttpRequestCompleteDelegate /*= FSimpleHttpSingleRequestCompleteDelegate()*/, FSimpleHttpSingleRequestProgressDelegate SimpleHttpRequestProgressDelegate /*= FSimpleHttpSingleRequestProgressDelegate()*/, FSimpleHttpSingleRequestHeaderReceivedDelegate SimpleHttpRequestHeaderReceivedDelegate /*= FSimpleHttpSingleRequestHeaderReceivedDelegate() */)
{
	FSimpleHttpActionRequest *NewRequest = new FSimpleHttpActionRequest();
	{
		NewRequest->SimpleHttpRequestCompleteDelegate = SimpleHttpRequestCompleteDelegate;
		NewRequest->SimpleHttpRequestHeaderReceivedDelegate = SimpleHttpRequestHeaderReceivedDelegate;
		NewRequest->SimpleHttpRequestProgressDelegate = SimpleHttpRequestProgressDelegate;
	}
	NewRequest->DeleteObject(InURL);
}
