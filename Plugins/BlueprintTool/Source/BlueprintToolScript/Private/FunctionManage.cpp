#include "FunctionManage.h"

namespace BTMess
{
	void Error(UObject* Context, FBTFrame& TheStack, void const * RefData)
	{
		check(0);
	}

	void Empty(UObject* Context, FBTFrame& TheStack, void const * RefData)
	{
		
	}

	//Event
	void EventNode(UObject* Context, FBTFrame& TheStack, void const * RefData)
	{
		for (TFieldIterator<UProperty> i(TheStack.Function); i; ++i)
		{
			TSharedPtr<FFunctionParameter> *FunctionParameter = (TSharedPtr<FFunctionParameter>*)const_cast<void *>(RefData);
			void const * NewConstAddr = GetEventParaAddr(*FunctionParameter);
			TheStack.Step(Context, NewConstAddr);
		}
	}
}

void AddParaAddr(void const* InAddr, TSharedPtr<FFunctionParameter> &FunctionParameter)
{
	 TSharedPtr<FFunctionParameter> ListParm = FunctionParameter;
	 if (!ListParm.IsValid())
	 {
		 FunctionParameter = MakeShareable(new FFunctionParameter(InAddr));
	 }
	 else
	 {
		 while (ListParm->Nest.IsValid())
		 {
			 ListParm = ListParm->Nest;
		 }

		 ListParm->Nest = MakeShareable(new FFunctionParameter(InAddr));
	 }
}

 void const * GetEventParaAddr(TSharedPtr<FFunctionParameter> &FunctionParameter)
 {
	 void const * ParaAddr = FunctionParameter->ParaAddr;
	 FunctionParameter = FunctionParameter->Nest;

	 return ParaAddr;
 }

FNativeBTFuncPtr ErrorFunction = BTMess::Error;
FNativeBTFuncPtr EmptyFunction = BTMess::Empty;
FNativeBTFuncPtr EventFunction = BTMess::EventNode;

TArray<FBTFuntionID> FFuntionManage::FunctionList;

FNativeBTFuncPtr & FFuntionManage::GetNativeFuncPtr(UFunction *Function)
{
	for (auto &Func : FunctionList)
	{
		if (Function->GetFName() == Func.FuncName && //函数名
			Function->GetOuter()->GetFName() == Func.ClassName) //类名
		{
			return Func.NativeFuncPtr;
		}
	}

	if (Function->GetMetaData("CodeType") == "Event")
	{
		return EventFunction;
	}

	return EmptyFunction;
}

int32 FFuntionManage::SetNativeFuncPtr(const FBTFuntionID &Function)
{
	FunctionList.Add(Function);

	return 0;
}

