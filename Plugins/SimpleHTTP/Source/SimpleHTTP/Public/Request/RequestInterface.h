#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"


namespace SimpleHTTP
{
	namespace HTTP
	{
		struct SIMPLEHTTP_API IHTTPClientRequest
		{
			friend struct FHTTPClient;

			IHTTPClientRequest();

			IHTTPClientRequest &operator<<(const FHttpRequestCompleteDelegate& SimpleDelegateInstance)
			{
				HttpReuest->OnProcessRequestComplete() = SimpleDelegateInstance;
				return *this;
			}

			IHTTPClientRequest &operator<<(const FHttpRequestProgressDelegate& SimpleDelegateInstance)
			{			
				HttpReuest->OnRequestProgress() = SimpleDelegateInstance;
				return *this;
			}

			IHTTPClientRequest &operator<<(const FHttpRequestHeaderReceivedDelegate& SimpleDelegateInstance)
			{
				HttpReuest->OnHeaderReceived() = SimpleDelegateInstance;
				return *this;
			}

		protected:
			bool ProcessRequest();
			void CancelRequest();

		protected:
			TSharedPtr<class IHttpRequest> HttpReuest;
		};
	}
}