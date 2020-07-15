#include "SimpleCodeBase.h"

void USimpleCodeBase::PrintString(FString String)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 30.f, FColor::Red, String.IsEmpty() ? "Hello BT" : String);
	}
}

FString USimpleCodeBase::SetString(FString Context)
{
	return Context;
}

bool USimpleCodeBase::SetBool(bool Context)
{
	return Context;
}

float USimpleCodeBase::SetFloat(float Context)
{
	return Context;
}

int32 USimpleCodeBase::SetInt32(int32 Context)
{
	return Context;
}

FText USimpleCodeBase::SetText(FText Context)
{
	return Context;
}

FName USimpleCodeBase::SetName(FName Context)
{
	return Context;
}

FLinearColor USimpleCodeBase::SetColor(FLinearColor Context)
{
	return Context;
}

uint8 USimpleCodeBase::SetByte(uint8 Context)
{
	return Context;
}

UObject * USimpleCodeBase::SetObject(UObject *Context)
{
	return Context;
}

