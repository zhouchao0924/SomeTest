// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HTTP/SimpleHttpActionRequest.h"
#include "PakFileUpload.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FPakEditorUploadDelegate, int32, InSurplus, int64, InTotal, const FString&, Mess);

USTRUCT()
struct FSimplePakEditor
{
	GENERATED_BODY()
};

namespace SimplePakEditor
{
	bool PAKEDITOR_API PakFileDeleteBucketByOSS(
		const FString &InAccessKeyID,
		const FString &InAccessKeyIDSecret,
		const FString &InEndPoint,
		const FString &InBucketName,
		TArray<FString> &Error
	);

	bool PAKEDITOR_API PakFileDeleteObjectByOSS(
		const FString &InAccessKeyID,
		const FString &InAccessKeyIDSecret,
		const FString &InEndPoint,
		const FString &InBucketName,
		const TArray<FString> &InObjectName,
		TArray<FString> &Error
	);

	void PAKEDITOR_API PakFileUploadByOSS(
		const FString &InAccessKeyID,
		const FString &InAccessKeyIDSecret,
		const FString &InEndPoint,
		const FString &InBucketName,
		const FString &InLocalPaths,
		TArray<FString> &Error
	);

	void PAKEDITOR_API AsynchPakFileUploadByOSS(
		const FString &InAccessKeyID,
		const FString &InAccessKeyIDSecret,
		const FString &InEndPoint,
		const FString &InBucketName,
		const FString &InLocalPaths,
		FPakEditorUploadDelegate PakEditorUploadDelegate
	);

	void PAKEDITOR_API PakFileUploadByHTTP(
		const FString &InURL,
		const FString &InLocalPaths,
		FSimpleHttpSingleRequestCompleteDelegate SimpleHttpRequestCompleteDelegate = FSimpleHttpSingleRequestCompleteDelegate(),
		FSimpleHttpSingleRequestProgressDelegate	SimpleHttpRequestProgressDelegate = FSimpleHttpSingleRequestProgressDelegate(),
		FSimpleHttpSingleRequestHeaderReceivedDelegate SimpleHttpRequestHeaderReceivedDelegate = FSimpleHttpSingleRequestHeaderReceivedDelegate()
	);

	void PAKEDITOR_API PakFileDeleteByHTTP(
		const FString &InURL,
		FSimpleHttpSingleRequestCompleteDelegate SimpleHttpRequestCompleteDelegate = FSimpleHttpSingleRequestCompleteDelegate(),
		FSimpleHttpSingleRequestProgressDelegate	SimpleHttpRequestProgressDelegate = FSimpleHttpSingleRequestProgressDelegate(),
		FSimpleHttpSingleRequestHeaderReceivedDelegate SimpleHttpRequestHeaderReceivedDelegate = FSimpleHttpSingleRequestHeaderReceivedDelegate()
	);
}