#define SIMPLEOSS_MACRO_ERROR(Outcome) \
AuxiliaryTools.Print(Outcome##.error().Code()); \
AuxiliaryTools.Print(Outcome##.error().Message()); \
AuxiliaryTools.Print(Outcome##.error().RequestId()); 

#define SIMPLEOSS_MACRO_TYPE1(Request) \
SIMPLEOSS_MACRO_META(Request) \
SIMPLEOSS_MACRO_PROGRESSCALLBACK(Request)

#define SIMPLEOSS_MACRO_META(Request) \
/* 设置HTTP header */ \
ObjectMetaData meta; \
{ \
	if (!OSSObjectMeta.ContentType.IsEmpty())\
	{\
		meta.setContentType(TCHAR_TO_UTF8(*OSSObjectMeta.ContentType)); \
	}\
	if (!OSSObjectMeta.CacheControl.IsEmpty()) \
	{\
		meta.setCacheControl(TCHAR_TO_UTF8(*OSSObjectMeta.CacheControl));\
	}\
	if (!OSSObjectMeta.ContentDisposition.IsEmpty())\
	{\
		meta.setCacheControl(TCHAR_TO_UTF8(*OSSObjectMeta.ContentDisposition));\
	}\
	if (!OSSObjectMeta.ContentEncoding.IsEmpty())\
	{\
		meta.setCacheControl(TCHAR_TO_UTF8(*OSSObjectMeta.ContentEncoding));\
	}\
	if (!OSSObjectMeta.ContentMd5.IsEmpty())\
	{\
		meta.setCacheControl(TCHAR_TO_UTF8(*OSSObjectMeta.ContentMd5));\
	}\
}\
/*自定义元数据*/ \
for (auto &Tmp : CustomMeta)\
{\
	meta.UserMetaData()[TCHAR_TO_UTF8(*Tmp.Key)] = TCHAR_TO_UTF8(*Tmp.Value);\
}\
Request.MetaData() = meta; 

#define SIMPLEOSS_MACRO_PROGRESSCALLBACK(Request) \
TransferProgress progressCallback = { InProgressCallback , nullptr }; \
Request.setTransferProgress(progressCallback);
