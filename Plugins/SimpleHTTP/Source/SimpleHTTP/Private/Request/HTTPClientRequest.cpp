#include "Request/HTTPClientRequest.h"
#include "Core/SimpleHttpMacro.h"

SimpleHTTP::HTTP::FPutObjectRequest::FPutObjectRequest(const FString &URL, const FString& ContentString)
{
	DEFINITION_HTTP_TYPE("PUT","application/x-www-form-urlencoded")
	HttpReuest->SetContentAsString(ContentString);
}

SimpleHTTP::HTTP::FPutObjectRequest::FPutObjectRequest(const FString &URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream)
{
	DEFINITION_HTTP_TYPE("PUT", "application/x-www-form-urlencoded")
	HttpReuest->SetContentFromStream(Stream);
}

SimpleHTTP::HTTP::FPutObjectRequest::FPutObjectRequest(const FString &URL,const TArray<uint8>& ContentPayload)
{
	DEFINITION_HTTP_TYPE("PUT", "application/x-www-form-urlencoded")
	HttpReuest->SetContent(ContentPayload);
}
//GET：从服务器获取资源
//POST：在服务器新建一个资源。
//PUT：在服务器更新资源 ->客户端提供改变后的完整资源
//PATCH:在服务器更新资源 ->客户端提供改变的属性
//DELETE:从服务器删除资源。
//HEAD：获取资源的元数据。
//OPTIONS：获取信息，关于资源的哪些属性是客户端可以改变的。
SimpleHTTP::HTTP::FGetObjectRequest::FGetObjectRequest(const FString &URL)
{
	DEFINITION_HTTP_TYPE("GET", "application/x-www-form-urlencoded")
}

SimpleHTTP::HTTP::FDeleteObjectsRequest::FDeleteObjectsRequest(const FString &URL)
{
	DEFINITION_HTTP_TYPE("DELETE", "application/x-www-form-urlencoded")
}
