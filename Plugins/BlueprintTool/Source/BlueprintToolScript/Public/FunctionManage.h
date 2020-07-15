#pragma once

#include "CoreMinimal.h"
#include "BTScript.h"

struct BLUEPRINTTOOLSCRIPT_API FBTFuntionID
{
	FName ClassName;
	FName FuncName;
	FNativeBTFuncPtr NativeFuncPtr;

	FBTFuntionID(FName NewClassName, FName NewFuncName, FNativeBTFuncPtr NewNativeFuncPtr)
		:ClassName(NewClassName)
		, FuncName(NewFuncName)
		, NativeFuncPtr(NewNativeFuncPtr)
	{}
};

struct BLUEPRINTTOOLSCRIPT_API FFuntionManage
{
	static FNativeBTFuncPtr &GetNativeFuncPtr(UFunction *Function);
	static int32 SetNativeFuncPtr(const FBTFuntionID &Function);
private:
	static TArray<FBTFuntionID> FunctionList;
};

//作为中间函数存储参数使用
class BLUEPRINTTOOLSCRIPT_API FFunctionParameter :public TSharedFromThis<FFunctionParameter>
{
public:
	FFunctionParameter(void const* InAddr)
		:ParaAddr(InAddr)
	{}

	void const *ParaAddr;
	TSharedPtr<FFunctionParameter> Nest;
};

BLUEPRINTTOOLSCRIPT_API void AddParaAddr(void const* InAddr, TSharedPtr<FFunctionParameter> &FunctionParameter);
BLUEPRINTTOOLSCRIPT_API void const *GetEventParaAddr(TSharedPtr<FFunctionParameter> &FunctionParameter);