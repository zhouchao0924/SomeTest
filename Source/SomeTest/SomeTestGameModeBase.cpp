// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "SomeTestGameModeBase.h"
#include "ReflexSystem/ReflexActor.h"
#include "ReflexSystem/ReflexStruct.h"
#include "UObject/PropertyPortFlags.h"
#include "UObject/UObjectHash.h"
#include "Async/TaskGraphInterfaces.h"
#include "MultiThread/GraphTask.h"
#include "MultiThread/MyAsyncTask.h"
#include "ThreadManage.h"
#include "PakEditor/PakFileUpload.h"
#include "PakRuntime/PakFileDownload.h"
#include "PakInfo.h"

#define IsUse 0

#if PLATFORM_WINDOWS
#pragma optimize("", off) 
#endif

//利用反射写代码
template<typename... TReturns, typename... TArgs>
void Call(FName FunctionName, TTuple<TReturns...>& OutParams, TArgs&&... Args);

FString ASomeTestGameModeBase::TestFunc3(int32 IntValue, const FString &Name, bool bShowWindows)
{
	TTuple<TArray<AActor*>, UObject*> ReturnNative;
	Call(TEXT("TestFuncCX"), ReturnNative, 13, 7.f, this, 6.7f);

	TTuple<int32, FName, FTransform, bool> ReturnBlueprint;
	Call(TEXT("EEERRR"), ReturnBlueprint, FRotator(3.f, 4.f, 5.f), 245, FString("-Hello"), this);

	return "EEE";

	//获取特定类型
	TArray<UObject*> ObjectArrayClass;
	GetObjectsOfClass(AReflexActor::StaticClass(), ObjectArrayClass);

	TArray<UObject*> ObjectArrayScriptStruct;
	GetObjectsOfClass(UScriptStruct::StaticClass(), ObjectArrayScriptStruct);

	TArray<UObject*> ObjectArrayEnum;
	GetObjectsOfClass(UEnum::StaticClass(), ObjectArrayEnum);

	UEnum* MyEnum = FindObject<UEnum>(ANY_PACKAGE, *FString("EReflexEnum"));

	FReflexStruct MyStruct;
	MyStruct.VlaueString = "W";
	MyStruct.VlaueC = 999.f;
	FString OutString;
	float V = 0.f;
	float *V2 = nullptr;
	EPropertyFlags PropertyFlags = EPropertyFlags::CPF_None;
	for (TFieldIterator<UProperty> i(MyStruct.StaticStruct()); i; ++i)
	{
		UProperty* prop = *i;
		if (prop)
		{
			if (prop->GetName() == "VlaueC")
			{
				UFloatProperty *floatP = Cast<UFloatProperty>(prop);
				if (floatP && floatP->HasAllPropertyFlags(CPF_BlueprintReadOnly))
				{
					void* ValueAddr = floatP->ContainerPtrToValuePtr<void*>(&MyStruct);
					floatP->SetPropertyValue(ValueAddr, 10.f);
					V = floatP->GetPropertyValue(ValueAddr);
					V2 = floatP->GetPropertyValuePtr(ValueAddr);
					PropertyFlags = floatP->GetPropertyFlags();
					//floatP->GetPropertyValue_InContainer(ValueAddr,3);
				}
			}
			if (prop->GetName() == "VlaueString")
			{
				UStrProperty *StrP = Cast<UStrProperty>(prop);
				if (StrP)
				{
					StrP->ImportText(TEXT("EEEE"), StrP->ContainerPtrToValuePtr<void*>(&MyStruct), EPropertyPortFlags::PPF_None, nullptr);
					StrP->ExportTextItem(OutString, StrP->ContainerPtrToValuePtr<void*>(&MyStruct), nullptr, NULL, EPropertyPortFlags::PPF_None, nullptr);
				}
			}
		}
	}

	for (TFieldIterator<UFunction> i(AReflexActor::StaticClass()); i; ++i)
	{
		UFunction* MyFunc = *i;
		for (TFieldIterator<UProperty> j(MyFunc); j; ++j)
		{
			UProperty* FunParam = *j;
			if (FunParam->PropertyFlags & CPF_ReturnParm)
			{
				TArray<UObject*> ObjectArrayEnum1;
				GetObjectsOfClass(UEnum::StaticClass(), ObjectArrayEnum1);
			}
		}
	}

	for (int32 i = 0; i < AReflexActor::StaticClass()->Interfaces.Num(); i++)
	{
		FImplementedInterface * ImplementedInterface = &AReflexActor::StaticClass()->Interfaces[i];

		TArray<UObject*> ObjectArrayEnum1;
		GetObjectsOfClass(UEnum::StaticClass(), ObjectArrayEnum1);
	}

	//描述 AReflexActor	AReflexActor::StaticClass()
	//描述 AActor		AActor::StaticClass()
	//描述 UObject		UObject::StaticClass()
	UClass* SuperClass = AReflexActor::StaticClass()->GetSuperClass();
	while (SuperClass)
	{
		SuperClass = SuperClass->GetSuperClass();

		TArray<UObject*> ObjectArrayEnum2;
		GetObjectsOfClass(UEnum::StaticClass(), ObjectArrayEnum2);
	}

	TArray<UClass*> ClassArray;
	GetDerivedClasses(AActor::StaticClass(), ClassArray, false);

	{
		TArray<UObject*> ObjectArrayEnum2;
		GetObjectsOfClass(UEnum::StaticClass(), ObjectArrayEnum2);
	}

	if (AReflexActor::StaticClass()->ImplementsInterface(UReflexInterface::StaticClass()))
	{
		{
			TArray<UObject*> ObjectArrayEnum2;
			GetObjectsOfClass(UEnum::StaticClass(), ObjectArrayEnum2);
		}
	}

	bool IsHave = false;
	ForEachObjectOfClass(AReflexActor::StaticClass(), [&IsHave](UObject* Object)
	{
		IsHave = CastChecked<AReflexActor>(Object)->TestFunc2();
	});

	if (IsHave)
	{
		TArray<UObject*> ObjectArrayEnum2;
		GetObjectsOfClass(UEnum::StaticClass(), ObjectArrayEnum2);
	}

	ClassHasInstancesAsyncLoading(AReflexActor::StaticClass());

	//TTuple<FString, TArray<AReflexActor*>> CC = CallFindFunctionByName<FString, TArray<AReflexActor*>>("TestFunc6", 10);




	TArray<UObject*> ObjectArrayEnum2;
	GetObjectsOfClass(UEnum::StaticClass(), ObjectArrayEnum2);

	bool IsBool = DoFun(this, "TestPrint", "Hello");
	bool IsBool2 = DoFun2(this, "TestPrint", "Hello");
	bool IsBool3 = DoFun3(this, "TestPrint", "Hello");
	DoFun4(this, "");

	TArray<AReflexActor*> Actor;
	DoFun5(this, 10, Actor);
	return FString();
}

