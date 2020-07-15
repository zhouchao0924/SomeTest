#pragma once

#include "ScriptMacro.h"
#include "BTScript.h"

#define  Z_BT_USimpleCodeLibrary \
BT_VM(VMC_SetLocation) \
{ \
	UStaticMeshComponent* Z_Target_Name = *(UStaticMeshComponent**) Stack.GetParmAddr(); \
	FVector Z_InLocation_Name = *(FVector*) Stack.GetParmAddr(); \
	USimpleCodeLibrary::SetLocation( Z_Target_Name, Z_InLocation_Name); \
} \
BT_VM(VMC_SetRotator) \
{ \
	UStaticMeshComponent* Z_Target_Name = *(UStaticMeshComponent**) Stack.GetParmAddr(); \
	FRotator Z_InRot_Name = *(FRotator*) Stack.GetParmAddr(); \
	USimpleCodeLibrary::SetRotator( Z_Target_Name, Z_InRot_Name); \
} \
BT_VM(VMC_SetMesh) \
{ \
	UStaticMeshComponent* Z_Target_Name = *(UStaticMeshComponent**) Stack.GetParmAddr(); \
	UStaticMesh* Z_InMesh_Name = *(UStaticMesh**) Stack.GetParmAddr(); \
	USimpleCodeLibrary::SetMesh( Z_Target_Name, Z_InMesh_Name); \
} \
BT_VM(VMC_SetMaterial) \
{ \
	UStaticMeshComponent* Z_Target_Name = *(UStaticMeshComponent**) Stack.GetParmAddr(); \
	int32 Z_Index_Name = *(int32*) Stack.GetParmAddr(); \
	UMaterialInterface* Z_InMaterial_Name = *(UMaterialInterface**) Stack.GetParmAddr(); \
	USimpleCodeLibrary::SetMaterial( Z_Target_Name, Z_Index_Name, Z_InMaterial_Name); \
} \
BT_VM(VMC_SetMaterialColor) \
{ \
	UStaticMeshComponent* Z_Target_Name = *(UStaticMeshComponent**) Stack.GetParmAddr(); \
	FName Z_ColorName_Name = *(FName*) Stack.GetParmAddr(); \
	FLinearColor Z_InColor_Name = *(FLinearColor*) Stack.GetParmAddr(); \
	int32 Z_MatralIndex_Name = *(int32*) Stack.GetParmAddr(); \
	USimpleCodeLibrary::SetMaterialColor( Z_Target_Name, Z_ColorName_Name, Z_InColor_Name, Z_MatralIndex_Name); \
} 

#define USimpleCodeLibrary_18_GENERATED_BODY_BT \
 Z_BT_USimpleCodeLibrary

#define CURRENT_FILE_ID_BT USimpleCodeLibrary
#define NewLine 18
