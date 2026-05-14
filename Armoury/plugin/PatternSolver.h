#pragma once
#include "..\pch.h"

enum EPatternID {
	PATID_FName_ctor_char,
	PATID_FName_ctor_wchar,
	PATID_FName_ToString,

	PATID_FMemory_QuantizeSize,
	PATID_FMemory_Realloc,
	PATID_FMemory_Free,

	PATID_OfflineInvFirstCheck,
	PATID_CVarSetter_Int,

	PATID_UAssetManager_UpdateCachedAssetData,

	PATID_GetIsKrossplayEnabled,
	PATID_ShouldDisplayKrossplayToggleOnlineSettings,
	PATID_ShouldDisplayKrossplayToggleMainMenu,

	PATID_Total_Patterns
};


class PatternSolver {
public:
	static uintptr_t ms_patterns[PATID_Total_Patterns];

	static uintptr_t GetPattern(const char* szPattern, int offset);

	static void			Initialize();
	static int			GetNumPatternsOK();
	static bool			CheckMissingPatterns();
	static const char*	GetPatternName(int id);
};


uintptr_t _pattern(EPatternID id);