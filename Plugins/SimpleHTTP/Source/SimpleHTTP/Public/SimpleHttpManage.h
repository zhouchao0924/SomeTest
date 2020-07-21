
#pragma once

#include "CoreMinimal.h"
#include "HTTP/SimpleHTTPHandle.h"
#include "SimpleHTTPType.h"
#include "Tickable.h"

struct FSimpleHttpActionRequest;

/*
 * A simple set of HTTP interface functions can quickly perform HTTP code operations. 
 * Only one interface is needed to interact with our HTTP server. Currently, 
 * HTTP supports downloading, uploading, deleting and other operations. 
 * See our API for details
*/
class SIMPLEHTTP_API FSimpleHttpManage :public FTickableGameObject
{
	/**
	 * GC
	 * Pure virtual that must be overloaded by the inheriting class. It will
	 * be called from within LevelTick.cpp after ticking all actors or from
	 * the rendering thread (depending on bIsRenderingThreadObject)
	 *
	 * @param DeltaTime	Game time passed since the last call.
	 */
	virtual void Tick(float DeltaTime);

	/**
	 * Used to determine whether the object should be ticked in the editor.  Defaults to false since
	 * that is the previous behavior.
	 *
	 * @return	true if this tickable object can be ticked in the editor
	 */
	virtual bool IsTickableInEditor() const;

	/** return the stat id to use for this tickable **/
	virtual TStatId GetStatId() const;

	/** Get HTTP function collection  **/
	struct SIMPLEHTTP_API FHTTP
	{
		/*We want to have direct access to the commands in http .*/
		friend class FSimpleHttpManage;

		/**
		 * The data can be downloaded to local memory via the HTTP serverll .
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					domain name .
		 * @Return						Returns true if the request succeeds 
		 */
		bool GetObjectToMemory(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL);
		
		/**
		 * The data can be downloaded to local memory via the HTTP serverll.
		 * Can download multiple at one time .
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					Need domain name .
		 */
		void GetObjectsToMemory(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL);

		/**
		 * Download individual data locally.
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					domain name .
		 * @param SavePaths				Path to local storage .
		 * @Return						Returns true if the request succeeds 
		 */
		bool GetObjectToLocal(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &SavePaths);
		
		/**
		 * Download multiple data to local .
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					Need domain name .
		 * @param SavePaths				Path to local storage .
		 */
		void GetObjectsToLocal(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL, const FString &SavePaths);

		/**
		 * Upload single file from disk to server .
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					domain name .
		 * @param LocalPaths			Specify the Path where you want to upload the file.
		 * @Return						Returns true if the request succeeds
		 */
		bool PutObjectFromLocal(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &LocalPaths);
		
		/**
		 * Upload duo files from disk to server  .
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					domain name .
		 * @param LocalPaths			Specify the Path where you want to upload the file.
		 * @Return						Returns true if the request succeeds
		 */
		bool PutObjectsFromLocal(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &LocalPaths);

		/**
		 * Can upload byte data .
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					domain name .
		 * @param Buffer				Byte code data.
		 * @Return						Returns true if the request succeeds
		 */
		bool PutObjectFromBuffer(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, const TArray<uint8> &Buffer);
		
		/**
		 * Stream data upload supported by UE4 .
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					domain name .
		 * @param Stream				UE4 storage structure .
		 * @Return						Returns true if the request succeeds
		 */
		bool PutObjectFromStream(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream);

		/**
		 * Remove a single object from the server .
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					domain name .
		 * @Return						Returns true if the request succeeds
		 */
		bool DeleteObject(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const FString &URL);
		
		/**
		 * Multiple URLs need to be specified to remove multiple objects from the server .
		 *
		 * @param BPResponseDelegate	Proxy set relative to the blueprint.
		 * @param URL					Need domain name .
		 */
		void DeleteObjects(const FSimpleHTTPBPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL);
		
		//////////////////////////////////////////////////////////////////////////

		/**
		 * The data can be downloaded to local memory via the HTTP serverll .
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					domain name .
		 * @Return						Returns true if the request succeeds
		 */
		bool GetObjectToMemory(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const FString &URL);

		/**
		 * The data can be downloaded to local memory via the HTTP serverll.
		 * Can download multiple at one time .
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					Need domain name .
		 */
		void GetObjectsToMemory(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL);

		/**
		 * Download individual data locally.
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					domain name .
		 * @param SavePaths				Path to local storage .
		 * @Return						Returns true if the request succeeds
		 */
		bool GetObjectToLocal(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &SavePaths);

		/**
		 * Download multiple data to local .
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					Need domain name .
		 * @param SavePaths				Path to local storage .
		 */
		void GetObjectsToLocal(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL, const FString &SavePaths);

		/**
		 * Upload single file from disk to server .
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					domain name .
		 * @param LocalPaths			Specify the Path where you want to upload the file.
		 * @Return						Returns true if the request succeeds
		 */
		bool PutObjectFromLocal(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &LocalPaths);

		/**
		 * Upload duo files from disk to server  .
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					domain name .
		 * @param LocalPaths			Specify the Path where you want to upload the file.
		 * @Return						Returns true if the request succeeds
		 */
		bool PutObjectsFromLocal(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const FString &URL, const FString &LocalPaths);

		/**
		 * Can upload byte data .
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					domain name .
		 * @param Buffer				Byte code data.
		 * @Return						Returns true if the request succeeds
		 */
		bool PutObjectFromBuffer(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const FString &URL, const TArray<uint8> &Buffer);

