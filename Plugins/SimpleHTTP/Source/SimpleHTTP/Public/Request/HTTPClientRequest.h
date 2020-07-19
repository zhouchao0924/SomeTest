#pragma once

#include "CoreMinimal.h"
#include "Request/RequestInterface.h"

namespace SimpleHTTP
{
	namespace HTTP
	{
		struct SIMPLEHTTP_API FPutObjectRequest :IHTTPClientRequest
		{
			FPutObjectRequest(const FString &URL, const FString& ContentString);
			FPutObjectRequest(const FString &URL, TArray<uint8>& ContentPayload);
			FPutObjectRequest(const FString &URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream);
		};

		struct SIMPLEHTTP_API FGetObjectRequest :IHTTPClientRequest
		{
			FGetObjectRequest(const FString &URL);
		};

		struct SIMPLEHTTP_API FDeleteObjectsRequest :IHTTPClientRequest
		{
			FDeleteObjectsRequest(const FString &URL);
		};
	}
}