// Fill out your copyright notice in the Description page of Project Settings.

#include "HTTP/SimpleHttpActionRequest.h"
#include "Client/HTTPClient.h"
#include "Core/SimpleHttpMacro.h"
#include "HAL/FileManager.h"

FSimpleHttpActionRequest::FSimpleHttpActionRequest()
	:bRequestComplete(false)
	,bSaveDisk(true)
{
}

FSimpleHttpActionRequest::~FSimpleHttpActionRequest()
{

}

void FSimpleHttpActionRequest::GetObjects(const TArray<FString> &URL, const FString &SavePaths)
{

}

void FSimpleHttpActionRequest::GetObjects(const TArray<FString> &URL)
{

}

void FSimpleHttpActionRequest::DeleteObjects(const TArray<FString> &URL)
{

}

void FSimpleHttpActionRequest::HttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	//404 405 100 -199 200 -299
	if (Request.IsValid() && 
		Response.IsValid() && 
		bConnectedSuccessfully && 
		EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{	
		if (Request->GetVerb() == "GET")
		{
			if (bSaveDisk)
			{
				FString Filename = FPaths::GetCleanFilename(Request->GetURL());
				FFileHelper::SaveArrayToFile(Response->GetContent(), *(TmpSavePaths / Filename));
			}
		}

		FSimpleHttpRequest SimpleHttpRequest;		
		FSimpleHttpResponse SimpleHttpResponse;
		ResponsePtrToSimpleResponse(Response, SimpleHttpResponse);
		RequestPtrToSimpleRequest(Request, SimpleHttpRequest);

		SimpleHttpRequestCompleteDelegate.ExecuteIfBound(SimpleHttpRequest, SimpleHttpResponse,bConnectedSuccessfully);
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

	AllRequestCompleteDelegate.ExecuteIfBound();
	AllTasksCompletedDelegate.ExecuteIfBound();

	bRequestComplete = true;
}

void FSimpleHttpActionRequest::HttpRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
	FSimpleHttpRequest SimpleHttpRequest;
	RequestPtrToSimpleRequest(Request, SimpleHttpRequest);

	SimpleHttpRequestProgressDelegate.ExecuteIfBound(SimpleHttpRequest, BytesSent, BytesReceived);
	SimpleSingleRequestProgressDelegate.ExecuteIfBound(SimpleHttpRequest, BytesSent, BytesReceived);
}

void FSimpleHttpActionRequest::HttpRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue)
{
	FSimpleHttpRequest SimpleHttpRequest;
	RequestPtrToSimpleRequest(Request, SimpleHttpRequest);

	SimpleHttpRequestHeaderReceivedDelegate.ExecuteIfBound(SimpleHttpRequest, HeaderName, NewHeaderValue);
	SimpleSingleRequestHeaderReceivedDelegate.ExecuteIfBound(SimpleHttpRequest, HeaderName, NewHeaderValue);
}

void FSimpleHttpActionRequest::Print(const FString &Msg, float Time /*= 10.f*/, FColor Color /*= FColor::Red*/)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Time, Color, Msg);
	}
}

bool FSimpleHttpActionRequest::GetObject(const FString &URL, const FString &SavePaths)
{
	TmpSavePaths = SavePaths;

	FHTTPClient Client;

	FGetObjectRequest Request(URL);

	REQUEST_BIND_FUN(FSimpleHttpActionRequest)

	return Client.GetObject(Request);
}

bool FSimpleHttpActionRequest::GetObject(const FString &URL)
{
	bSaveDisk = false;

	FHTTPClient Client;

	FGetObjectRequest Request(URL);

	REQUEST_BIND_FUN(FSimpleHttpActionRequest)

	return Client.GetObject(Request);
}

bool FSimpleHttpActionRequest::PutObject(const FString &URL,const TArray<uint8> &Data)
{
	FHTTPClient Client;

	FPutObjectRequest Request(URL, Data);

	REQUEST_BIND_FUN(FSimpleHttpActionRequest)

	return Client.PutObject(Request);
}

bool FSimpleHttpActionRequest::PutObject(const FString &URL, const FString &LocalPaths)
{
	TArray<uint8> Data;
	if (FFileHelper::LoadFileToArray(Data, *LocalPaths))
	{
		FHTTPClient Client;
		FPutObjectRequest Request(URL, Data);

		REQUEST_BIND_FUN(FSimpleHttpActionRequest)

		return Client.PutObject(Request);
	}

	return false;
}

bool FSimpleHttpActionRequest::PutObject(const FString &URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream)
{
	FHTTPClient Client;

	FPutObjectRequest Request(URL, Stream);

	REQUEST_BIND_FUN(FSimpleHttpActionRequest)

	return Client.PutObject(Request);
}

bool FSimpleHttpActionRequest::DeleteObject(const FString &URL)
{
	FHTTPClient Client;

	FDeleteObjectsRequest Request(URL);

	return Client.DeleteObject(Request);
}
