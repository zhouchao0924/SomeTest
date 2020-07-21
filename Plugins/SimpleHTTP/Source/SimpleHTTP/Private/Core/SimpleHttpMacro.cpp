#include "Core/SimpleHttpMacro.h"

void RequestPtrToSimpleRequest(FHttpRequestPtr Request, FSimpleHttpRequest &SimpleHttpRequest)
{
	if (Request.IsValid())
	{
		SimpleHttpRequest.Verb = Request->GetVerb();
		SimpleHttpRequest.URL = Request->GetURL();
		SimpleHttpRequest.Status = (FSimpleHttpStarte)Request->GetStatus();
		SimpleHttpRequest.ElapsedTime = Request->GetElapsedTime();
		SimpleHttpRequest.ContentType = Request->GetContentType();
		SimpleHttpRequest.ContentLength = Request->GetContentLength();
		SimpleHttpRequest.AllHeaders = Request->GetAllHeaders();
	}
}

void ResponsePtrToSimpleResponse(FHttpResponsePtr Response, FSimpleHttpResponse &SimpleHttpResponse)
{
	if (Response.IsValid())
	{
		SimpleHttpResponse.ResponseCode = Response->GetResponseCode();
		SimpleHttpResponse.URL = Response->GetURL();
		SimpleHttpResponse.ResponseMessage = Response->GetContentAsString();
		SimpleHttpResponse.ContentType = Response->GetContentType();
		SimpleHttpResponse.ContentLength = Response->GetContentLength();
		SimpleHttpResponse.AllHeaders = Response->GetAllHeaders();
		SimpleHttpResponse.Content->Content = const_cast<TArray<uint8>*>(&Response->GetContent());
	}
}