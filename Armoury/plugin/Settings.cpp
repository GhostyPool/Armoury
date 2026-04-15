#include "pch.h"
#include "Settings.h"
#include "../utils/IniReader.h"

eSettingsManager::eSettingsManager()
{
	CIniReader ini(ININAME);

	if (ini.HasInited())
	{
		bEnableConsoleWindow = ini.ReadBoolean("Misc", "EnableConsoleWindow", false);
		swprintf(wszArmourFolder, _countof(wszArmourFolder), ini.ReadWideString(L"General", L"ArmourFolder", L"Armours"));
	}
	else
	{
		bEnableConsoleWindow = false;
		wcscpy(wszArmourFolder, L"Armours");
	}
}
