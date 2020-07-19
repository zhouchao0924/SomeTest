#define DEFINITION_HTTP_TYPE(VerbString,Content) \
HttpReuest->SetURL(URL);\
HttpReuest->SetVerb(TEXT(##VerbString));\
HttpReuest->SetHeader(TEXT("Content-Type"), TEXT(##Content)); 

#define REQUEST_BIND_FUN \
Request \
<< FHttpRequestHeaderReceivedDelegate::CreateRaw(this, &FSimpleHttpManage::HttpRequestHeaderReceived)\
<< FHttpRequestProgressDelegate::CreateRaw(this, &FSimpleHttpManage::HttpRequestProgress)\
<< FHttpRequestCompleteDelegate::CreateRaw(this, &FSimpleHttpManage::HttpRequestComplete);