void ASomeTestGameModeBase::TestFunc4()
{
	TArray<UObject*> ObjectArrayEnum2;
	int32 i = 0;
	i++;
}

FString ASomeTestGameModeBase::TestFunc5(int32 Index, TArray<class AReflexActor*> &Actor)
{
	Actor.SetNum(Index);

	return FString("Hello World");
}

FString ASomeTestGameModeBase::TestFuncXXX(int32 Index, TArray<class AReflexActor*> &Actor)
{
	Actor.SetNum(Index);
	return FString("Hello World");
}

UObject * ASomeTestGameModeBase::TestFuncCX(int32 Index, float FFF, UObject *NewC, float C, TArray<class AActor*> &Actor)
{
	Actor.Add(this);
	return this;
}

FString ASomeTestGameModeBase::TestFunc6(int32 Index, TArray<class AReflexActor*> &Actor)
{
	Actor.SetNum(Index);
	return FString("Hello World");
}

FString ASomeTestGameModeBase::TestFunc23(int32 Index, float cc)
{
	return "ddd";
}

UObject * ASomeTestGameModeBase::TestFunc7()
{
	return nullptr;
}

FString CC;
FString & ASomeTestGameModeBase::TestFunc8()
{
	return CC;
}

bool ASomeTestGameModeBase::DoFun(UObject* Object, FString FunName, FString String)
{
	struct FDoFun_Parms
	{
		FString String;
		bool ReturnValue;
	};

	FDoFun_Parms DoFun_Parms;
	DoFun_Parms.String = String;

	UFunction* FunInstance = Object->FindFunctionChecked(*FunName);

	Object->ProcessEvent(FunInstance, &DoFun_Parms);

	return DoFun_Parms.ReturnValue;
}

