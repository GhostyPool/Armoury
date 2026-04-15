// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "armoury/Armoury.h"
#include "plugin/Settings.h"
#include "minhook/include/MinHook.h"

static bool ValidateGameVersion();

extern "C" __declspec(dllexport) void InitializeASI()
{
    MH_Initialize();
    eLog::Initialize();
    PatternSolver::Initialize();

    if (!ValidateGameVersion())
        return;

    if (SettingsMgr.bEnableConsoleWindow)
    {
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }

    eLog::Message(ELogLevel::Info, "%s (%s | %s) Begin!", PROJ_NAME, VERSION, __DATE__);

    Trampoline* tramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));

    MH_CreateHook((void*)_pattern(PATID_OfflineInvFirstCheck), &Armoury::OfflineInvFirstCheck_Hook, (void**)&Armoury::oOfflineInvFirstCheck);
    MH_EnableHook((void*)_pattern(PATID_OfflineInvFirstCheck));

    Memory::VP::InjectHook(_pattern(PATID_UAssetManager_UpdateCachedAssetData), tramp->Jump(Armoury::UAssetManager_UpdateCachedAssetData_Hook));
    Memory::VP::Patch<uint8_t>(_pattern(PATID_UAssetManager_UpdateCachedAssetData), 0xE8);
    Memory::VP::Patch<uint8_t>(_pattern(PATID_UAssetManager_UpdateCachedAssetData) + 5, 0x90);

    Armoury::LoadArmours(SettingsMgr.wszArmourFolder);
}

static bool ValidateGameVersion()
{
    if (PatternSolver::CheckMissingPatterns())
    {
        int result = MessageBoxW(nullptr,
            L"Could not start Armoury!\n\n"
            L"One or more code patterns could not be found. This might indicate the game version is not supported or the plugin has not been updated yet.\n\n"
            L"You might experience bugs or crashes.\n"
            L"Launch anyway?\n",
            L"Armoury",
            MB_ICONWARNING | MB_YESNO | MB_SYSTEMMODAL);

        if (result == IDYES)
            return true;
        else
            ExitProcess(1);
    }
    return true;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

