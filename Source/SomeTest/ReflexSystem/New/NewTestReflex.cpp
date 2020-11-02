#include "NewTestReflex.h"

#if OFF_REFLEXACTOR

#include "UObject/GeneratedCppIncludes.h"
#include "SomeTest/ReflexSystem/New/NewReflexActor.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNewReflexActor() {}
// Cross Module References
SOMETEST_API UClass* Z_Construct_UClass_ANewReflexActor_NoRegister();
SOMETEST_API UClass* Z_Construct_UClass_ANewReflexActor();
ENGINE_API UClass* Z_Construct_UClass_AActor();
UPackage* Z_Construct_UPackage__Script_SomeTest();
// End Cross Module References

void ANewReflexActor::StaticRegisterNativesANewReflexActor()
{
}

UClass* Z_Construct_UClass_ANewReflexActor_NoRegister()
{
	return ANewReflexActor::StaticClass();
}

struct Z_Construct_UClass_ANewReflexActor_Statics
{
	static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
	static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
	static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
	static const UE4CodeGen_Private::FClassParams ClassParams;
};

UObject* (*const Z_Construct_UClass_ANewReflexActor_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AActor,
	(UObject* (*)())Z_Construct_UPackage__Script_SomeTest,
};

#if WITH_METADATA
const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ANewReflexActor_Statics::Class_MetaDataParams[] = {
	{ "IncludePath", "ReflexSystem/New/NewReflexActor.h" },
	{ "ModuleRelativePath", "ReflexSystem/New/NewReflexActor.h" },
};
#endif

const FCppClassTypeInfoStatic Z_Construct_UClass_ANewReflexActor_Statics::StaticCppClassTypeInfo = {
	TCppClassTypeTraits<ANewReflexActor>::IsAbstract,
};

const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ANewReflexActor_Statics::ClassParams = {
	&ANewReflexActor::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x009000A4u,
	METADATA_PARAMS(Z_Construct_UClass_ANewReflexActor_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ANewReflexActor_Statics::Class_MetaDataParams))
};

UClass* Z_Construct_UClass_ANewReflexActor()
{
	static UClass* OuterClass = nullptr;
	if (!OuterClass)
	{
		UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ANewReflexActor_Statics::ClassParams);
	}
	return OuterClass;
}

IMPLEMENT_CLASS(ANewReflexActor, 1374352086);//Õ¹¿ª
{
	// Register a class at startup time.
	//#define IMPLEMENT_CLASS(ANewReflexActor, 1374352086) 
	static TClassCompiledInDefer<ANewReflexActor> AutoInitializeANewReflexActor(TEXT(#ANewReflexActor), sizeof(ANewReflexActor), 1374352086); 
	UClass* ANewReflexActor::GetPrivateStaticClass() 
	{ 
		static UClass* PrivateStaticClass = NULL; 
		if (!PrivateStaticClass) 
		{ 
			/* this could be handled with templates, but we want it external to avoid code bloat */ 
			GetPrivateStaticClassBody( 
				StaticPackage(), 
				(TCHAR*)TEXT(ANewReflexActor) + 1 + ((StaticClassFlags & CLASS_Deprecated) ? 11 : 0), 
				PrivateStaticClass, 
				StaticRegisterNativesANewReflexActor, 
				sizeof(ANewReflexActor), 
				alignof(ANewReflexActor), 
				(EClassFlags)ANewReflexActor::StaticClassFlags, 
				ANewReflexActor::StaticClassCastFlags(), 
				ANewReflexActor::StaticConfigName(), 
				(UClass::ClassConstructorType)InternalConstructor<ANewReflexActor>, 
				(UClass::ClassVTableHelperCtorCallerType)InternalVTableHelperCtorCaller<ANewReflexActor>, 
				&ANewReflexActor::AddReferencedObjects, 
				&ANewReflexActor::Super::StaticClass, 
				&ANewReflexActor::WithinClass::StaticClass 
			); 
		} 
		return PrivateStaticClass; 
	}
}

template<> SOMETEST_API UClass* StaticClass<ANewReflexActor>()
{
	return ANewReflexActor::StaticClass();
}

static FCompiledInDefer Z_CompiledInDefer_UClass_ANewReflexActor(Z_Construct_UClass_ANewReflexActor, &ANewReflexActor::StaticClass, TEXT("/Script/SomeTest"), TEXT("ANewReflexActor"), false, nullptr, nullptr, nullptr);

DEFINE_VTABLE_PTR_HELPER_CTOR(ANewReflexActor);

PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif





#endif