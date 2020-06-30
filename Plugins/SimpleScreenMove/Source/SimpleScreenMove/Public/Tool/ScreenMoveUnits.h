#pragma once

#include "CoreMinimal.h"

enum EScreenMoveState
{
	Screen_None,
	Screen_UP,
	Screen_Down,
	Screen_Right,
	Screen_Left,
	Screen_RightAndUP,
	Screen_RightAndDown,
	Screen_LeftAndUP,
	Screen_LeftAndDown,
	Screen_MAX
};

class APlayerController;
struct SIMPLESCREENMOVE_API FScreenMoveUnits
{
	bool ListenScreenMove(APlayerController *PlayerController, float ScreenMoveSpeed = 100.f);

protected:
	EScreenMoveState CursorMove(const APlayerController *PlayerController);

	bool MoveDirection(APlayerController *PlayerController, EScreenMoveState ScreenMoveState, float ScreenMoveSpeed = 100.f);
};