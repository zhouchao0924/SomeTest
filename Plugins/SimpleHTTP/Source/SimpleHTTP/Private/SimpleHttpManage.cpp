#include "SimpleHTTPManage.h"
#include "HTTP/SimpleHttpActionMultpleRequest.h"
#include "Core/SimpleHttpMacro.h"
#include "Misc/FileHelper.h"

FSimpleHttpManage* FSimpleHttpManage::Instance = NULL;

TSharedPtr<FSimpleHttpActionRequest> GetHttpActionRequest(EHTTPRequestType RequestType)
{
	TSharedPtr<FSimpleHttpActionRequest> HttpObject = nullptr;
	switch (RequestType)
	{
		case EHTTPRequestType::SINGLE:
		{
			HttpObject = MakeShareable(new FSimpleHttpActionRequest());
			break;
		}
		case EHTTPRequestType::MULTPLE:
		{
			HttpObject = MakeShareable(new FSimpleHttpActionMultpleRequest());
			break;
		}
	}

	return HttpObject;
}

void FSimpleHttpManage::Tick(float DeltaTime)
{
	FScopeLock ScopeLock(&Instance->Mutex);

	TArray<FString> RemoveRequest;
	for (auto &Tmp : HTTP.HTTPMap)
	{
		if (Tmp.Value->IsRequestComplete())
		{
			RemoveRequest.Add(Tmp.Key);
		}
	}

	for (auto &Tmp : RemoveRequest)
	{
		GetHTTP().HTTPMap.Remove(Tmp);
	}
}

bool FSimpleHttpManage::IsTickableInEditor() const
{
	return true;
}

TStatId FSimpleHttpManage::GetStatId() const
{
	return TStatId();
}

FSimpleHttpManage * FSimpleHttpManage::Get()
{
	if (Instance == nullptr)
	{
		Instance = new FSimpleHttpManage();
	}

	return Instance;
}

void FSimpleHttpManage::Destroy()
{
	if (Instance != nullptr)
	{
		FScopeLock ScopeLock(&Instance->Mutex);
		delete Instance;		
	}

	Instance = nullptr;
}

FSimpleHTTPHandle FSimpleHttpManage::FHTTP::RegisteredHttpRequest(
	EHTTPRequestType RequestType ,
	FSimpleHttpSingleRequestCompleteDelegate SimpleHttpRequestCompleteDelegate /*= FSimpleHttpRequestCompleteDelegate()*/,
	FSimpleHttpSingleRequestProgressDelegate SimpleHttpRequestProgressDelegate /*= FSimpleHttpRequestProgressDelegate()*/, 
	FSimpleHttpSingleRequestHeaderReceivedDelegate SimpleHttpRequestHeaderReceivedDelegate /*= FSimpleHttpRequestHeaderReceivedDelegate()*/,
	FAllRequestCompleteDelegate AllRequestCompleteDelegate /*= FAllRequestCompleteDelegate()*/)
{
	FScopeLock ScopeLock(&Instance->Mutex);

	TSharedPtr<FSimpleHttpActionRequest> HttpObject = GetHttpActionRequest(RequestType);
	
	HttpObject->SimpleHttpRequestCompleteDelegate = SimpleHttpRequestCompleteDelegate;
	HttpObject->SimpleHttpRequestHeaderReceivedDelegate = SimpleHttpRequestHeaderReceivedDelegate;
	HttpObject->SimpleHttpRequestProgressDelegate = SimpleHttpRequestProgressDelegate;
	HttpObject->AllRequestCompleteDelegate = AllRequestCompleteDelegate;

	FSimpleHTTPHandle Key = FGuid::NewGuid().ToString();
	HTTPMap.Add(Key,HttpObject);

	return Key;
}

FSimpleHTTPHandle FSimpleHttpManage::FHTTP::RegisteredHttpRequest(
	EHTTPRequestType RequestType /*= EHTTPRequestType::SINGLE*/, 
	FSimpleSingleCompleteDelegate SimpleHttpRequestCompleteDelegate /*= nullptr*/, 
	FSimpleSingleRequestProgressDelegate SimpleHttpRequestProgressDelegate /*= nullptr*/,
	FSimpleSingleRequestHeaderReceivedDelegate SimpleHttpRequestHeaderReceivedDelegate /*= nullptr*/,
	FSimpleDelegate AllRequestCompleteDelegate /*= nullptr*/)
{
	FScopeLock ScopeLock(&Instance->Mutex);

	TSharedPtr<FSimpleHttpActionRequest> HttpObject = GetHttpActionRequest(RequestType);

	HttpObject->SimpleCompleteDelegate = SimpleHttpRequestCompleteDelegate;
	HttpObject->SimpleSingleRequestHeaderReceivedDelegate = SimpleHttpRequestHeaderReceivedDelegate;
	HttpObject->SimpleSingleRequestProgressDelegate = SimpleHttpRequestProgressDelegate;
	HttpObject->AllTasksCompletedDelegate = AllRequestCompleteDelegate;

	FSimpleHTTPHandle Key = FGuid::NewGuid().ToString();
	HTTPMap.Add(Key, HttpObject);

	return Key;
}

