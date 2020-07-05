#include "TestReflex.h"

#if OFF_REFLEXACTOR

#include "UObject/GeneratedCppIncludes.h"
#include "SomeTest/ReflexSystem/ReflexActor.h"//引用依赖文件
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	//生成的ReflexActor代码的空链接函数
	void EmptyLinkFunctionForGeneratedCodeReflexActor() {}

	// Cross Module References
	//构造UClass对应的UClass对象，无注册
	SOMETEST_API UClass* Z_Construct_UClass_AReflexActor_NoRegister();
	//构造UClass对应的UClass对象，有注册
	SOMETEST_API UClass* Z_Construct_UClass_AReflexActor();
	ENGINE_API UClass* Z_Construct_UClass_AActor();

	//构造SomeTest本身的UPackage对象
	UPackage* Z_Construct_UPackage__Script_SomeTest();

	SOMETEST_API UFunction* Z_Construct_UDelegateFunction_SomeTest_CharacterDelegate_Dynamic__DelegateSignature();
	SOMETEST_API UFunction* Z_Construct_UDelegateFunction_SomeTest_CharacterDelegate_DynamicRetVal__DelegateSignature();
	SOMETEST_API UFunction* Z_Construct_UDelegateFunction_SomeTest_CharacterDelegate_DynamicMulticast__DelegateSignature();
	SOMETEST_API UFunction* Z_Construct_UDelegateFunction_SomeTest_CharacterDelegate_DynamicMulticast2__DelegateSignature();
	UPackage* Z_Construct_UPackage__Script_SomeTest()
	{
		static UPackage* ReturnPackage = nullptr;
		if (!ReturnPackage)
		{
			static UObject* (*const SingletonFuncArray[])() = {
				(UObject* (*)())Z_Construct_UDelegateFunction_SomeTest_CharacterDelegate_Dynamic__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_SomeTest_CharacterDelegate_DynamicRetVal__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_SomeTest_CharacterDelegate_DynamicMulticast__DelegateSignature,
				(UObject* (*)())Z_Construct_UDelegateFunction_SomeTest_CharacterDelegate_DynamicMulticast2__DelegateSignature,
			};
			static const UE4CodeGen_Private::FPackageParams PackageParams = {
				"/Script/SomeTest",
				SingletonFuncArray,
				UE_ARRAY_COUNT(SingletonFuncArray),
				PKG_CompiledIn | 0x00000000,
				0x1AC6EC1B,
				0xAB156667,
				METADATA_PARAMS(nullptr, 0)
			};

			//struct FPackageParams
			//{
			//	const char*                        NameUTF8 =						"/Script/SomeTest";
			//	UObject*						   (*const *SingletonFuncArray)() = SingletonFuncArray;
			//	int32                              NumSingletons =					sizeof(SingletonFuncArray) - 1;
			//	uint32                             PackageFlags =					PKG_CompiledIn | 0x00000000; // EPackageFlags 此包来自“编译于”类。
			//	uint32                             BodyCRC =						0x1AC6EC1B;
			//	uint32                             DeclarationsCRC =				0xAB156667;
			//#if WITH_METADATA
			//	const FMetaDataPairParam*          MetaDataArray =					nullptr;
			//	int32                              NumMetaData =					0;
			//#endif
			//};

			UE4CodeGen_Private::ConstructUPackage(ReturnPackage, PackageParams);
		}
		return ReturnPackage;
	}

	// End Cross Module References
	//静态注册
	void AReflexActor::StaticRegisterNativesAReflexActor()
	{
	}

	//构造AReflexActor对应的UClass对象 无注册
	UClass* Z_Construct_UClass_AReflexActor_NoRegister()
	{
		return AReflexActor::StaticClass();
	}

	struct Z_Construct_UClass_AReflexActor_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA//需要在编辑器模式下
	//元数据的配对参数
	static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
	//定义特定C++类的特性
	static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
	//类参数
	static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	//依赖项
	UObject* (*const Z_Construct_UClass_AReflexActor_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,//构造UClass对应的UClass对象，有注册
		(UObject* (*)())Z_Construct_UPackage__Script_SomeTest,//构造SomeTest本身的UPackage对象
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AReflexActor_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "ReflexSystem/ReflexActor.h" },
		{ "ModuleRelativePath", "ReflexSystem/ReflexActor.h" },
	};
