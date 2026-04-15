#include "pch.h"
#include "FString.h"

FString::FString(const wchar_t* wideStr)
{
	if (wideStr)
	{
		int strLength = wcslen(wideStr) + 1;
		ResizeTo(strLength);
		if (Max >= strLength)
		{
			Count = strLength;
			std::memcpy(Data, wideStr, strLength * sizeof(wchar_t));
		}
		else
			eLog::Message(ELogLevel::Error, "Could not allocate space for FString construction!");
	}
}

void FString::Destroy()
{
	static uintptr_t patFMemoryFree = _pattern(PATID_FMemory_Free);
	if (patFMemoryFree)
	{
		if (Data)
			((void(*)(void*))patFMemoryFree)(Data);
	}
}