TWeakPtr<FSimpleHttpActionRequest> FSimpleHttpManage::FHTTP::Find(const FSimpleHTTPHandle &Handle)
{
	FScopeLock ScopeLock(&Instance->Mutex);

	TWeakPtr<FSimpleHttpActionRequest> Object = nullptr;
	for (auto &Tmp : Instance->GetHTTP().HTTPMap)
	{
		if (Tmp.Key == Handle)
		{
			Object = Tmp.Value;
			break;
		}
	}

	return Object;
}

bool FSimpleHttpManage::FHTTP::GetObjectToMemory(const FSimpleHTTPHandle &Handle, const FString &URL)
{
	TWeakPtr<FSimpleHttpActionRequest> Object = Find(Handle);
	if (Object.IsValid())
	{
		return Object.Pin()->GetObject(URL);
	}

	return false;
}

bool FSimpleHttpManage::FHTTP::GetObjectToMemory(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL)
{
	SIMPLE_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::SINGLE);

	return GetObjectToMemory(Handle, URL);
}

bool FSimpleHttpManage::FHTTP::GetObjectToMemory(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const FString &URL)
{
	SIMPLE_HTTP_REGISTERED_REQUEST(EHTTPRequestType::SINGLE);

	return GetObjectToMemory(Handle, URL);
}

void FSimpleHttpManage::FHTTP::GetObjectsToMemory(const FSimpleHTTPHandle &Handle, const TArray<FString> &URL)
{
	TWeakPtr<FSimpleHttpActionRequest> Object = Find(Handle);
	if (Object.IsValid())
	{
		Object.Pin()->GetObjects(URL);
	}
}

void FSimpleHttpManage::FHTTP::GetObjectsToMemory(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL)
{
	SIMPLE_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::MULTPLE);

	GetObjectsToMemory(Handle, URL);
}

void FSimpleHttpManage::FHTTP::GetObjectsToMemory(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL)
{
	SIMPLE_HTTP_REGISTERED_REQUEST(EHTTPRequestType::MULTPLE);

	GetObjectsToMemory(Handle, URL);
}

bool FSimpleHttpManage::FHTTP::GetObjectToLocal(const FSimpleHTTPHandle &Handle, const FString &URL, const FString &SavePaths)
{
	TWeakPtr<FSimpleHttpActionRequest> Object = Find(Handle);
	if (Object.IsValid())
	{
		return Object.Pin()->GetObject(URL, SavePaths);
	}

	return false;
}

bool FSimpleHttpManage::FHTTP::GetObjectToLocal(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &SavePaths)
{
	SIMPLE_HTTP_REGISTERED_REQUEST(EHTTPRequestType::SINGLE);

	return GetObjectToLocal(Handle, URL, SavePaths);
}

bool FSimpleHttpManage::FHTTP::GetObjectToLocal(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &SavePaths)
{
	SIMPLE_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::SINGLE);

	return GetObjectToLocal(Handle, URL, SavePaths);
}

void FSimpleHttpManage::FHTTP::GetObjectsToLocal(const FSimpleHTTPHandle &Handle, const TArray<FString> &URL, const FString &SavePaths)
{
	TWeakPtr<FSimpleHttpActionRequest> Object = Find(Handle);
	if (Object.IsValid())
	{
		Object.Pin()->GetObjects(URL, SavePaths);
	}
}

void FSimpleHttpManage::FHTTP::GetObjectsToLocal(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL, const FString &SavePaths)
{
	SIMPLE_HTTP_REGISTERED_REQUEST(EHTTPRequestType::MULTPLE);

	GetObjectsToLocal(Handle, URL, SavePaths);
}

void FSimpleHttpManage::FHTTP::GetObjectsToLocal(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL, const FString &SavePaths)
{
	SIMPLE_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::MULTPLE);

	GetObjectsToLocal(Handle, URL, SavePaths);
}

bool FSimpleHttpManage::FHTTP::PutObjectFromBuffer(const FSimpleHTTPHandle &Handle, const FString &URL, const TArray<uint8> &Data)
{
	TWeakPtr<FSimpleHttpActionRequest> Object = Find(Handle);
	if (Object.IsValid())
	{
		return Object.Pin()->PutObject(URL, Data);
	}

	return false;
}

bool FSimpleHttpManage::FHTTP::PutObjectFromBuffer(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const TArray<uint8> &Buffer)
{
	SIMPLE_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::SINGLE);

	return PutObjectFromBuffer(Handle, URL, Buffer);
}