bool ASomeTestGameModeBase::DoFun2(UObject* Object, FString FunName, FString String)
{
	struct FDoFun_Parms
	{
		FString String;
		bool ReturnValue;
	};

	FDoFun_Parms DoFun_Parms;
	DoFun_Parms.String = String;

	UFunction* FunInstance = Object->FindFunctionChecked(*FunName);

	FFrame Frame(nullptr, FunInstance, &DoFun_Parms, nullptr, FunInstance->Children);
	Object->CallFunction(Frame, &DoFun_Parms + FunInstance->ReturnValueOffset, FunInstance);

	return DoFun_Parms.ReturnValue;
}

bool ASomeTestGameModeBase::DoFun3(UObject* Object, FString FunName, FString String)
{
	struct FDoFun_Parms
	{
		FString String;
		bool ReturnValue;
	};

	FDoFun_Parms DoFun_Parms;
	DoFun_Parms.String = String;

	UFunction* FunInstance = Object->FindFunctionChecked(*FunName);

	FFrame Frame(nullptr, FunInstance, &DoFun_Parms, nullptr, FunInstance->Children);
	FunInstance->Invoke(Object, Frame, &DoFun_Parms + FunInstance->ReturnValueOffset);

	return DoFun_Parms.ReturnValue;
}

UFunction* Construct_UFunction();
UFunction* Construct_UFunction2();

void ASomeTestGameModeBase::DoFun4(UObject* Object, FString FunName)
{
	UFunction* FunInstance = Construct_UFunction();
	FunInstance->SetNativeFunc(ASomeTestGameModeBase::execTestFunc4);

	FFrame Frame(nullptr, FunInstance, nullptr, nullptr, FunInstance->Children);

	FunInstance->Invoke(Object, Frame, nullptr);
}

FString ASomeTestGameModeBase::DoFun5(UObject* Object, int32 Index, TArray<AReflexActor*> &Actor)
{
	struct FDoFun_Parms
	{
		int32 Index;
		TArray<AReflexActor*> Actor;
		FString ReturnValue;
	};

	FDoFun_Parms DoFun_Parms;
	DoFun_Parms.Index = Index;

	UFunction* FunInstance = Construct_UFunction2();
	FunInstance->SetNativeFunc(ASomeTestGameModeBase::execTestFunc5c);

	FFrame Frame(nullptr, FunInstance, &DoFun_Parms, nullptr, FunInstance->Children);

	FOutParmRec** LastOut = &Frame.OutParms;
	for (TFieldIterator<UProperty> ParmIt(FunInstance); ParmIt; ++ParmIt)
	{
		UProperty *Property = *ParmIt;
		if (Property->GetName() == "Index")
		{
			Cast<UIntProperty>(Property)->SetPropertyValue(Property->ContainerPtrToValuePtr<void*>(&DoFun_Parms), DoFun_Parms.Index);
		}

		for (;;)
		{
			if (*LastOut == nullptr)
			{
				(*LastOut) = (FOutParmRec*)FMemory_Alloca(sizeof(FOutParmRec));
				(*LastOut)->PropAddr = (uint8 *)(Property->ContainerPtrToValuePtr<void*>(&DoFun_Parms));
				(*LastOut)->Property = Property;
				(*LastOut)->NextOutParm = nullptr;
				LastOut = &(*LastOut)->NextOutParm;

				break;
			}
			else
			{
				LastOut = &(*LastOut)->NextOutParm;
			}
		}
	}

	FunInstance->Invoke(Object, Frame, &DoFun_Parms.ReturnValue);

	Actor = DoFun_Parms.Actor;
	return DoFun_Parms.ReturnValue;
}

