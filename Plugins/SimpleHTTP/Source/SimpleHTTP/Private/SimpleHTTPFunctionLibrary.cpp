// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleHTTPFunctionLibrary.h"
#include "SimpleHttpManage.h"
#include "HTTP/SimpleHttpActionRequest.h"
#include "HTTP/SimpleHTTPHandle.h"

bool USimpleHTTPFunctionLibrary::GetObjectToMemory(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL)
{
	return SIMPLE_HTTP.GetObjectToMemory(BPResponseDelegate, URL);
}

bool USimpleHTTPFunctionLibrary::GetObjectToLocal(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &SavePaths)
{
	return SIMPLE_HTTP.GetObjectToLocal(BPResponseDelegate, URL, SavePaths);
}

bool USimpleHTTPFunctionLibrary::PutObjectFromLocal(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &LocalPaths)
{
	return SIMPLE_HTTP.PutObjectFromLocal(BPResponseDelegate, URL, LocalPaths);
}

bool USimpleHTTPFunctionLibrary::PutObjectFromBuffer(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const TArray<uint8> &Buffer)
{
	return SIMPLE_HTTP.PutObjectFromBuffer(BPResponseDelegate, URL, Buffer);
}

bool USimpleHTTPFunctionLibrary::DeleteObject(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL)
{
	return SIMPLE_HTTP.DeleteObject(BPResponseDelegate, URL);
}

bool USimpleHTTPFunctionLibrary::PutObjectsFromLocal(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &LocalPaths)
{
	return SIMPLE_HTTP.PutObjectsFromLocal(BPResponseDelegate, URL,LocalPaths);
}

void USimpleHTTPFunctionLibrary::GetObjectsToLocal(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL, const FString &SavePaths)
{
	SIMPLE_HTTP.GetObjectsToLocal(BPResponseDelegate, URL, SavePaths);
}

void USimpleHTTPFunctionLibrary::GetObjectsToMemory(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL)
{
	SIMPLE_HTTP.GetObjectsToMemory(BPResponseDelegate, URL);
}

void USimpleHTTPFunctionLibrary::DeleteObjects(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL)
{
	SIMPLE_HTTP.DeleteObjects(BPResponseDelegate, URL);
}
