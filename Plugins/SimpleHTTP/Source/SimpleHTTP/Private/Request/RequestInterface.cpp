#include "Request/RequestInterface.h"
#include "HttpModule.h"

SimpleHTTP::HTTP::IHTTPClientRequest::IHTTPClientRequest()
	:HttpReuest(FHttpModule::Get().CreateRequest())
{

}

bool SimpleHTTP::HTTP::IHTTPClientRequest::ProcessRequest()
{
	return HttpReuest->ProcessRequest();
}

void SimpleHTTP::HTTP::IHTTPClientRequest::CancelRequest()
{
	HttpReuest->CancelRequest();
}