bool FSimpleHttpManage::FHTTP::PutObjectFromBuffer(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const FString &URL, const TArray<uint8> &Buffer)
{
	SIMPLE_HTTP_REGISTERED_REQUEST(EHTTPRequestType::SINGLE);

	return PutObjectFromBuffer(Handle, URL, Buffer);
}

bool FSimpleHttpManage::FHTTP::PutObjectFromStream(const FSimpleHTTPHandle &Handle, const FString &URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream)
{
	TWeakPtr<FSimpleHttpActionRequest> Object = Find(Handle);
	if (Object.IsValid())
	{
		return Object.Pin()->PutObject(URL, Stream);
	}

	return false;
}

bool FSimpleHttpManage::FHTTP::PutObjectFromStream(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream)
{
	SIMPLE_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::SINGLE);

	return PutObjectFromStream(Handle, URL, Stream);
}

bool FSimpleHttpManage::FHTTP::PutObjectFromStream(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const FString &URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream)
{
	SIMPLE_HTTP_REGISTERED_REQUEST(EHTTPRequestType::SINGLE);

	return PutObjectFromStream(Handle, URL, Stream);
}

bool FSimpleHttpManage::FHTTP::PutObjectFromLocal(const FSimpleHTTPHandle &Handle, const FString &URL, const FString &LocalPaths)
{
	TWeakPtr<FSimpleHttpActionRequest> Object = Find(Handle);
	if (Object.IsValid())
	{
		return Object.Pin()->PutObject(URL, LocalPaths);
	}

	return false;
}

bool FSimpleHttpManage::FHTTP::PutObjectFromLocal(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &LocalPaths)
{
	SIMPLE_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::SINGLE);

	return PutObjectFromLocal(Handle, URL, LocalPaths);
}

bool FSimpleHttpManage::FHTTP::PutObjectFromLocal(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &LocalPaths)
{
	SIMPLE_HTTP_REGISTERED_REQUEST(EHTTPRequestType::SINGLE);

	return GetObjectToLocal(Handle, URL, LocalPaths);
}

bool FSimpleHttpManage::FHTTP::PutObjectsFromLocal(const FSimpleHTTPHandle &Handle, const FString &URL, const FString &LocalPaths)
{
	return PutObjectFromLocal(Handle, URL, LocalPaths);
}

bool FSimpleHttpManage::FHTTP::PutObjectsFromLocal(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &LocalPaths)
{
	SIMPLE_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::MULTPLE);

	return PutObjectsFromLocal(Handle, URL, LocalPaths);
}

bool FSimpleHttpManage::FHTTP::PutObjectsFromLocal(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &LocalPaths)
{
	SIMPLE_HTTP_REGISTERED_REQUEST(EHTTPRequestType::MULTPLE);

	return PutObjectsFromLocal(Handle, URL, LocalPaths);
}

bool FSimpleHttpManage::FHTTP::DeleteObject(const FSimpleHTTPHandle &Handle, const FString &URL)
{
	TWeakPtr<FSimpleHttpActionRequest> Object = Find(Handle);
	if (Object.IsValid())
	{
		return Object.Pin()->DeleteObject(URL);
	}

	return false;
}

bool FSimpleHttpManage::FHTTP::DeleteObject(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL)
{
	SIMPLE_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::SINGLE);

	return DeleteObject(Handle, URL);
}

bool FSimpleHttpManage::FHTTP::DeleteObject(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const FString &URL)
{
	SIMPLE_HTTP_REGISTERED_REQUEST(EHTTPRequestType::SINGLE);

	return DeleteObject(Handle, URL);
}

void FSimpleHttpManage::FHTTP::DeleteObjects(const FSimpleHTTPHandle &Handle, const TArray<FString> &URL)
{
	TWeakPtr<FSimpleHttpActionRequest> Object = Find(Handle);
	if (Object.IsValid())
	{
		Object.Pin()->DeleteObjects(URL);
	}
}

void FSimpleHttpManage::FHTTP::DeleteObjects(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL)
{
	SIMPLE_HTTP_REGISTERED_REQUEST_BP(EHTTPRequestType::MULTPLE);

	DeleteObjects(Handle, URL);
}

void FSimpleHttpManage::FHTTP::DeleteObjects(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL)
{
	SIMPLE_HTTP_REGISTERED_REQUEST(EHTTPRequestType::MULTPLE);

	DeleteObjects(Handle, URL);
}

TArray<uint8> & USimpleHttpContent::GetContent()
{
	return *Content;
}

bool USimpleHttpContent::Save(const FString &LocalPath)
{
	return FFileHelper::SaveArrayToFile(*Content, *LocalPath);
}