UFunction* Construct_UFunction()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UE4CodeGen_Private::FFunctionParams FunctionParams;

		FunctionParams =
		{
			(UObject*(*)())ASomeTestGameModeBase::StaticClass,
				nullptr,
				"TestFunc4",
				nullptr,
				nullptr,
				0,
				nullptr,
				0,
				RF_Public | RF_Transient | RF_MarkAsNative,
				(EFunctionFlags)0x04020401,
				0,
				0,
				0,
				0
		};

		UE4CodeGen_Private::ConstructUFunction(ReturnFunction, FunctionParams);
	}

	return ReturnFunction;
}

struct FConstruct_UFunction_Params
{
	struct Function_ParamsArray
	{
		int32 Index;
		TArray<AReflexActor*> Actor;
		FString ReturnValue;
	};

	static const UE4CodeGen_Private::FStrPropertyParams NewProp_ReturnValue;
	static const UE4CodeGen_Private::FIntPropertyParams NewProp_Index;
	static const UE4CodeGen_Private::FArrayPropertyParams NewProp_Actor;
	static const UE4CodeGen_Private::FObjectPropertyParams NewProp_Actor_Inner;

	static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];

#if WITH_METADATA
	static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif

	static const UE4CodeGen_Private::FFunctionParams FuncParams;
};

#if WITH_METADATA
const UE4CodeGen_Private::FMetaDataPairParam FConstruct_UFunction_Params::Function_MetaDataParams[] =
{
	{ "ModuleRelativePath", "SomeTestGameModeBase.h" },
};
#endif

const UE4CodeGen_Private::FPropertyParamsBase* const FConstruct_UFunction_Params::PropPointers[] =
{
	(const UE4CodeGen_Private::FPropertyParamsBase*)&FConstruct_UFunction_Params::NewProp_ReturnValue,
	(const UE4CodeGen_Private::FPropertyParamsBase*)&FConstruct_UFunction_Params::NewProp_Index,
	(const UE4CodeGen_Private::FPropertyParamsBase*)&FConstruct_UFunction_Params::NewProp_Actor,
	(const UE4CodeGen_Private::FPropertyParamsBase*)&FConstruct_UFunction_Params::NewProp_Actor_Inner,
};

const UE4CodeGen_Private::FStrPropertyParams FConstruct_UFunction_Params::NewProp_ReturnValue =
{
	"ReturnValue",
	nullptr,
	(EPropertyFlags)0x0010000000000580,
	UE4CodeGen_Private::EPropertyGenFlags::Str,
	RF_Public | RF_Transient | RF_MarkAsNative,
	1,
	STRUCT_OFFSET(Function_ParamsArray, ReturnValue),
	METADATA_PARAMS(nullptr, 0)
};

const UE4CodeGen_Private::FIntPropertyParams FConstruct_UFunction_Params::NewProp_Index =
{	
	"Index",
	nullptr,
	(EPropertyFlags)0x0010000000000080,
	UE4CodeGen_Private::EPropertyGenFlags::Int,
	RF_Public | RF_Transient | RF_MarkAsNative,
	1,
	STRUCT_OFFSET(Function_ParamsArray, Index),
	METADATA_PARAMS(nullptr, 0)
};

const UE4CodeGen_Private::FArrayPropertyParams FConstruct_UFunction_Params::NewProp_Actor =
{
	"Actor",
	nullptr,
	(EPropertyFlags)0x0010000000000180,
	UE4CodeGen_Private::EPropertyGenFlags::Array,
	RF_Public | RF_Transient | RF_MarkAsNative,
	1,
	STRUCT_OFFSET(Function_ParamsArray, Actor),
	METADATA_PARAMS(nullptr, 0)
};

