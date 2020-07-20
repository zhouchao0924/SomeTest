#pragma once

#include "CoreMinimal.h"

namespace SimpleXML
{
	void Write(const FString &XMLHead, const FString &Attribute, const FString &XMLValue, FString &Content, bool bArray = false);

	FString Write(const FString &XMLHead, const FString &Attribute, const FString &XMLValue, bool bArray = false);

	FString ANSI(int32 i, int32 J);

	FString AorB(const FString &Value, const FString &A, const FString &B, const FString &Condition = "-1");

}