#include "pch.h"
#include "Settings.h"
#include "../utils/IniReader.h"

eSettingsManager::eSettingsManager()
{
	CIniReader ini(ININAME);

	if (ini.HasInited())
	{
		bEnableConsoleWindow = ini.ReadBoolean("Misc", "EnableConsoleWindow", false);
	}
	else
	{
		bEnableConsoleWindow = false;
	}
}
