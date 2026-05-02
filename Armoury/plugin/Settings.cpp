#include "pch.h"
#include "Settings.h"
#include "../utils/IniReader.h"

eSettingsManager::eSettingsManager()
	: ini(ININAME)
{
	if (ini.HasInited())
	{
		bSkipStartupDialog = ini.ReadBoolean("General", "SkipStartupDialog", false);
		bEnableConsoleWindow = ini.ReadBoolean("Misc", "EnableConsoleWindow", false);
	}
	else
	{
		bEnableConsoleWindow = false;
		bSkipStartupDialog = false;
	}
}
