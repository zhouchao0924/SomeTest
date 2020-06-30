#include "SimplePopupUtils.h"
#include "SimplePopupType.h"
#include "Core/UI_SimplePopup.h"


bool FPopupHandle::Sure()
{
	if (SimplePopup.IsValid())
	{
		SimplePopup->Sure();
		return true;
	}

	return false;
}

bool FPopupHandle::Cancel()
{
	if (SimplePopup.IsValid())
	{
		SimplePopup->Cancel();
		return true;
	}

	return false;
}

FPopupHandle SimplePopupUtils::CreatePopup(
	UWorld *InWorld,
	TSubclassOf<UUI_SimplePopup> PopupClass,
	const FText &Content,
	ESimplePopupType SimplePopupType,
	float Life /*= 10.f*/, 
	FSimpleDelegate SureDelegate /*= FSimpleDelegate()*/,
	FSimpleDelegate CancelDelegate /*= FSimpleDelegate()*/)
{
	FPopupHandle Handle;
	if (InWorld && PopupClass)
	{
		if (UUI_SimplePopup *SimplePopupPtr = CreateWidget<UUI_SimplePopup>(InWorld, PopupClass))
		{
			SimplePopupPtr->AddToViewport(100);

			SimplePopupPtr->SimpleCancelDelegate = CancelDelegate;
			SimplePopupPtr->SimpleSureDelegate = SureDelegate;
			SimplePopupPtr->Delay = Life;
			SimplePopupPtr->SimplePopupType = SimplePopupType;
			SimplePopupPtr->SetText(Content);
			SimplePopupPtr->InitPopup();

			Handle.SetSimplePopup(SimplePopupPtr);
		}
	}

	return Handle;
}

void FPopupHandle::SetSimplePopup(UUI_SimplePopup *NewSimplePopup)
{
	SimplePopup = NewSimplePopup;
}