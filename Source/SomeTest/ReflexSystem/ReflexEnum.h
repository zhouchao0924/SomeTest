#pragma once

#include "ReflexEnum.generated.h"

UENUM(BlueprintType)
enum class EReflexEnum : uint8
{
	Heath	UMETA(DisplayName = "1"),
	Mana	UMETA(DisplayName = "2"),
	Attack	UMETA(DisplayName = "3"),
	Arm		UMETA(DisplayName = "4"),
	Hello	UMETA(DisplayName = "5"),
	World	UMETA(DisplayName = "6"),
	Max
};

UENUM(BlueprintType)
enum EHello
{
	Heath	UMETA(DisplayName = "1"),
	Mana	UMETA(DisplayName = "2"),
};