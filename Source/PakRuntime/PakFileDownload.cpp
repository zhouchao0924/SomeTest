//#include "PakFileDownload.h"
//#include "SimpleHTTPType.h"
//#include "HTTP/SimpleHttpActionRequest.h"
//#include "XmlFile.h"
//#include "HTTP/SimpleHttpActionMultpleRequest.h"
//#include "SimpleZipLibrary.h"
//
//void SimplePakRuntime::PakFileDownloadByHTTP(
//	const FString &InXmlURL, 
//	const FString &InLocalPaths, 
//	EEncryptionMode EncryptionMode /*= EEncryptionMode::NONE*/,
//	const FString &key /*= ""*/,
//	FSimpleHttpSingleRequestCompleteDelegate SimpleHttpRequestCompleteDelegate /*= FSimpleHttpSingleRequestCompleteDelegate()*/, 
//	FSimpleHttpSingleRequestProgressDelegate SimpleHttpRequestProgressDelegate /*= FSimpleHttpSingleRequestProgressDelegate()*/, 
//	FSimpleHttpSingleRequestHeaderReceivedDelegate SimpleHttpRequestHeaderReceivedDelegate /*= FSimpleHttpSingleRequestHeaderReceivedDelegate() */)
//{
//	auto XMLCompleteDelegate = [=](const FSimpleHttpRequest Request, const FSimpleHttpResponse Response, bool bConnectedSuccessfully)
//	{
//		if (Response.ResponseCode >= 200 && Response.ResponseCode <= 206 && bConnectedSuccessfully)
//		{
//			FString XMLPath = InLocalPaths / TEXT("DependencyPakFile.xml");
//			FXmlFile *XmlFile = new FXmlFile(XMLPath);
//			FXmlNode *RootNode = XmlFile->GetRootNode();
//			TArray<FString> HashNameArray;
//			FString URL;
//			if (RootNode)
//			{
//				URL = RootNode->GetAttribute(TEXT("BaseURL"));
//				const TArray<FXmlNode*> NodeFiles = RootNode->GetChildrenNodes();
//				for (const FXmlNode *NodeFile : NodeFiles)
//				{
//					const TArray<FXmlNode*> Blobs = NodeFile->GetChildrenNodes();
//					for (const FXmlNode *Blob : Blobs)
//					{
//						FString Hash = Blob->GetAttribute(TEXT("Hash"));
//						HashNameArray.AddUnique(Hash);
//					}
//				}
//			}
//
//			if (!URL.Contains(TEXT("http://")))
//			{
//				TEXT("http:/") / URL;
//			}
//			
//			TArray<FString> SearchURL;
//			for (auto &Tmp : HashNameArray)
//			{
//				SearchURL.Add(URL / Tmp);
//			}
//
//			auto AllTasksCompleted = [=]()
//			{
//				USimpleZipLibrary::DecompressedData(InLocalPaths, InLocalPaths, EncryptionMode, key);
//			};
//
//			FSimpleHttpActionMultpleRequest *MultpleRequest = new FSimpleHttpActionMultpleRequest();
//			MultpleRequest->SimpleHttpRequestCompleteDelegate = SimpleHttpRequestCompleteDelegate;
//			MultpleRequest->SimpleHttpRequestHeaderReceivedDelegate = SimpleHttpRequestHeaderReceivedDelegate;
//			MultpleRequest->SimpleHttpRequestProgressDelegate = SimpleHttpRequestProgressDelegate;
//			MultpleRequest->AllTasksCompletedDelegate = FSimpleDelegate::CreateLambda(AllTasksCompleted);
//
//			MultpleRequest->GetObjects(SearchURL, InLocalPaths);
//		}
//	};
//
//	FSimpleHttpActionRequest* NewRequest = new FSimpleHttpActionRequest();
//	NewRequest->SimpleCompleteDelegate = FSimpleSingleCompleteDelegate::CreateLambda(XMLCompleteDelegate);
//	NewRequest->GetObject(InXmlURL, InLocalPaths);
//}