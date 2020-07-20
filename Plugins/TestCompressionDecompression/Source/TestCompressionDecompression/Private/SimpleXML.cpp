#include "SimpleXML.h"

void SimpleXML::Write(const FString &XMLHead, const FString &Attribute, const FString &XMLValue, FString &Content, bool bArray /*= false*/)
{
	Content += TEXT("<") + XMLHead + TEXT(" ") + Attribute + TEXT(">") +
		FString(bArray ? TEXT("\n") : TEXT("")) + XMLValue +
		TEXT("</") + XMLHead + TEXT(">\n");
}

FString SimpleXML::Write(const FString &XMLHead, const FString &Attribute, const FString &XMLValue, bool bArray /*= false*/)
{
	return TEXT("<") + XMLHead + TEXT(" ") + Attribute + TEXT(">") +
		FString(bArray ? TEXT("\n") : TEXT("")) + XMLValue +
		TEXT("</") + XMLHead + TEXT(">\n");
}

FString SimpleXML::ANSI(int32 I, int32 J)
{
	ANSICHAR IndexA = I + 65;
	TCHAR ANSIChar = FString(&IndexA).GetCharArray()[0];

	return FString(&ANSIChar) + FString::FromInt(J + 1);
}

FString SimpleXML::AorB(const FString &Value, const FString &A, const FString &B, const FString &Condition /*= "-1"*/)
{
	return Value == Condition ? B : A;
}