#endif
	//定义特定C++类的特性 为抽象
	const FCppClassTypeInfoStatic Z_Construct_UClass_AReflexActor_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AReflexActor>::IsAbstract,
	};

	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AReflexActor_Statics::ClassParams = {
		&AReflexActor::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		sizeof(DependentSingletons) - 1,
		0,
		0,
		0,
		0x009000A4u,
		//METADATA_PARAMS(Z_Construct_UClass_AReflexActor_Statics::Class_MetaDataParams, sizeof(Z_Construct_UClass_AReflexActor_Statics::Class_MetaDataParams) - 1)展开
		Z_Construct_UClass_AReflexActor_Statics::Class_MetaDataParams, sizeof(Z_Construct_UClass_AReflexActor_Statics::Class_MetaDataParams) - 1,
	};

	//struct FClassParams
	//{
	//	UClass*										(*ClassNoRegisterFunc)() =					&AReflexActor::StaticClass;
	//	const char*                                 ClassConfigNameUTF8							= "Engine";
	//	const FCppClassTypeInfoStatic*              CppClassInfo =								&StaticCppClassTypeInfo;
	//	UObject*									(*const *DependencySingletonFuncArray)() =	DependentSingletons;
	//	const FClassFunctionLinkInfo*               FunctionLinkArray =							nullptr;
	//	const FPropertyParamsBase* const*           PropertyArray =								nullptr;
	//	const FImplementedInterfaceParams*          ImplementedInterfaceArray =					nullptr;
	//	int32                                       NumDependencySingletons =					sizeof(DependentSingletons) - 1;
	//	int32                                       NumFunctions =								0;
	//	int32                                       NumProperties =								0;
	//	int32                                       NumImplementedInterfaces					0;
	//	uint32                                      ClassFlags =								0x009000A4u; // EClassFlags
	//#if WITH_METADATA
	//	const FMetaDataPairParam*                   MetaDataArray =								Z_Construct_UClass_AReflexActor_Statics::Class_MetaDataParams;
	//	int32                                       NumMetaData =								sizeof(Z_Construct_UClass_AReflexActor_Statics::Class_MetaDataParams) - 1;
	//#endif
	//};


	//构造 注册成员
	UClass* Z_Construct_UClass_AReflexActor()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AReflexActor_Statics::ClassParams);
		}
		return OuterClass;
	}

	//IMPLEMENT_CLASS(AReflexActor, 1395401713);展开
	//#define IMPLEMENT_CLASS(AReflexActor, TClassCrc)
	//延迟注册
	static TClassCompiledInDefer<AReflexActor> AutoInitializeAReflexActor(TEXT("AReflexActor"), sizeof(AReflexActor), 1395401713); 

	UClass* AReflexActor::GetPrivateStaticClass() 
	{ 
		static UClass* PrivateStaticClass = NULL; 
		if (!PrivateStaticClass) 
		{ 
			/* this could be handled with templates, but we want it external to avoid code bloat */ 
			//这可以用模板处理，但我们希望它是外部的，以避免代码膨胀。
			GetPrivateStaticClassBody( 
				StaticPackage(), 
				(TCHAR*)TEXT("AReflexActor") + 1 + ((StaticClassFlags & CLASS_Deprecated) ? 11 : 0),//+1 为了去掉前缀 U、A、F ，+11 是后缀_Deprecated
				PrivateStaticClass, 
				StaticRegisterNativesAReflexActor, 
				sizeof(AReflexActor), 
				alignof(AReflexActor), 
				(EClassFlags)AReflexActor::StaticClassFlags, 
				AReflexActor::StaticClassCastFlags(), 
				AReflexActor::StaticConfigName(), 
				(UClass::ClassConstructorType)InternalConstructor<AReflexActor>, 
				(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<AReflexActor>, 
				&AReflexActor::AddReferencedObjects, 
				&AReflexActor::Super::StaticClass, 
				&AReflexActor::WithinClass::StaticClass 
			); 
		} 
		return PrivateStaticClass; 
	}

	template<> SOMETEST_API UClass* StaticClass<AReflexActor>()
	{
		return AReflexActor::StaticClass();
	}

	static FCompiledInDefer Z_CompiledInDefer_UClass_AReflexActor(Z_Construct_UClass_AReflexActor, &AReflexActor::StaticClass, TEXT("/Script/SomeTest"), TEXT("AReflexActor"), false, nullptr, nullptr, nullptr);
	//DEFINE_VTABLE_PTR_HELPER_CTOR(AReflexActor);展开
	AReflexActor::AReflexActor(FVTableHelper& Helper) : Super(Helper) {};

	PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif

// Sets default values
AReflexActor::AReflexActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AReflexActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AReflexActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#endif