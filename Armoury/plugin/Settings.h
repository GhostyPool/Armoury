#pragma once

class eSettingsManager {
public:
	eSettingsManager();

	bool bEnableConsoleWindow;
	wchar_t wszArmourFolder[260];
};

inline eSettingsManager SettingsMgr;