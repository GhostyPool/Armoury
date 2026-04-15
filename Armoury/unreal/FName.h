#pragma once
#include "FString.h"

enum EFindName
{
	FNAME_Find,
	FNAME_Add,
};


class FName {
public:
	int Index;
	int Number;

	FString* ToString(FString* str) const;

	FName() : Index(0), Number(0) {}
	FName(const char* Name, EFindName FindType);
	FName(const wchar_t* Name, EFindName FindType);

	bool operator==(const char* str) const { return operator==(FName(str, FNAME_Add)); }
	bool operator==(const wchar_t* strW) const { return operator==(FName(strW, FNAME_Add)); }

	bool operator==(const FName& Other) const { return Index == Other.Index && Number == Other.Number; }
	bool operator!=(const FName& Other) const { return Index != Other.Index || Number != Other.Number; }
};
