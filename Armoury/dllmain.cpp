// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "armoury/Armoury.h"
#include "plugin/Settings.h"
#include "minhook/include/MinHook.h"

static bool ConfirmationToRun();
static bool ValidateGameVersion();
extern "C" __declspec(dllexport) void InitializeASI()
{
    if (!SettingsMgr.bSkipStartupDialog && !ConfirmationToRun())
        return;

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

    Memory::VP::ReadCall(_pattern(PATID_FCurlHttpRequest_ProcessRequest_IsDomainAllowed), Armoury::oProcessRequest_IsDomainAllowed);
    Memory::VP::InjectHook(_pattern(PATID_FCurlHttpRequest_ProcessRequest_IsDomainAllowed), tramp->Jump(Armoury::ProcessRequest_IsDomainAllowed_Hook));

    Armoury::LoadArmours();
}

static bool ConfirmationToRun()
{
    int result = MessageBoxW(nullptr,
        L"Armoury is a game modification that enables custom items to be used, such as skins and gear.\n\n"
        L"In order to achieve this, the game will be launched in a semi-offline state, which disables and affects certain online features, such as:\n"
        L"Online casual, koth and ranked play, online inventory synchronization, online message receival, online reward receival, and other online features.\n"
        L"Private matches and Towers of Time/Invasions remain playable.\n\n"
        L"Online features will become fully available once you launch the game without Armoury.\n\n"
        L"Launch with Armoury enabled?\n"
        L"(This dialog may be disabled from Armoury.ini)\n",
        L"Armoury",
        MB_ICONINFORMATION | MB_YESNO);

    if (result == IDYES)
        return true;
        
    return false;
}

static bool ValidateGameVersion()
{
    if (PatternSolver::CheckMissingPatterns())
    {
        int result = MessageBoxW(nullptr,
            L"Could not start Armoury!\n\n"
            L"One or more code patterns could not be found. This either means the game version you're using is not supported or an update broke this plugin and it has not been updated yet.\n\n"
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

