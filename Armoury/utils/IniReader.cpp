#include "pch.h"
#include "IniReader.h"
#include <iostream>

#pragma warning(disable:4996)
EXTERN_C IMAGE_DOS_HEADER __ImageBase;

#define DisplayPathError() { eLog::Message(ELogLevel::Error, "Unable to initialize IniReader! This program does not support paths over 260 characters!"); }

CIniReader::CIniReader(const char* szFileName)
{
	char			moduleName[MAX_PATH];
	char			dllPath[MAX_PATH];
	char			iniName[MAX_PATH];
	char*			tempPointer;

	DWORD nSize = GetModuleFileNameA((HINSTANCE)&__ImageBase, moduleName, MAX_PATH);
	if (nSize == 0 || nSize == MAX_PATH) { DisplayPathError(); return; }

	tempPointer = strrchr(moduleName, '.');
	*tempPointer = '\0';
	tempPointer = strrchr(moduleName, '\\');
	strcpy(iniName, tempPointer + 1);
	if (strcat_s(iniName, MAX_PATH, ".ini")) { DisplayPathError(); return; }
	strncpy(dllPath, moduleName, (tempPointer - moduleName + 1));
	dllPath[tempPointer - moduleName + 1] = '\0';
	if (strcmp(szFileName, "") == 0)
	{
		if (strcat_s(dllPath, MAX_PATH, iniName)) { DisplayPathError(); return; }
	}
	else
	{
		if (strcat_s(dllPath, MAX_PATH, szFileName)) { DisplayPathError(); return; }
	}

	snprintf(m_szFileName, 260, "%s", dllPath);
	swprintf(m_szFileNameW, 260, L"%S", dllPath);
	m_bInited = true;
}

CIniReader::CIniReader(const wchar_t* szFileName)
{
	wchar_t			moduleName[MAX_PATH];
	wchar_t			dllPath[MAX_PATH];
	wchar_t			iniName[MAX_PATH];
	wchar_t*		tempPointer;

	DWORD nSize = GetModuleFileNameW((HINSTANCE)&__ImageBase, moduleName, MAX_PATH);
	if (nSize == 0 || nSize == MAX_PATH) { DisplayPathError(); return; }

	tempPointer = wcsrchr(moduleName, L'.');
	*tempPointer = L'\0';
	tempPointer = wcsrchr(moduleName, L'\\');
	wcscpy(iniName, tempPointer + 1);
	if (wcscat_s(iniName, MAX_PATH, L".ini")) { DisplayPathError(); return; }
	wcsncpy(dllPath, moduleName, (tempPointer - moduleName + 1));
	dllPath[tempPointer - moduleName + 1] = '\0';
	if (wcscmp(szFileName, L"") == 0)
	{
		if (wcscat_s(dllPath, MAX_PATH, iniName)) { DisplayPathError(); return; }
	}
	else
	{
		if (wcscat_s(dllPath, MAX_PATH, szFileName)) { DisplayPathError(); return; }
	}

	snprintf(m_szFileName, 260, "%ws", dllPath);
	swprintf(m_szFileNameW, 260, L"%s", dllPath);
	m_bInited = true;
}

bool CIniReader::HasInited() const { return m_bInited; }

int CIniReader::ReadInteger(const char* szSection, const char* szKey, int iDefaultValue)
{
	int iResult = GetPrivateProfileIntA(szSection,  szKey, iDefaultValue, m_szFileName);
	return iResult;
}

float CIniReader::ReadFloat(const char* szSection, const char* szKey, float fltDefaultValue)
{
	char szResult[255];
	char szDefault[255];
	float fltResult;
	snprintf(szDefault, 255, "%f", fltDefaultValue);
	if (GetPrivateProfileStringA(szSection, szKey, szDefault, szResult, 255, m_szFileName) == 255 - 1)
		return fltDefaultValue;

	fltResult = (float)atof(szResult);
	return fltResult;
}

bool CIniReader::ReadBoolean(const char* szSection, const char* szKey, bool bolDefaultValue)
{
	char szResult[8];
	char szDefault[8];
	bool bolResult;
	snprintf(szDefault, 8, "%s", bolDefaultValue ? "True" : "False");
	if (GetPrivateProfileStringA(szSection, szKey, szDefault, szResult, 8, m_szFileName) == 8 - 1)
		return bolDefaultValue;

	bolResult = _stricmp(szResult, "true") == 0 ? true : false;
	return bolResult;
}

const char* CIniReader::ReadString(const char* szSection, const char* szKey, const char* szDefaultValue)
{
	static thread_local char szResult[255] = {};
	if (GetPrivateProfileStringA(szSection, szKey, szDefaultValue, szResult, 255, m_szFileName) == 255 - 1)
		return szDefaultValue;

	return szResult;
}

const wchar_t* CIniReader::ReadWideString(const wchar_t* szSection, const wchar_t* szKey, const wchar_t* szDefaultValue)
{
	static thread_local wchar_t szResult[255] = {};
	if (GetPrivateProfileStringW(szSection, szKey, szDefaultValue, szResult, 255, m_szFileNameW) == 255 - 1)
		return szDefaultValue;

	return szResult;
}

void CIniReader::WriteInteger(const char* szSection, const char* szKey, int iValue)
{
	char szValue[255];
	snprintf(szValue, 255, "%d", iValue);
	WritePrivateProfileStringA(szSection, szKey, szValue, m_szFileName);
}

void CIniReader::WriteFloat(const char* szSection, const char* szKey, float fltValue)
{
	char szValue[255];
	snprintf(szValue, 255, "%f", fltValue);
	WritePrivateProfileStringA(szSection, szKey, szValue, m_szFileName);
}

void CIniReader::WriteBoolean(const char* szSection, const char* szKey, bool bolValue)
{
	char szValue[8];
	snprintf(szValue, 8, "%s", bolValue ? "True" : "False");
	WritePrivateProfileStringA(szSection, szKey, szValue, m_szFileName);
}

void CIniReader::WriteString(const char* szSection, const char* szKey, const char* szValue)
{
	WritePrivateProfileStringA(szSection, szKey, szValue, m_szFileName);
}

void CIniReader::WriteWideString(const wchar_t* szSection, const wchar_t* szKey, const wchar_t* szValue)
{
	WritePrivateProfileStringW(szSection, szKey, szValue, m_szFileNameW);
}