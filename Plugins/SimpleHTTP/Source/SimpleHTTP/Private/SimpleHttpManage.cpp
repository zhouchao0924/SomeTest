// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleHttpManage.h"
#include "Client/HTTPClient.h"
#include "Core/SimpleHttpMacro.h"

FSimpleHttpManage *FSimpleHttpManage::SimpleHttp = nullptr;

FSimpleHttpManage::FSimpleHttpManage()
{
}

FSimpleHttpManage::~FSimpleHttpManage()
{
}

FSimpleHttpManage * FSimpleHttpManage::Get()
{
	if (!SimpleHttp)
	{
		SimpleHttp = new FSimpleHttpManage();
	}

	return SimpleHttp;
}

void FSimpleHttpManage::Destroy()
{
	if (SimpleHttp)
	{
		delete SimpleHttp;
	}

	SimpleHttp = nullptr;
}

void FSimpleHttpManage::HttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	//404 405 100 -199 200 -299
	if (Request.IsValid() && 
		Response.IsValid() && 
		bConnectedSuccessfully && 
		EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{	
		if (Request->GetVerb() == "GET")
		{
			FString Filename = FPaths::GetCleanFilename(Request->GetURL());
			FFileHelper::SaveArrayToFile(Response->GetContent(), *(Get()->TmpSavePaths / Filename));
		}
		else if (Request->GetVerb() == "PUT")
		{

		}
		else if (Request->GetVerb() == "DELETE")
		{

		}
	}
}

void FSimpleHttpManage::HttpRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{

}

void FSimpleHttpManage::HttpRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue)
{

}

void FSimpleHttpManage::Print(const FString &Msg, float Time /*= 10.f*/, FColor Color /*= FColor::Red*/)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Time, Color, Msg);
	}
}

bool FSimpleHttpManage::GetObject(const FString &URL, const FString &SavePaths)
{
	TmpSavePaths = SavePaths;

	FHTTPClient Client;

	FGetObjectRequest Request(URL);

	REQUEST_BIND_FUN

	return Client.GetObject(Request);
}

bool FSimpleHttpManage::PutObject(const FString &URL, TArray<uint8> &Data)
{
	FHTTPClient Client;

	FPutObjectRequest Request(URL, Data);

	REQUEST_BIND_FUN

	return Client.PutObject(Request);
}

bool FSimpleHttpManage::PutObject(const FString &URL, const FString &LocalPaths)
{
	TArray<uint8> Data;
	if (FFileHelper::LoadFileToArray(Data, *LocalPaths))
	{
		FHTTPClient Client;
		FPutObjectRequest Request(URL, Data);

		REQUEST_BIND_FUN

		return Client.PutObject(Request);
	}

	return false;
}

bool FSimpleHttpManage::PutObject(const FString &URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream)
{
	FHTTPClient Client;

	FPutObjectRequest Request(URL, Stream);

	REQUEST_BIND_FUN

	return Client.PutObject(Request);
}

bool FSimpleHttpManage::DeleteObject(const FString &URL)
{
	FHTTPClient Client;

	FDeleteObjectsRequest Request(URL);

	return Client.DeleteObject(Request);
}
