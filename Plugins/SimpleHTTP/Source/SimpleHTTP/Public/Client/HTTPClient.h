#pragma once

#include "CoreMinimal.h"
#include "Request/HTTPClientRequest.h"

using namespace SimpleHTTP::HTTP;

namespace SimpleHTTP
{
	namespace HTTP
	{
		struct SIMPLEHTTP_API FHTTPClient
		{
			FHTTPClient();

			bool GetObject(FGetObjectRequest &Request) const;
			bool DeleteObject(FDeleteObjectsRequest &Request)const;
			bool PutObject(FPutObjectRequest &Request) const;

			void AbortRequest(IHTTPClientRequest &Request);
		};
	}
}