
#pragma once

#include "CoreMinimal.h"
#include "HTTP/SimpleHttpActionRequest.h"

struct SIMPLEHTTP_API FSimpleHttpActionMultpleRequest : public FSimpleHttpActionRequest
{
public:
	FSimpleHttpActionMultpleRequest();

	virtual void GetObjects(const TArray<FString> &URL, const FString &SavePaths);
	virtual void GetObjects(const TArray<FString> &URL);
	virtual void DeleteObjects(const TArray<FString> &URL);
	virtual bool PutObject(const FString &URL, const FString &LocalPaths);

protected:
	virtual void HttpRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	virtual void HttpRequestProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived);
	virtual void HttpRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue);

private:
	uint32 RequestNumber;
};