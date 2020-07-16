#include "Windows/WindowsPlatformThread.h"

#if PLATFORM_WINDOWS

THIRD_PARTY_INCLUDES_START
#pragma warning(push)
#pragma warning(disable:4005)
#include <d2d1.h>
#include <strsafe.h>
#include <wincodec.h>
#pragma warning(pop)
THIRD_PARTY_INCLUDES_END
#pragma optimize("",off) 
#pragma comment( lib, "windowscodecs.lib" )		

static HANDLE WindowsRowThread = NULL;
FSimpleDelegate FWindowsPlatformThread::CompletedDelegate;
FSimpleDelegate FWindowsPlatformThread::RunDelegate;

//Ïß³Ìº¯Êý
uint32 WINAPI StartThread(LPVOID unused)
{
	FWindowsPlatformThread::RunDelegate.ExecuteIfBound();

	FWindowsPlatformThread::CompletedDelegate.ExecuteIfBound();

	{
		FWindowsPlatformThread::RunDelegate.Unbind();
		FWindowsPlatformThread::CompletedDelegate.Unbind();
	}

	FWindowsPlatformThread::Hide();
	return 0;
}

void FWindowsPlatformThread::Show()
{
	if (!WindowsRowThread)
	{
		DWORD ThreadID = 0;
		WindowsRowThread = 
		CreateThread(
			NULL, 
			128 * 1024,
			(LPTHREAD_START_ROUTINE)StartThread, 
			(LPVOID)NULL, 
			STACK_SIZE_PARAM_IS_A_RESERVATION,
			&ThreadID);
	}
}

void FWindowsPlatformThread::Hide()
{
	if (WindowsRowThread)
	{
		CloseHandle(WindowsRowThread);
		WindowsRowThread = nullptr;
	}
}

bool FWindowsPlatformThread::IsShown()
{
	return (WindowsRowThread != nullptr);
}
#pragma optimize("",on) 
#endif