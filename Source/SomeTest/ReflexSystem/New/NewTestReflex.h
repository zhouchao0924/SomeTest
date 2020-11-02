#pragma once
#define OFF_REFLEXACTOR 0
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewReflexActor.generated.h"
#if OFF_REFLEXACTOR

GENERATED_BODY()//展开
{
	CURRENT_FILE_ID___LINE___GENERATED_BODY
	SomeTest_Source_SomeTest_ReflexSystem_New_NewReflexActor_h_12_GENERATED_BODY

	SomeTest_Source_SomeTest_ReflexSystem_New_NewReflexActor_h_12_PRIVATE_PROPERTY_OFFSET//空
	SomeTest_Source_SomeTest_ReflexSystem_New_NewReflexActor_h_12_SPARSE_DATA//空
	SomeTest_Source_SomeTest_ReflexSystem_New_NewReflexActor_h_12_RPC_WRAPPERS_NO_PURE_DECLS//空

	SomeTest_Source_SomeTest_ReflexSystem_New_NewReflexActor_h_12_INCLASS_NO_PURE_DECLS//展开
	{
		private:
			static void StaticRegisterNativesANewReflexActor();
			friend struct Z_Construct_UClass_ANewReflexActor_Statics;
		public:
			DECLARE_CLASS(ANewReflexActor, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/SomeTest"), NO_API)//展开
			{
				private:
					ANewReflexActor& operator=(ANewReflexActor&&);
					ANewReflexActor& operator=(const ANewReflexActor&);
					NO_API static UClass* GetPrivateStaticClass();
				public:
					/** Bitwise union of #EClassFlags pertaining to this class.*/
					enum { StaticClassFlags = COMPILED_IN_FLAGS(0 | CLASS_Config) };
					/** Typedef for the base class ({{ typedef-type }}) */
					typedef AActor Super;
					/** Typedef for {{ typedef-type }}. */
					typedef ANewReflexActor ThisClass;
					/** Returns a UClass object representing this class at runtime */
					inline static UClass* StaticClass()
					{
						return GetPrivateStaticClass();
					}
					/** Returns the package this class belongs in */
					inline static const TCHAR* StaticPackage()
					{
						return TEXT("/Script/SomeTest");
					}
					/** Returns the static cast flags for this class */
					inline static EClassCastFlags StaticClassCastFlags()
					{
						return CASTCLASS_None;
					}
					/** For internal use only; use StaticConstructObject() to create new objects. */
					inline void* operator new(const size_t InSize, EInternal InInternalOnly, UObject* InOuter = (UObject*)GetTransientPackage(), FName InName = NAME_None, EObjectFlags InSetFlags = RF_NoFlags) \
					{
						return StaticAllocateObject(StaticClass(), InOuter, InName, InSetFlags);
					}
					/** For internal use only; use StaticConstructObject() to create new objects. */
					inline void* operator new(const size_t InSize, EInternal* InMem)
					{
						return (void*)InMem;
					}
			}
			DECLARE_SERIALIZER(ANewReflexActor)//展开
			{
				friend FArchive &operator<<(FArchive& Ar, ANewReflexActor*& Res)
				{
					return Ar << (UObject*&)Res;
				}
					friend void operator<<(FStructuredArchive::FSlot InSlot, ANewReflexActor*& Res)
				{
					InSlot << (UObject*&)Res;
				}
			}
	}

	SomeTest_Source_SomeTest_ReflexSystem_New_NewReflexActor_h_12_ENHANCED_CONSTRUCTORS//展开
	{
		private:
			/** Private move- and copy-constructors, should never be used */
			NO_API ANewReflexActor(ANewReflexActor&&);
			NO_API ANewReflexActor(const ANewReflexActor&);
		public:
			DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ANewReflexActor);//展开
			{
				/** DO NOT USE. This constructor is for internal usage only for hot-reload purposes. */ 
				NO_API ANewReflexActor(FVTableHelper& Helper);
			}
			DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ANewReflexActor);//展开
			{
				static UObject* __VTableCtorCaller(FVTableHelper& Helper) 
				{ 
					return new (EC_InternalUseOnlyConstructor, (UObject*)GetTransientPackage(), NAME_None, RF_NeedLoad | RF_ClassDefaultObject | RF_TagGarbageTemp) ANewReflexActor(Helper);
				}
			}
			DEFINE_DEFAULT_CONSTRUCTOR_CALL(ANewReflexActor)//展开
			{
				static void __DefaultConstructor(const FObjectInitializer& X) { new((EInternal*)X.GetObj())ANewReflexActor; }
			}
	}
}

























#endif
