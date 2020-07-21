// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleHTTPType.h"
#include "SimpleHTTPFunctionLibrary.generated.h"

/**
 * A simple set of HTTP interface functions can quickly perform HTTP code operations.
 * Only one interface is needed to interact with our HTTP server. Currently,
 * HTTP supports downloading, uploading, deleting and other operations.
 * See our API for details
*/
UCLASS()
class SIMPLEHTTP_API USimpleHTTPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
	* The data can be downloaded to local memory via the HTTP serverll .
	*
	* @param BPResponseDelegate	Proxy set relative to the blueprint.
	* @param URL					domain name .
	* @Return						Returns true if the request succeeds
	*/
	UFUNCTION(BlueprintCallable, Category = "SimpleHTTP|SingleAction")
	static bool GetObjectToMemory(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL);

	/**
	 * Download individual data locally.
	 *
	 * @param BPResponseDelegate	Proxy set relative to the blueprint.
	 * @param URL					domain name .
	 * @param SavePaths				Path to local storage .
	 * @Return						Returns true if the request succeeds
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleHTTP|SingleAction")
	static bool GetObjectToLocal(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &SavePaths);

	/**
	 * Upload single file from disk to server .
	 *
	 * @param BPResponseDelegate	Proxy set relative to the blueprint.
	 * @param URL					domain name .
	 * @param LocalPaths			Specify the Path where you want to upload the file.
	 * @Return						Returns true if the request succeeds
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleHTTP|SingleAction")
	static bool PutObjectFromLocal(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &LocalPaths);
	
	/**
	 * Can upload byte data .
	 *
	 * @param BPResponseDelegate	Proxy set relative to the blueprint.
	 * @param URL					domain name .
	 * @param Buffer				Byte code data.
	 * @Return						Returns true if the request succeeds
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleHTTP|SingleAction")
	static bool PutObjectFromBuffer(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const TArray<uint8> &Buffer);

	/**
	 * Remove a single object from the server .
	 *
	 * @param BPResponseDelegate	Proxy set relative to the blueprint.
	 * @param URL					domain name .
	 * @Return						Returns true if the request succeeds
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleHTTP|SingleAction")
	static bool DeleteObject(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL);

	/**
	 * Upload duo files from disk to server  .
	 *
	 * @param BPResponseDelegate	Proxy set relative to the blueprint.
	 * @param URL					domain name .
	 * @param LocalPaths			Specify the Path where you want to upload the file.
	 * @Return						Returns true if the request succeeds
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleHTTP|MultpleAction")
	static bool PutObjectsFromLocal(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &LocalPaths);
	
	/**
	 * Download multiple data to local .
	 *
	 * @param BPResponseDelegate	Proxy set relative to the blueprint.
	 * @param URL					Need domain name .
	 * @param SavePaths				Path to local storage .
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleHTTP|MultpleAction")
	static void GetObjectsToLocal(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL, const FString &SavePaths);
	
	/**
	 * The data can be downloaded to local memory via the HTTP serverll.
	 * Can download multiple at one time .
	 *
	 * @param BPResponseDelegate	Proxy set relative to the blueprint.
	 * @param URL					Need domain name .
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleHTTP|MultpleAction")
	static void GetObjectsToMemory(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL);
	
	/**
	 * Multiple URLs need to be specified to remove multiple objects from the server .
	 *
	 * @param BPResponseDelegate	Proxy set relative to the blueprint.
	 * @param URL					Need domain name .
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleHTTP|MultpleAction")
	static void DeleteObjects(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL);
};
