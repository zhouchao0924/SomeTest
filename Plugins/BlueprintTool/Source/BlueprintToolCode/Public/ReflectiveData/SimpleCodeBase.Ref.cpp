//// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
///*===========================================================================
//	Generated code exported from BT UBT.
//	DO NOT modify this manually! Edit the corresponding .h files instead!
//===========================================================================*/
//
//#include "SimpleCodeBase.h"
//#include "FunctionManage.h"
//#include "UObject/GeneratedCppIncludes.h"
//
//#ifdef _MSC_VER
//#pragma warning (push)
//#pragma warning (disable : 4883)
//#endif
//PRAGMA_DISABLE_DEPRECATION_WARNINGS
//
///* As an executable function pointer for global registration
//we skip the UE4 UFund local function registration because there is no bytecode in it.*/
//uint8 Register_USimpleCodeBase()
//{
//	FFuntionManage::SetNativeFuncPtr(FBTFuntionID(TEXT("SimpleCodeBase"),TEXT("PrintString"), USimpleCodeBase::VMC_PrintString));
//	FFuntionManage::SetNativeFuncPtr(FBTFuntionID(TEXT("SimpleCodeBase"),TEXT("SetString"), USimpleCodeBase::VMC_SetString));
//	FFuntionManage::SetNativeFuncPtr(FBTFuntionID(TEXT("SimpleCodeBase"),TEXT("SetBool"), USimpleCodeBase::VMC_SetBool));
//	FFuntionManage::SetNativeFuncPtr(FBTFuntionID(TEXT("SimpleCodeBase"),TEXT("SetFloat"), USimpleCodeBase::VMC_SetFloat));
//	FFuntionManage::SetNativeFuncPtr(FBTFuntionID(TEXT("SimpleCodeBase"),TEXT("SetInt32"), USimpleCodeBase::VMC_SetInt32));
//	FFuntionManage::SetNativeFuncPtr(FBTFuntionID(TEXT("SimpleCodeBase"),TEXT("SetText"), USimpleCodeBase::VMC_SetText));
//	FFuntionManage::SetNativeFuncPtr(FBTFuntionID(TEXT("SimpleCodeBase"),TEXT("SetName"), USimpleCodeBase::VMC_SetName));
//	FFuntionManage::SetNativeFuncPtr(FBTFuntionID(TEXT("SimpleCodeBase"),TEXT("SetColor"), USimpleCodeBase::VMC_SetColor));
//	FFuntionManage::SetNativeFuncPtr(FBTFuntionID(TEXT("SimpleCodeBase"),TEXT("SetByte"), USimpleCodeBase::VMC_SetByte));
//	FFuntionManage::SetNativeFuncPtr(FBTFuntionID(TEXT("SimpleCodeBase"),TEXT("SetObject"), USimpleCodeBase::VMC_SetObject));
//
//	return 0;
//}
//static uint8 USimpleCodeBase_Index = Register_USimpleCodeBase();
//
//PRAGMA_ENABLE_DEPRECATION_WARNINGS
//#ifdef _MSC_VER
//#pragma warning (pop)
//#endif