		/**
		 * Stream data upload supported by UE4 .
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					domain name .
		 * @param Stream				UE4 storage structure .
		 * @Return						Returns true if the request succeeds
		 */
		bool PutObjectFromStream(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const FString &URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream);

		/**
		 * Remove a single object from the server .
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					domain name .
		 * @Return						Returns true if the request succeeds
		 */
		bool DeleteObject(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const FString &URL);

		/**
		 * Multiple URLs need to be specified to remove multiple objects from the server .
		 *
		 * @param BPResponseDelegate	C + + based proxy interface .
		 * @param URL					Need domain name .
		 */
		void DeleteObjects(const FSimpleHTTPResponseDelegate &BPResponseDelegate, const TArray<FString> &URL);

	private:

		/**
		 * Register our agent BP for internal use .
		 *
		 * @param return	Use handle find PRequest.
		 */
		FSimpleHTTPHandle RegisteredHttpRequest(EHTTPRequestType RequestType = EHTTPRequestType::SINGLE,
			FSimpleHttpSingleRequestCompleteDelegate SimpleHttpRequestCompleteDelegate = FSimpleHttpSingleRequestCompleteDelegate(),
			FSimpleHttpSingleRequestProgressDelegate	SimpleHttpRequestProgressDelegate = FSimpleHttpSingleRequestProgressDelegate(),
			FSimpleHttpSingleRequestHeaderReceivedDelegate SimpleHttpRequestHeaderReceivedDelegate = FSimpleHttpSingleRequestHeaderReceivedDelegate(), 
			FAllRequestCompleteDelegate AllRequestCompleteDelegate = FAllRequestCompleteDelegate());

		/**
		 * Register our agent BP for internal use .
		 *
		 * @param return	Use handle find PRequest.
		 */
		FSimpleHTTPHandle RegisteredHttpRequest(EHTTPRequestType RequestType = EHTTPRequestType::SINGLE,
			FSimpleSingleCompleteDelegate SimpleHttpRequestCompleteDelegate = nullptr,
			FSimpleSingleRequestProgressDelegate	SimpleHttpRequestProgressDelegate = nullptr,
			FSimpleSingleRequestHeaderReceivedDelegate SimpleHttpRequestHeaderReceivedDelegate = nullptr,
			FSimpleDelegate AllRequestCompleteDelegate = nullptr);

		/** 
		 * Refer to the previous API for internal use details only 
		 *
		 * @param Handle	Easy to find requests .
		 */
		bool GetObjectToMemory(const FSimpleHTTPHandle &Handle,const FString &URL);
		
		/** 
		 * Refer to the previous API for internal use details only 
		 *
		 * @param Handle	Easy to find requests .
		 */
		void GetObjectsToMemory(const FSimpleHTTPHandle &Handle, const TArray<FString> &URL);
	
		/**
		 * Refer to the previous API for internal use details only
		 *
		 * @param Handle	Easy to find requests .
		 */
		bool GetObjectToLocal(const FSimpleHTTPHandle &Handle, const FString &URL, const FString &SavePaths);

		/**
		 * Refer to the previous API for internal use details only
		 *
		 * @param Handle	Easy to find requests .
		 */
		void GetObjectsToLocal(const FSimpleHTTPHandle &Handle, const TArray<FString> &URL, const FString &SavePaths);
	
		/**
		 * Refer to the previous API for internal use details only
		 *
		 * @param Handle	Easy to find requests .
		 */
		bool PutObjectFromLocal(const FSimpleHTTPHandle &Handle, const FString &URL, const FString &LocalPaths);
		
		/**
		 * Refer to the previous API for internal use details only
		 *
		 * @param Handle	Easy to find requests .
		 */
		bool PutObjectsFromLocal(const FSimpleHTTPHandle &Handle, const FString &URL, const FString &LocalPaths);
		
		/**
		 * Refer to the previous API for internal use details only
		 *
		 * @param Handle	Easy to find requests .
		 */
		bool PutObjectFromBuffer(const FSimpleHTTPHandle &Handle, const FString &URL, const TArray<uint8> &Buffer);
		
		/**
		 * Refer to the previous API for internal use details only
		 *
		 * @param Handle	Easy to find requests .
		 */
		bool PutObjectFromStream(const FSimpleHTTPHandle &Handle, const FString &URL, TSharedRef<FArchive, ESPMode::ThreadSafe> Stream);
	
		/**
		 * Refer to the previous API for internal use details only
		 *
		 * @param Handle	Easy to find requests .
		 */
		bool DeleteObject(const FSimpleHTTPHandle &Handle, const FString &URL);
		
		/**
		 * Refer to the previous API for internal use details only
		 *
		 * @param Handle	Easy to find requests .
		 */
		void DeleteObjects(const FSimpleHTTPHandle &Handle, const TArray<FString> &URL);

	private:
		/*You can find the corresponding request according to the handle  */
		TWeakPtr<FSimpleHttpActionRequest> Find(const FSimpleHTTPHandle &Handle);

		/*HTTP Map*/
		TMap<FSimpleHTTPHandle, TSharedPtr<FSimpleHttpActionRequest>> HTTPMap;
	};

public:
	static FSimpleHttpManage *Get();
	static void Destroy();

	/** Get HTTP function collection  **/
	FORCEINLINE FHTTP &GetHTTP() { return HTTP; }
private:

	static FSimpleHttpManage *Instance;
	FHTTP HTTP;
	FCriticalSection Mutex;
};

#define SIMPLE_HTTP FSimpleHttpManage::Get()->GetHTTP()
