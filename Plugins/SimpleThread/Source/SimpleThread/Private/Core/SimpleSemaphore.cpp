#include "Core/SimpleSemaphore.h"
#include "HAL/Event.h"
#include "SimpleTreadPlatform.h"

FSimpleSemaphore::FSimpleSemaphore()
	:Event(FPlatformProcess::GetSynchEventFromPool())
	,bWait(false)
{

}

FSimpleSemaphore::~FSimpleSemaphore()
{
	FPlatformProcess::ReturnSynchEventToPool(Event);
}

void FSimpleSemaphore::Wait()
{
	bWait = true;
	Event->Wait();
}

void FSimpleSemaphore::Wait(uint32 WaitTime, const bool bIgnoreThreadIdleStats)
{
	bWait = true;
	Event->Wait(WaitTime, bIgnoreThreadIdleStats);
}

void FSimpleSemaphore::Trigger()
{
	Event->Trigger();
	bWait = false;
}