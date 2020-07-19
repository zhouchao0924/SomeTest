#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
/**
 * 
 */
class SIMPLEHTTP_API FSimpleHttpManage
{
public:
	FSimpleHttpManage();
	~FSimpleHttpManage();

	static FSimpleHttpManage *Get();
	static void Destroy();

	bool GetObject(const FString &URL, const FString &SavePaths);
	bool PutObject(const FString &URL, TArray<uint8> &Data);
	bool PutObject(const FString &URL, const FString &LocalPaths);
	bool PutObject(const FString &URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream);
	bool DeleteObject(const FString &URL);

private:
	void HttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void HttpRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived);
	void HttpRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue);

	void Print(const FString &Msg, float Time = 10.f, FColor Color = FColor::Red);
private:
	static FSimpleHttpManage	*SimpleHttp;
	FString						TmpSavePaths;
};