const UE4CodeGen_Private::FObjectPropertyParams FConstruct_UFunction_Params::NewProp_Actor_Inner =
{	
	"Actor",
	nullptr,
	(EPropertyFlags)0x0000000000000000,
	UE4CodeGen_Private::EPropertyGenFlags::Object,
	RF_Public | RF_Transient | RF_MarkAsNative,
	1,
	0,
	AReflexActor::StaticClass,
	METADATA_PARAMS(nullptr, 0)
};

//使用静态方式注册函数变量
const UE4CodeGen_Private::FFunctionParams FConstruct_UFunction_Params::FuncParams =
{
	(UObject*(*)())ASomeTestGameModeBase::StaticClass,
	nullptr,
	"TestFunc5",
	nullptr,
	nullptr,
	sizeof(Function_ParamsArray),
	FConstruct_UFunction_Params::PropPointers,
	UE_ARRAY_COUNT(FConstruct_UFunction_Params::PropPointers),
	RF_Public | RF_Transient | RF_MarkAsNative,
	(EFunctionFlags)0x04020401,
	0, 
	0,
	FConstruct_UFunction_Params::Function_MetaDataParams,
	UE_ARRAY_COUNT(FConstruct_UFunction_Params::Function_MetaDataParams)
};

UFunction* Construct_UFunction2()
{
	static UFunction* ReturnFunction1 = nullptr;
	if (!ReturnFunction1)
	{
		//构造
		UE4CodeGen_Private::ConstructUFunction(ReturnFunction1, FConstruct_UFunction_Params::FuncParams);
	}

	return ReturnFunction1;
}

bool ASomeTestGameModeBase::TestPrint(FString Index)
{
	TArray<UObject*> ObjectArrayEnum2;
	GetObjectsOfClass(UEnum::StaticClass(), ObjectArrayEnum2);

	return true;
}

void ASomeTestGameModeBase::BeginPlay()
{
	Super::BeginPlay();
#if IsUse
	//多线程
	NewInterface = new ITestMultiThreadInterface();
	NewTestMultiThread = new TestMultiThread();
	NewTestMultiThread->TestMultiThreadDelegate.BindUObject(this, &ASomeTestGameModeBase::print);
	NewTestMultiThread->CreateThread(NewInterface);

	TGraphTask<GraphTask>::CreateTask(NULL, ENamedThreads::GameThread).ConstructAndDispatchWhenReady(4.5f);

	FAsyncTask<MyAsyncTask> *MyTask = new FAsyncTask<MyAsyncTask>(3);
	MyTask->StartBackgroundTask();
	//MyTask->StartSynchronousTask();
	if (MyTask->IsDone())
	{
		UE_LOG(LogTemp, Log, TEXT("MyTask->IsDone"));
	}
	MyTask->EnsureCompletion();
	delete MyTask;
#endif
	//OSSTest::MainOSS();		

	FString AccessKeyID = "XXX";
	FString AccessKeyIDSecret = "XXX";
	FString EndPoint = "oss-cn-beijing.aliyuncs.com";
	FString BucketName = "smartuiloss";
	FString LocalPath = "C:/Users/Smartuil/Desktop/SomeTest/Pak/File";
	TArray<FString> Error;

	//SimplePakEditor::PakFileUploadByOSS(AccessKeyID, AccessKeyIDSecret, EndPoint, BucketName, LocalPath, Error);
	//FPakEditorUploadDelegate PakEditorUploadDelegate;
	//PakEditorUploadDelegate.BindDynamic(this, &ASomeTestGameModeBase::TestPak);
	//SimplePakEditor::AsynchPakFileUploadByOSS(AccessKeyID, AccessKeyIDSecret, EndPoint, BucketName, LocalPath, PakEditorUploadDelegate);

	//for (auto &Tmp : Error)
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, Tmp);
	//	}
	//}

	FSimpleHttpSingleRequestCompleteDelegate			SimpleHttpRequestCompleteDelegate;
	FSimpleHttpSingleRequestProgressDelegate			SimpleHttpRequestProgressDelegate;
	FSimpleHttpSingleRequestHeaderReceivedDelegate		SimpleHttpRequestHeaderReceivedDelegate;
	SimpleHttpRequestCompleteDelegate.BindDynamic(this, &ASomeTestGameModeBase::TestPak2);
	SimpleHttpRequestProgressDelegate.BindDynamic(this, &ASomeTestGameModeBase::TestPak3);
	SimpleHttpRequestHeaderReceivedDelegate.BindDynamic(this, &ASomeTestGameModeBase::TestPak4);

	//FString URL = "http://smartuiloss.oss-cn-beijing.aliyuncs.com";
	//SimplePakEditor::PakFileUploadByHTTP(URL, LocalPath, SimpleHttpRequestCompleteDelegate, SimpleHttpRequestProgressDelegate, SimpleHttpRequestHeaderReceivedDelegate);
	
	FString URL = "https://smartuiloss.oss-cn-beijing.aliyuncs.com/DependencyPakFile.xml";
	FString XMLPath = "C:/Users/Smartuil/Desktop/SomeTest/Pak/Down";
	SimplePakRuntime::PakFileDownloadByHTTP(URL, XMLPath, EEncryptionMode::NONE, "", SimpleHttpRequestCompleteDelegate, SimpleHttpRequestProgressDelegate, SimpleHttpRequestHeaderReceivedDelegate);
}

void ASomeTestGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
#if IsUse
	delete NewTestMultiThread;
	NewInterface = nullptr;
#endif
	FThreadManagement::Destroy();
}

//主线程执行内容
void ASomeTestGameModeBase::print()
{
	UE_LOG(LogTemp, Log, TEXT("print"));
}

void ASomeTestGameModeBase::TestPak(int32 InSurplus, int64 InTotal, const FString& Mess)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, 
			FString::Printf(TEXT("InSurplus = %i, InTotal = %i, Mess = %s"), InSurplus, InTotal, *Mess));
	}
}

void ASomeTestGameModeBase::TestPak2(const FSimpleHttpRequest Request, const FSimpleHttpResponse Response, bool bConnectedSuccessfully)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
			FString::Printf(TEXT("bConnectedSuccessfully = %i"), bConnectedSuccessfully));
	}
}

void ASomeTestGameModeBase::TestPak3(const FSimpleHttpRequest Request, int32 BytesSent, int32 BytesReceived)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
			FString::Printf(TEXT("BytesSent = %i, BytesReceived = %i"), BytesSent, BytesReceived));
	}
}

void ASomeTestGameModeBase::TestPak4(const FSimpleHttpRequest Request, const FString HeaderName, const FString NewHeaderValue)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red,
			FString::Printf(TEXT("HeaderName = %s, NewHeaderValue = %s"), *HeaderName, *NewHeaderValue));
	}
}

