#pragma once
#define OFF_REFLEXACTOR 1

#if OFF_REFLEXACTOR
#include "GameFramework/Actor.h"
#include "ReflexActor.generated.h"
class AReflexActor : public AActor
{
	//GENERATED_BODY()
	// This pair of macros is used to help implement GENERATED_BODY() and GENERATED_USTRUCT_BODY()
	//#define BODY_MACRO_COMBINE_INNER(A,B,C,D) A##B##C##D
	//#define BODY_MACRO_COMBINE(A,B,C,D) BODY_MACRO_COMBINE_INNER(A,B,C,D)

	// Include a redundant semicolon at the end of the generated code block, so that intellisense parsers can start parsing
	// a new declaration if the line number/generated code is out of date.
	//#define GENERATED_BODY_LEGACY(...) BODY_MACRO_COMBINE(CURRENT_FILE_ID,_,__LINE__,_GENERATED_BODY_LEGACY);
	//#define GENERATED_BODY(...) BODY_MACRO_COMBINE(CURRENT_FILE_ID,_,__LINE__,_GENERATED_BODY);
	//CURRENT_FILE_ID在generated文件中找，__LINE__行数
	//SomeTest_Source_SomeTest_ReflexSystem_ReflexActor_h_12_GENERATED_BODY//第一次宏替换后的内容

//2
//////////////////////////////////////////////////////////////////////////////////////
//public:
//	SomeTest_Source_SomeTest_ReflexSystem_ReflexActor_h_12_PRIVATE_PROPERTY_OFFSET//空
//	SomeTest_Source_SomeTest_ReflexSystem_ReflexActor_h_12_SPARSE_DATA//空
//	SomeTest_Source_SomeTest_ReflexSystem_ReflexActor_h_12_RPC_WRAPPERS_NO_PURE_DECLS//空
//	SomeTest_Source_SomeTest_ReflexSystem_ReflexActor_h_12_INCLASS_NO_PURE_DECLS
//	SomeTest_Source_SomeTest_ReflexSystem_ReflexActor_h_12_ENHANCED_CONSTRUCTORS

//添加函数后
public: 
	//SomeTest_Source_SomeTest_ReflexSystem_ReflexActor_h_12_PRIVATE_PROPERTY_OFFSET 
	//SomeTest_Source_SomeTest_ReflexSystem_ReflexActor_h_12_SPARSE_DATA 
	//SomeTest_Source_SomeTest_ReflexSystem_ReflexActor_h_12_RPC_WRAPPERS_NO_PURE_DECLS 
	//SomeTest_Source_SomeTest_ReflexSystem_ReflexActor_h_12_CALLBACK_WRAPPERS 
	//SomeTest_Source_SomeTest_ReflexSystem_ReflexActor_h_12_INCLASS_NO_PURE_DECLS 
	//SomeTest_Source_SomeTest_ReflexSystem_ReflexActor_h_12_ENHANCED_CONSTRUCTORS 

//3
//////////////////////////////////////////////////////////////////////////////////////
//添加函数后
	//SomeTest_Source_SomeTest_ReflexSystem_ReflexActor_h_12_PRIVATE_PROPERTY_OFFSET展开
	FORCEINLINE static uint32 __PPO__MyBoxComponent() { return STRUCT_OFFSET(AReflexActor, MyBoxComponent); } 
	FORCEINLINE static uint32 __PPO__MyBoxComponent2() { return STRUCT_OFFSET(AReflexActor, MyBoxComponent2); }

	//SomeTest_Source_SomeTest_ReflexSystem_ReflexActor_h_12_RPC_WRAPPERS_NO_PURE_DECLS 展开
	virtual void TestFunc5_Implementation(int32 IntValue); 

	//DECLARE_FUNCTION(execTestFunc5) //前缀exec，蓝图调用约定
	static void execTestFunc5(UObject* Context, FFrame& Stack, /* RESULT_DECL */void*const Z_Param__Result)
	{ 
		//P_GET_PROPERTY(UIntProperty, Z_Param_IntValue); 展开
		UIntProperty::TCppType Z_Param_IntValue = UIntProperty::GetDefaultPropertyValue();					
		Stack.StepCompiledIn<UIntProperty>(&Z_Param_IntValue);
		//P_FINISH; 展开
		Stack.Code += !!Stack.Code; /* increment the code ptr unless it is null */
		//P_NATIVE_BEGIN; 展开
		{
			//SCOPED_SCRIPT_NATIVE_TIMER(ScopedNativeCallTimer);展开
			FBlueprintEventTimer::FScopedNativeTimer ScopedNativeCallTimer;

			//P_THIS->TestFunc5_Implementation(Z_Param_IntValue); 展开    1
			//P_THIS_CAST(ThisClass)->TestFunc5_Implementation(Z_Param_IntValue); 展开    2
			//((ThisClass*)P_THIS_OBJECT)->TestFunc5_Implementation(Z_Param_IntValue); 展开    3
			((ThisClass*)(Context))->TestFunc5_Implementation(Z_Param_IntValue); 
			//P_NATIVE_END; 展开
		}
	} 
		
