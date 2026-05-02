#pragma once
#include "../utils/IniReader.h"

class eSettingsManager {
public:
	eSettingsManager();


	bool bSkipStartupDialog;
	bool bEnableConsoleWindow;
private:
	CIniReader ini;
};

inline eSettingsManager SettingsMgr;