template<typename... TReturns, typename... TArgs>
void ImplementFunction(UClass* OuterClass, UFunction* Function, TTuple<TReturns...>& OutParams, TArgs&&... Args)
{
	void* FuncPramsStructAddr = (uint8*)FMemory_Alloca(Function->ParmsSize);
	uint8* OutPramsByte = (uint8*)&OutParams;
	TTuple<TArgs...> InParams(Forward<TArgs>(Args)...);
	uint8* InPramsByte = (uint8*)&InParams;

	if (Function->HasAnyFunctionFlags(FUNC_Native))
	{
		FFrame Frame(nullptr, Function, &FuncPramsStructAddr, nullptr, Function->Children);
		uint8* ReturnParams = nullptr;

		//执行无字节码的函数
		//注册虚拟机参数列表
		FOutParmRec** LastOut = &Frame.OutParms;

		for (TFieldIterator<UProperty> ParmIt(Function); ParmIt; ++ParmIt)
		{
			UProperty *Property = *ParmIt;
			if (Property->PropertyFlags & CPF_OutParm)
			{
				if (Property->PropertyFlags & CPF_ReturnParm)
				{
					ReturnParams = OutPramsByte;
					OutPramsByte += Property->GetSize();
				}
				else
				{
					for (;;)
					{
						if (*LastOut == nullptr)
						{
							(*LastOut) = (FOutParmRec*)FMemory_Alloca(sizeof(FOutParmRec));
							(*LastOut)->PropAddr = OutPramsByte;
							(*LastOut)->Property = Property;
							(*LastOut)->NextOutParm = nullptr;
							LastOut = &(*LastOut)->NextOutParm;

							OutPramsByte += Property->GetSize();

							break;
						}
						else
						{
							LastOut = &(*LastOut)->NextOutParm;
						}
					}
				}
			}
			else if (Property->PropertyFlags & CPF_Parm)
			{
				for (;;)
				{
					if (*LastOut == nullptr)
					{
						(*LastOut) = (FOutParmRec*)FMemory_Alloca(sizeof(FOutParmRec));
						(*LastOut)->PropAddr = (uint8 *)(Property->ContainerPtrToValuePtr<void*>(&FuncPramsStructAddr));
						if (UObjectProperty *ObjectPropert = Cast<UObjectProperty>(Property))
						{
							ObjectPropert->SetObjectPropertyValue(const_cast<uint8*>((*LastOut)->PropAddr), *(UObject**)InPramsByte);
						}
						else
						{
							FMemory::Memcpy((*LastOut)->PropAddr, InPramsByte, Property->GetSize());
						}

						(*LastOut)->Property = Property;
						(*LastOut)->NextOutParm = nullptr;
						LastOut = &(*LastOut)->NextOutParm;

						InPramsByte += Property->GetSize();

						break;
					}
					else
					{
						LastOut = &(*LastOut)->NextOutParm;
					}
				}
			}
		}

		Function->Invoke(OuterClass, Frame, ReturnParams);
		return;
	}

	//必须要有字节码，也就是它必须是蓝图函数，或者是BlueprintNativeEvent or BlueprintImplementableEvent 在蓝图编译的函数
	//遍历函数
	for (TFieldIterator<UProperty> i(Function); i; ++i)
	{
		UProperty* Property = *i;
		void* PropAddr = Property->ContainerPtrToValuePtr<void*>(FuncPramsStructAddr);
		if (Property->PropertyFlags & CPF_OutParm)
		{
			FMemory::Memcpy(PropAddr, OutPramsByte, Property->GetSize());
			OutPramsByte += Property->GetSize();
		}
		else if (Property->PropertyFlags & CPF_Parm)
		{
			FMemory::Memcpy(PropAddr, InPramsByte, Property->GetSize());
			InPramsByte += Property->GetSize();
		}
	}

	//必须要有字节码
	OuterClass->ProcessEvent(Function, FuncPramsStructAddr);

	OutPramsByte = (uint8*)&OutParams;
	for (TFieldIterator<UProperty> i(Function); i; ++i)
	{
		UProperty* Property = *i;
		if (Property->PropertyFlags & CPF_OutParm)
		{
			void* PropAddr = Property->ContainerPtrToValuePtr<void*>(FuncPramsStructAddr);
			FMemory::Memcpy(OutPramsByte, PropAddr, Property->GetSize());

			OutPramsByte += Property->GetSize();
		}
	}
}

//TReturns	包含引用 和返回值 一般返回值放在后面
//TArgs		传递参数 包含指针和变量
template<typename... TReturns, typename... TArgs>
void Call(FName FunctionName, TTuple<TReturns...>& OutParams, TArgs&&... Args)
{
	//可以找到带有字节码的函数
	UFunction* Function = (UFunction*)StaticFindObjectFast(UFunction::StaticClass(), nullptr, FunctionName, false, true, RF_Transient);
	if (!Function)
	{
		//寻找无字节码的函数 
		Function = FindObject<UFunction>(ANY_PACKAGE, *FunctionName.ToString());
	}

	ImplementFunction<TReturns...>(Function->GetOuterUClass(), Function, OutParams, Forward<TArgs>(Args)...);
}

#if PLATFORM_WINDOWS
#pragma optimize("", on) 
#endif