	//DECLARE_FUNCTION(execTestFunc4) 
	static void execTestFunc4(UObject* Context, FFrame& Stack, void*const Z_Param__Result)
	{ 
		//P_FINISH; 
		//P_NATIVE_BEGIN; 
		//P_THIS->TestFunc4(); 
		//P_NATIVE_END; 
		Stack.Code += !!Stack.Code; /* increment the code ptr unless it is null */
		{
			FBlueprintEventTimer::FScopedNativeTimer ScopedNativeCallTimer;
			((ThisClass*)(Context))->TestFunc4();
		}
	} 
		
	//DECLARE_FUNCTION(execTestFunc3) 
	static void execTestFunc3(UObject* Context, FFrame& Stack, void*const Z_Param__Result)
	{ 
		//P_GET_PROPERTY(UIntProperty, Z_Param_IntValue); 
		//P_GET_PROPERTY(UStrProperty, Z_Param_Name); 
		//P_GET_UBOOL(Z_Param_bShowWindows); 
		//P_FINISH;
		//P_NATIVE_BEGIN; 
		//*(FString*)Z_Param__Result = P_THIS->TestFunc3(Z_Param_IntValue, Z_Param_Name, Z_Param_bShowWindows); 
		//P_NATIVE_END; 
		UIntProperty::TCppType Z_Param_IntValue = UIntProperty::GetDefaultPropertyValue();
		Stack.StepCompiledIn<UIntProperty>(&Z_Param_IntValue);

		UStrProperty::TCppType Z_Param_Name = UStrProperty::GetDefaultPropertyValue();
		Stack.StepCompiledIn<UStrProperty>(&Z_Param_Name);

		uint32 Z_Param_bShowWindows32 = 0; 
		bool Z_Param_bShowWindows = false;
		Stack.StepCompiledIn<UBoolProperty>(&Z_Param_bShowWindows32);
		Z_Param_bShowWindows = !!Z_Param_bShowWindows32; // translate the bitfield into a bool type for non-intel platforms

		Stack.Code += !!Stack.Code; /* increment the code ptr unless it is null */

		{
			FBlueprintEventTimer::FScopedNativeTimer ScopedNativeCallTimer;
			*(FString*)Z_Param__Result = ((ThisClass*)(Context))->TestFunc3(Z_Param_IntValue, Z_Param_Name, Z_Param_bShowWindows);
		}
	} 
		
	//DECLARE_FUNCTION(execTestFunc2) 
	static void execTestFunc2(UObject* Context, FFrame& Stack, void*const Z_Param__Result)
	{ 
		//P_FINISH; 
		//P_NATIVE_BEGIN; 
		//*(bool*)Z_Param__Result = P_THIS->TestFunc2(); 
		//P_NATIVE_END; 
		Stack.Code += !!Stack.Code; /* increment the code ptr unless it is null */
		{
			FBlueprintEventTimer::FScopedNativeTimer ScopedNativeCallTimer;//可以理解为追踪执行时间
			*(bool*)Z_Param__Result = ((ThisClass*)(Context))->TestFunc2();
		}
	} 
		
	//DECLARE_FUNCTION(execTestFunc1) 
	static void execTestFunc1(UObject* Context, FFrame& Stack, void*const Z_Param__Result)
	{ 
		//P_FINISH; 
		//P_NATIVE_BEGIN; 
		//P_THIS->TestFunc1(); 
		//P_NATIVE_END; 
		Stack.Code += !!Stack.Code; /* increment the code ptr unless it is null */
		{
			FBlueprintEventTimer::FScopedNativeTimer ScopedNativeCallTimer;
			((ThisClass*)(Context))->TestFunc1();
		}
	}

//SomeTest_Source_SomeTest_ReflexSystem_ReflexActor_h_12_INCLASS_NO_PURE_DECLS展开
private:
	//注册C++原生函数以便暴露给蓝图虚拟机使用
	static void StaticRegisterNativesAReflexActor();

	//为了构建对应的UClass ,是一个辅助函数
	friend struct Z_Construct_UClass_AReflexActor_Statics;
public:
	//基本函数
	//TEXT("/Script/SomeTest") 类所处于的包名 表示该脚本逻辑在SomeTest里面（包括C++ 和 蓝图）
	//NO_API 最终会编译为exe 所以不需要导出宏
	//DECLARE_CLASS(AReflexActor, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/SomeTest"), NO_API)展开
private:
	AReflexActor& operator=(AReflexActor&&);  
	AReflexActor& operator=(const AReflexActor&);  
	NO_API static UClass* GetPrivateStaticClass();
public: 
	/** Bitwise union of #EClassFlags pertaining to this class.*/
	//与此类相关的#EClassFlags的属于这个类别
	//C++ Native类 为了和蓝图里面动态构造区分
	//0 标记为不带任何属性
	enum { StaticClassFlags = (0 | CLASS_Config & ~(CLASS_Intrinsic)) };

