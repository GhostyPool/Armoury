#include "pch.h"
#include "PatternSolver.h"
#include <chrono>

uintptr_t PatternSolver::ms_patterns[PATID_Total_Patterns];


uintptr_t PatternSolver::GetPattern(const char* szPattern, int offset)
{
    uintptr_t addr = 0;
    try {
        addr = (uintptr_t)hook::txn::get_pattern(szPattern, offset);
    }
    TXN_CATCH();

    return addr;
}

void PatternSolver::Initialize()
{
    eLog::Message(ELogLevel::Info, "Starting pattern search");

    for (int i = 0; i < PATID_Total_Patterns; i++)
        ms_patterns[i] = 0;

    auto begin = std::chrono::high_resolution_clock::now();

    ms_patterns[PATID_FName_ctor_char] = GetPattern("48 89 5C 24 ? 57 48 83 EC ? 48 89 54 24 ? 41 8B F8", 0);
    ms_patterns[PATID_FName_ctor_wchar] = GetPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 48 89 54 24 ? 33 C9", 0);
    ms_patterns[PATID_FName_ToString] = GetPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? ? ? 48 8B F1 8B F8", 0);

    ms_patterns[PATID_FMemory_QuantizeSize] = GetPattern("4C 8B 0D ? ? ? ? 4D 85 C9 75 ? 48 8B C1", 0);
    ms_patterns[PATID_FMemory_Realloc] = GetPattern("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F1 41 8B D8 48 8B 0D", 0);
    ms_patterns[PATID_FMemory_Free] = GetPattern("48 85 C9 74 ? 53 48 83 EC ? 48 8B D9 48 8B 0D", 0);

    ms_patterns[PATID_OfflineInvFirstCheck] = GetPattern("F6 41 ? ? 75 ? 48 8B 05 ? ? ? ? ? ? 00 75 ? 32 D2 EB ? B2 ? 48 8B 05 ? ? ? ? ? ? 00 75 ? 84 D2 74 ? 48 8B 51 ? 48 81 C2 ? ? ? ? 48 8B 42", 0);
    ms_patterns[PATID_CVarSetter_Int] = GetPattern("48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 8B FA 48 8B 0D ? ? ? ? 48 85 C9 75 ? E8 ? ? ? ? 48 8B 0D ? ? ? ? ? ? ? 41 B0 ? 48 8B D3 FF 90 ? ? ? ? 48 8B D8", 0);

    ms_patterns[PATID_UAssetManager_UpdateCachedAssetData] = GetPattern("FF 90 ? ? ? ? E9 ? ? ? ? 48 C7 44 24 ? 00 00 00 00", 0);

    ms_patterns[PATID_FCurlHttpRequest_ProcessRequest_IsDomainAllowed] = GetPattern("E8 ? ? ? ? 84 C0 0F 84 ? ? ? ? 48 8B 05 ? ? ? ? 48 85 C0 75", 0);

    auto end = std::chrono::high_resolution_clock::now();

    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    auto timeSeconds = std::chrono::duration_cast<std::chrono::seconds>(end - begin);
    eLog::Message(ELogLevel::Info, "Checked %d patterns in %dms (%ds)", PATID_Total_Patterns, time.count(), timeSeconds.count());
}

int PatternSolver::GetNumPatternsOK()
{
    int patternNum = 0;

    for (int i = 0; i < PATID_Total_Patterns; i++)
        if (ms_patterns[i]) patternNum++;
    return patternNum;
}

bool PatternSolver::CheckMissingPatterns()
{
    int missingPatterns = 0;
    for (int i = 0; i < PATID_Total_Patterns; i++)
        if (ms_patterns[i] == 0)
        {
            missingPatterns++;
            eLog::Message(ELogLevel::Error, "Could not find %s!", GetPatternName(i));
        }
    return missingPatterns > 0;
}

const char* PatternSolver::GetPatternName(int id)
{
    if (id >= PATID_Total_Patterns)
        return "UNKNOWN";

    static const char* szPatternNames[PATID_Total_Patterns] = {
        "FName_ctor_char",
        "FName_ctor_wchar",
        "FName_ToString",
        "FMemory_QuantizeSize",
        "FMemory_Realloc",
        "FMemory_Free",
        "OfflineInvFirstCheck",
        "CVarSetter_Int",
        "UAssetManager_UpdateCachedAssetData"
    };   

    return szPatternNames[id];
}

uintptr_t _pattern(EPatternID id)
{
    if (id >= PATID_Total_Patterns)
        return 0;

    return PatternSolver::ms_patterns[id];
}
