#include "pch.h"
#include "Armoury.h"
#include "minhook/include/MinHook.h"

void* Armoury::OfflineInvFirstCheck_Hook(void* a1)
{
	uintptr_t pat = _pattern(PATID_CVarSetter_Int);
	if (pat)
		((void(*)(const wchar_t*, unsigned int))pat)(L"NRS.MK12Inventory.UseOfflineInventory", 1u);

	MH_DisableHook((void*)_pattern(PATID_OfflineInvFirstCheck));

	return oOfflineInvFirstCheck(a1);
}