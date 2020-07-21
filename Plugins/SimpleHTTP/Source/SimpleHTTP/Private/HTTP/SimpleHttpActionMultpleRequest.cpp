#include "HTTP/SimpleHttpActionMultpleRequest.h"
#include "Client/HTTPClient.h"
#include "Core/SimpleHttpMacro.h"
#include "HAL/FileManager.h"

FSimpleHttpActionMultpleRequest::FSimpleHttpActionMultpleRequest()
	:FSimpleHttpActionRequest()
	,RequestNumber(0)
{

}

void FSimpleHttpActionMultpleRequest::GetObjects(const TArray<FString> &URL, const FString &SavePaths)
{
	SetPaths(SavePaths);

	for (const auto &Tmp : URL)
	{
		FHTTPClient Client;

		FGetObjectRequest Request(Tmp);

		REQUEST_BIND_FUN(FSimpleHttpActionMultpleRequest)

		if (Client.GetObject(Request))
		{
			RequestNumber++;
		}
	}
}

void FSimpleHttpActionMultpleRequest::GetObjects(const TArray<FString> &URL)
{
	bSaveDisk = false;

	for (const auto &Tmp : URL)
	{
		FHTTPClient Client;

		FGetObjectRequest Request(Tmp);

		REQUEST_BIND_FUN(FSimpleHttpActionMultpleRequest)

		if (Client.GetObject(Request))
		{
			RequestNumber++;
		}
	}
}

void FSimpleHttpActionMultpleRequest::DeleteObjects(const TArray<FString> &URL)
{
	for (const auto &Tmp : URL)
	{
		FHTTPClient Client;

		FDeleteObjectsRequest Request(Tmp);

		REQUEST_BIND_FUN(FSimpleHttpActionMultpleRequest)

		if (Client.DeleteObject(Request))
		{
			RequestNumber++;
		}
	}
}

bool FSimpleHttpActionMultpleRequest::PutObject(const FString &URL, const FString &LocalPaths)
{
	SetPaths(LocalPaths);

	//开始过滤路径
	TArray<FString> AllPaths;
	IFileManager::Get().FindFilesRecursive(AllPaths, *LocalPaths, TEXT("*"), true, true);

	for (const auto &Tmp: AllPaths)
	{
		TArray<uint8> ByteData;
		FFileHelper::LoadFileToArray(ByteData, *Tmp);

		FString ObjectName = FPaths::GetCleanFilename(Tmp);

		FHTTPClient Client;

		FPutObjectRequest Request(URL / ObjectName, ByteData);

		REQUEST_BIND_FUN(FSimpleHttpActionMultpleRequest)

		if (Client.PutObject(Request))
		{
			RequestNumber++;
		}
	}

	return RequestNumber > 0;
}

void FSimpleHttpActionMultpleRequest::HttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (Request.IsValid() &&
		Response.IsValid() &&
		bConnectedSuccessfully &&
		EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		if (Request->GetVerb() == "GET")
		{
			FString Filename = FPaths::GetCleanFilename(Request->GetURL());
			FFileHelper::SaveArrayToFile(Response->GetContent(), *(GetPaths() / Filename));
		}

		FSimpleHttpRequest SimpleHttpRequest;
		FSimpleHttpResponse SimpleHttpResponse;
		ResponsePtrToSimpleResponse(Response, SimpleHttpResponse);
		RequestPtrToSimpleRequest(Request, SimpleHttpRequest);

		SimpleHttpRequestCompleteDelegate.ExecuteIfBound(SimpleHttpRequest, SimpleHttpResponse, bConnectedSuccessfully);
		SimpleCompleteDelegate.ExecuteIfBound(SimpleHttpRequest, SimpleHttpResponse, bConnectedSuccessfully);
	}
	else
	{
		FSimpleHttpRequest SimpleHttpRequest;
		FSimpleHttpResponse SimpleHttpResponse;
		ResponsePtrToSimpleResponse(Response, SimpleHttpResponse);
		RequestPtrToSimpleRequest(Request, SimpleHttpRequest);

		SimpleHttpRequestCompleteDelegate.ExecuteIfBound(SimpleHttpRequest, SimpleHttpResponse, bConnectedSuccessfully);
		SimpleCompleteDelegate.ExecuteIfBound(SimpleHttpRequest, SimpleHttpResponse, bConnectedSuccessfully);
	}

	if (RequestNumber > 0)
	{
		RequestNumber--;
		if (RequestNumber <= 0)
		{
			AllRequestCompleteDelegate.ExecuteIfBound();
			AllTasksCompletedDelegate.ExecuteIfBound();

			bRequestComplete = true;
		}
	}
	else
	{
		check(0);
	}
}

void FSimpleHttpActionMultpleRequest::HttpRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
	FSimpleHttpActionRequest::HttpRequestProgress(Request, BytesSent, BytesReceived);
}

void FSimpleHttpActionMultpleRequest::HttpRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue)
{
	FSimpleHttpActionRequest::HttpRequestHeaderReceived(Request, HeaderName, NewHeaderValue);
}

