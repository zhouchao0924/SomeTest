#pragma once

#include "ScriptMacro.h"
#include "BTScript.h"

#define  Z_BT_USimpleCodeBase \
BT_VM(VMC_PrintString) \
{ \
	FString Z_String_Name = *(FString*) Stack.GetParmAddr(); \
	USimpleCodeBase::PrintString( Z_String_Name); \
} \
BT_VM(VMC_SetString) \
{ \
	FString Z_Context_Name = *(FString*) Stack.GetParmAddr(); \
	*(FString*)RefData = USimpleCodeBase::SetString( Z_Context_Name); \
} \
BT_VM(VMC_SetBool) \
{ \
	bool Z_Context_Name = *(bool*) Stack.GetParmAddr(); \
	*(bool*)RefData = USimpleCodeBase::SetBool( Z_Context_Name); \
} \
BT_VM(VMC_SetFloat) \
{ \
	float Z_Context_Name = *(float*) Stack.GetParmAddr(); \
	*(float*)RefData = USimpleCodeBase::SetFloat( Z_Context_Name); \
} \
BT_VM(VMC_SetInt32) \
{ \
	int32 Z_Context_Name = *(int32*) Stack.GetParmAddr(); \
	*(int32*)RefData = USimpleCodeBase::SetInt32( Z_Context_Name); \
} \
BT_VM(VMC_SetText) \
{ \
	FText Z_Context_Name = *(FText*) Stack.GetParmAddr(); \
	*(FText*)RefData = USimpleCodeBase::SetText( Z_Context_Name); \
} \
BT_VM(VMC_SetName) \
{ \
	FName Z_Context_Name = *(FName*) Stack.GetParmAddr(); \
	*(FName*)RefData = USimpleCodeBase::SetName( Z_Context_Name); \
} \
BT_VM(VMC_SetColor) \
{ \
	FLinearColor Z_Context_Name = *(FLinearColor*) Stack.GetParmAddr(); \
	*(FLinearColor*)RefData = USimpleCodeBase::SetColor( Z_Context_Name); \
} \
BT_VM(VMC_SetByte) \
{ \
	uint8 Z_Context_Name = *(uint8*) Stack.GetParmAddr(); \
	*(uint8*)RefData = USimpleCodeBase::SetByte( Z_Context_Name); \
} \
BT_VM(VMC_SetObject) \
{ \
	UObject* Z_Context_Name = *(UObject**) Stack.GetParmAddr(); \
	*(UObject**)RefData = USimpleCodeBase::SetObject( Z_Context_Name); \
} 

#define USimpleCodeBase_18_GENERATED_BODY_BT \
 Z_BT_USimpleCodeBase

#undef  CURRENT_FILE_ID_BT 
#define CURRENT_FILE_ID_BT USimpleCodeBase
#define NewLine 18
