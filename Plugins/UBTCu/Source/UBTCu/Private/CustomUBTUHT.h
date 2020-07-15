// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

//一个参数
struct FParamElement
{
	FName Name;
	bool bConst;//const
	bool bPointer;//*
	bool bReference;//&
	FName Type;//float int ...

	FParamElement()
		: bPointer(false)
		, bReference(false)
		, bConst(false)
	{}
};

//变量
struct FVariableAnalysis :public FParamElement
{
	FString CodeType;//是不是 Resources
	bool bStatic;

	FVariableAnalysis()
		:bStatic(false)
	{}
};

//函数解析
struct FFunctionAnalysis
{
	TArray<FParamElement> ParamArray;
	FParamElement Return;

	FName FunctionName;
	bool bStatic;
	bool bVirtual;
	FString CodeType;

	FFunctionAnalysis()
		:bStatic(false)
		, bVirtual(false)
	{}
};

//类解析成员
struct FClassAnalysis
{
	TArray<FFunctionAnalysis> Function;
	TArray<FVariableAnalysis> Variable;

	FName ClassName;
	TArray<FName> InheritName;
	FName CodeCPPName;

	int32 CodeLine;//反射宏标记在哪行？
	FString Include;
};

enum ECollectionParmType
{
	Type_Return,
	Type_Parm,
};

//IUnrealTypeCollection
class IUTC
{
public:
	//收集，通过路径收集类信息
	virtual bool Collection(const FString &Paths, FClassAnalysis &ClassAnalysis) = 0;
};

//IUnrealTypeBuilder
class IUTB
{
public:
	//构建反射信息
	virtual bool Builder(const FClassAnalysis &ClassAnalysis) = 0;
};

class FConstructionManagement
{
public:
	//执行我们的总逻辑
	void Builder();
};