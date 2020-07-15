#pragma once
#define BT_VM(func) static void func( UObject* Context, FBTFrame& Stack,void const *RefData)
#define BT_VM_FUNCTION(func) void func( UObject* Context, FBTFrame& Stack,void const *RefData)

#define Add_VM_FUNCTION(BytecodeIndex)\
static uint8 BytecodeIndex##_Native = GBTRegisterNative( (int32)EVMCommand::##BytecodeIndex, &UBTScriptObject::##BytecodeIndex );

#define BODY_MACRO_COMBINE_INNER_BP(A,B,C,D) A##B##C##D
#define BODY_MACRO_COMBINE_BT(A,B,C,D) BODY_MACRO_COMBINE_INNER_BP(A,B,C,D)

#define GENERATED_BT_BODY(...) BODY_MACRO_COMBINE_BT(CURRENT_FILE_ID_BT,_,NewLine,_GENERATED_BODY_BT);