	/** Typedef for the base class ({{ typedef-type }}) */
	typedef AActor Super;

	/** Typedef for {{ typedef-type }}. */ 
	typedef AReflexActor ThisClass; 

	/** Returns a UClass object representing this class at runtime */ 
	//在Runtime模式下返回这个对象所属的类
	inline static UClass* StaticClass() 
	{ 
		return GetPrivateStaticClass(); 
	} 

	/** Returns the package this class belongs in */ 
	//返回属于这个类的pak
	inline static const TCHAR* StaticPackage() 
	{ 
		return TEXT("/Script/SomeTest"); \
	} 

	/** Returns the static cast flags for this class */ 
	//返回这个类的标志
	inline static EClassCastFlags StaticClassCastFlags() 
	{ 
		return CASTCLASS_None;
	} 

	/** For internal use only; use StaticConstructObject() to create new objects. */ 
	//仅供内部使用 通过StaticConstructObject()去创建一个新的Objects
	inline void* operator new(const size_t InSize, EInternal InInternalOnly, UObject* InOuter = (UObject*)GetTransientPackage(), FName InName = NAME_None, EObjectFlags InSetFlags = RF_NoFlags) 
	{ 
		return StaticAllocateObject(StaticClass(), InOuter, InName, InSetFlags); 
	} 

	/** For internal use only; use StaticConstructObject() to create new objects. */ 
	//仅供内部使用 通过StaticConstructObject()去创建一个新的Objects
	inline void* operator new(const size_t InSize, EInternal* InMem) 
	{ 
		return (void*)InMem; 
	}

	//DECLARE_SERIALIZER(AReflexActor)展开
	//序列化自己
	friend FArchive &operator<<(FArchive& Ar, AReflexActor*& Res) 
	{ 
		return Ar << (UObject*&)Res; 
	}

	friend void operator<<(FStructuredArchive::FSlot InSlot, AReflexActor*& Res) 
	{ 
		InSlot << (UObject*&)Res; 
	}

//SomeTest_Source_SomeTest_ReflexSystem_ReflexActor_h_12_ENHANCED_CONSTRUCTORS展开
private:
	/** Private move- and copy-constructors, should never be used */
	NO_API AReflexActor(AReflexActor&&);
	NO_API AReflexActor(const AReflexActor&);
public:
	//DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AReflexActor);展开
	/** DO NOT USE. This constructor is for internal usage only for hot-reload purposes. */
	//请勿使用。此构造函数仅供内部使用，用于hot-reload为目的。
	//调用专用热重载构造函数的助手类。
	NO_API AReflexActor(FVTableHelper& Helper);

	//DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AReflexActor);展开
	static UObject* __VTableCtorCaller(FVTableHelper& Helper) 
	{ 
		return new (EC_InternalUseOnlyConstructor, (UObject*)GetTransientPackage(), NAME_None, RF_NeedLoad | RF_ClassDefaultObject | RF_TagGarbageTemp) AReflexActor(Helper); 
	}
	//DEFINE_DEFAULT_CONSTRUCTOR_CALL(AReflexActor)展开
	//FObjectInitializer 调用真正的C++构造函数后，完成UObject创建(初始化属性)的内部类。
	//构造函数包装器 根据名字反射创建对象的时候，需要调用该类的构造函数
	//对应 UCalss 	typedef void		(*ClassConstructorType)				(const FObjectInitializer&);
	static void __DefaultConstructor(const FObjectInitializer& X) 
	{ 
		//GetObj() 构造后，从静态分配对象初始化对象
		//EInternal 仅供内部使用的构造函数
		new((EInternal*)X.GetObj())AReflexActor;
	}



////////////////////////////////////////////////////////////////////////////////////
public:
	// Sets default values for this actor's properties
	AReflexActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};


////////////////////////////////////////////////////////////////////////////////////



//USTRUCT(Blueprintable)
struct FReflexStruct
{
	//GENERATED_USTRUCT_BODY()

	//#define SomeTest_Source_SomeTest_ReflexSystem_ReflexStruct_h_8_GENERATED_BODY 展开
	friend struct Z_Construct_UScriptStruct_FReflexStruct_Statics; //创建结构体反射对象 UScriptStruct
	SOMETEST_API static class UScriptStruct* StaticStruct();//反射结构体

public:
};






#endif