#pragma once
#include "TArray.h"

class FString : public TArray<wchar_t> {
public:
	wchar_t*& GetStr() { return Data; }

	FString() = default;
	FString(const wchar_t* wideStr);

	void Destroy();
};