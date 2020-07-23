//// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "HTTP/SimpleHttpActionRequest.h"
//#include "PakInfo.h"
//
//namespace SimplePakRuntime
//{
//	void PAKRUNTIME_API PakFileDownloadByHTTP(
//		const FString &InXmlURL,
//		const FString &InLocalPaths,
//		EEncryptionMode EncryptionMode = EEncryptionMode::NONE,
//		const FString &key = "",
//		FSimpleHttpSingleRequestCompleteDelegate SimpleHttpRequestCompleteDelegate = FSimpleHttpSingleRequestCompleteDelegate(),
//		FSimpleHttpSingleRequestProgressDelegate	SimpleHttpRequestProgressDelegate = FSimpleHttpSingleRequestProgressDelegate(),
//		FSimpleHttpSingleRequestHeaderReceivedDelegate SimpleHttpRequestHeaderReceivedDelegate = FSimpleHttpSingleRequestHeaderReceivedDelegate()
//	);
//}