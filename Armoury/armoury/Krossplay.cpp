#include "pch.h"
#include "Armoury.h"
#include "minhook/include/MinHook.h"

bool Armoury::GetIsKrossplayEnabled_Hook(void* a1)
{
	*(bool*)((char*)a1 + 946) = false;

	MH_DisableHook((void*)_pattern(PATID_GetIsKrossplayEnabled));

	return GetIsKrossplayEnabled(a1);
}