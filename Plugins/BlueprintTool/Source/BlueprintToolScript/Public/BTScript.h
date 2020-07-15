// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

enum EVMCommand
{
	VMC_Undefined			= 0x03, // Undefined

	VMC_LetBool				= 0x14, // Let boolean variable.
	VMC_IntConst			= 0x1D,	// Int constant.^
	VMC_FloatConst			= 0x1E,	// Floating point constant.
	VMC_StringConst			= 0x1F,	// String constant.
	VMC_ObjectConst		    = 0x20,	// An object constant.
	VMC_NameConst			= 0x21,	// A name constant.
	VMC_RotationConst		= 0x22,	// A rotation constant.
	VMC_VectorConst			= 0x23,	// A vector constant.
	VMC_ByteConst			= 0x24,	// A byte constant.
	VMC_True				= 0x27,	// Bool True.
	VMC_False				= 0x28,	// Bool False.
	VMC_TextConst			= 0x29, // FText constant
	VMC_NoObject			= 0x2A,	// NoObject.
	VMC_TransformConst		= 0x2B, // A transform constant
	VMC_UnicodeStringConst   = 0x34, // Unicode string constant.
	VMC_Int64Const			= 0x35,	// 64-bit integer constant.
	VMC_UInt64Const			= 0x36,	// 64-bit unsigned integer constant.
	VMC_EndOfScript			= 0x53, // Last byte in script code	
	VMC_LetObject			= 0x5F,	// assign to any object ref pointer
	VMC_LetName				= 0x70,
	VMC_LetFloat			= 0x71,
	VMC_SaveValue			= 0x72,	// Take out String constant.
	VMC_LetInt				= 0x73, // Last int in script code
	VMC_LetInt64			= 0x74,
	VMC_LetUInt64			= 0x75,
	VMC_LetByte				= 0x76,
	VMC_Funtion				= 0x77,	// Call funtion as frist
	VMC_LetString			= 0x82,	// Let String constant.
	VMC_LetText				= 0x84,	// Let Text constant.
	VMC_LetVector			= 0x86,	// Let Vector constant.
	VMC_LetRotator			= 0x88,	// Let Rotator constant.
	VMC_LetTransform		= 0x8A,	// Let Transform constant.
	VMC_Max					= 0x100,
};
class FBTOutParm :public TSharedFromThis<FBTOutParm>
{
public:
	void const*      PropAddr;
	TSharedPtr<FBTOutParm> Nest;
};

//ÐéÄâ»úÕ»
struct FBTFrame
{
	FBTFrame(UFunction *NewFunction);

	void AddOutParm(void const *Addr);
	template<typename TNumericType>
	TNumericType ReadInt();
	float ReadFloat();
	UProperty *ReadProperty();
	FName ReadName();
	FGuid ReadPinID();
	UObject* ReadObject();
	void Step(UObject* Context, void const * RefData);
	_declspec(dllexport) uint8 *GetParmAddr();
	void ClearParm();
public:
	uint8* Code;
	UFunction *Function;

protected:
	TSharedPtr<FBTOutParm> BTOutParm;
};

template<typename TNumericType>
TNumericType FBTFrame::ReadInt()
{
	TNumericType Result = *(TNumericType*)Code;

	Code += sizeof(TNumericType);

	return Result;
}

typedef void(*FNativeBTFuncPtr)(UObject* Context, FBTFrame& TheStack, void const * RefData);
