#pragma once

#include "CoreMinimal.h"
#include "BTScript.h"
#include "ScriptMacro.h"
#include "ScriptObject.generated.h"


UCLASS()
class BLUEPRINTTOOLSCRIPT_API UBTScriptObject : public UObject
{
	GENERATED_BODY()

	friend struct FBTCompilationUtilities;
public:
	UBTScriptObject(){}

	//static void VMC_Undefined(UObject* Context, FBTFrame& Stack, void const *RefData)
	BT_VM(VMC_Undefined);
	BT_VM(VMC_LetBool);
	BT_VM(VMC_IntConst);
	BT_VM(VMC_FloatConst);
	BT_VM(VMC_StringConst);
	BT_VM(VMC_ObjectConst);
	BT_VM(VMC_NameConst);
	BT_VM(VMC_RotationConst);
	BT_VM(VMC_VectorConst);
	BT_VM(VMC_ByteConst);
	BT_VM(VMC_True);
	BT_VM(VMC_False);
	BT_VM(VMC_TextConst);
	BT_VM(VMC_NoObject);
	BT_VM(VMC_TransformConst);
	BT_VM(VMC_UnicodeStringConst);
	BT_VM(VMC_Int64Const);
	BT_VM(VMC_UInt64Const);
	BT_VM(VMC_EndOfScript);
	BT_VM(VMC_LetObject);
	BT_VM(VMC_LetName);
	BT_VM(VMC_LetFloat);
	BT_VM(VMC_SaveValue);
	BT_VM(VMC_LetInt);
	BT_VM(VMC_LetInt64);
	BT_VM(VMC_LetUInt64);
	BT_VM(VMC_LetByte);
	BT_VM(VMC_Funtion);
	BT_VM(VMC_LetString);
	BT_VM(VMC_LetText);
	BT_VM(VMC_LetVector);
	BT_VM(VMC_LetRotator);
	BT_VM(VMC_LetTransform);
	
	static void CallBTFunction(FBTFrame& Stack, void const *Data, UFunction* Function);

	static void ExecutionScript(UFunction * Function, void const *Data);

	//该函数可以获取编译后带字节码的函数
	static UFunction *FindScriptFuntion(FName FunName);

private:
	static TSet<UFunction*> FunctionList;
};