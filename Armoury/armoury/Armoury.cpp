#include "pch.h"
#include "Armoury.h"

std::unordered_map<std::string, std::vector<ArmourEntry>> Armoury::characterItems =
{
	{ "Ashrah", {} },
	{ "Baraka", {} },
	{ "Geras", {} },
	{ "Havik", {} },
	{ "JohnnyCage", {} },
	{ "Kenshi", {} },
	{ "Kitana", {} },
	{ "KungLao", {} },
	{ "LiMei", {} },
	{ "LiuKang", {} },
	{ "Mileena", {} },
	{ "Nitara", {} },
	{ "Raiden", {} },
	{ "RainMage", {} },
	{ "Reiko", {} },
	{ "Reptile", {} },
	{ "Scorpion", {} },
	{ "ShangTsung", {} },
	{ "ShaoKahn", {} },
	{ "Sindel", {} },
	{ "Smoke", {} },
	{ "SubZero", {} },
	{ "Tanya", {} },
	{ "Omniman", {} },
	{ "QuanChi", {} },
	{ "Peacemaker", {} },
	{ "Ermac", {} },
	{ "Homelander", {} },
	{ "Cyrax", {} },
	{ "Sektor", {} },
	{ "NoobSaibot", {} },
	{ "Ghostface", {} },
	{ "Conan", {} },
	{ "T1000", {} },

	// kameos
	{ "CyraxKAM", {} },
	{ "DarriusKAM", {} },
	{ "FrostKAM", {} },
	{ "GoroKAM", {} },
	{ "JaxKAM", {} },
	{ "KanoKAM", {} },
	{ "KungLaoKAM", {} },
	{ "MotaroKAM", {} },
	{ "SareenaKAM", {} },
	{ "ScorpionKAM", {} },
	{ "SektorKAM", {} },
	{ "ShujinkoKAM", {} },
	{ "SonyaKAM", {} },
	{ "StrykerKAM", {} },
	{ "SubZeroKAM", {} },
	{ "TremorKAM", {} },
	{ "KhameleonKAM", {} },
	{ "JanetCageKAM", {} },
	{ "MavadoKAM", {} },
	{ "FerraKAM", {} },
	{ "MadamBoKAM", {} }
};

#define PrintUnableToReadMsg(fieldName, armourSection, filePath) eLog::Message(ELogLevel::Error, "Could not read \"%s\" field from %ws at path \"%ws\", will be skipped. This program does not support paths over 260 characters!", fieldName, armourSection, filePath);

bool Armoury::AddArmourEntry(const std::string& charName, const wchar_t* armourSection, const wchar_t* filePath)
{
	auto it = characterItems.find(charName);
	if (it != characterItems.end())
	{
		wchar_t bundleName[MAX_PATH];
		if (GetPrivateProfileStringW(armourSection, L"BundleName", L"", bundleName, MAX_PATH, filePath) == MAX_PATH - 1)
		{
			PrintUnableToReadMsg("BundleName", armourSection, filePath);
			return false;
		}
		if (wcscmp(bundleName, L"") == 0)
		{
			eLog::Message(ELogLevel::Warning, "%ws at path: \"%ws\" specifies an empty \"BundleName\" field, will be skipped.", armourSection, filePath);
			return false;
		}

		wchar_t bundleAsset[MAX_PATH];
		if (GetPrivateProfileStringW(armourSection, L"BundleAsset", L"", bundleAsset, MAX_PATH, filePath) == MAX_PATH - 1)
		{
			PrintUnableToReadMsg("BundleAsset", armourSection, filePath);
			return false;
		}
		if (wcscmp(bundleAsset, L"") == 0)
		{
			eLog::Message(ELogLevel::Warning, "%ws at path \"%ws\" specifies an empty \"BundleAsset\" field, will be skipped.", armourSection, filePath);
			return false;
		}

		it->second.emplace_back(ArmourEntry{
			.CharacterName = charName,
			.BundleName = bundleName,
			.BundleAsset = bundleAsset
		});
	}
	else
	{
		eLog::Message(ELogLevel::Warning, "%ws at path \"%ws\" specifies an invalid \"Character\" field, will be skipped.", armourSection, filePath);
		return false;
	}

	return true;
}

void Armoury::LoadArmours()
{
	wchar_t moduleName[MAX_PATH] = {};
	DWORD nSize = GetModuleFileNameW(nullptr, moduleName, MAX_PATH);
	if (nSize == 0 || nSize == MAX_PATH)
	{
		eLog::Message(ELogLevel::Error, "Unable to get module path! This program does not support paths over 260 characters!");
		return;
	}

	for (int i = 0; i < 3; ++i)
	{
		wchar_t* slashPtr = wcsrchr(moduleName, L'\\');
		*slashPtr = L'\0';
	}

	wcscat(moduleName, L"\\Mods\\*");

	WIN32_FIND_DATAW fd;
	HANDLE hFind = FindFirstFileW(moduleName, &fd);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && (wcscmp(fd.cFileName, L".") != 0 && wcscmp(fd.cFileName, L"..") != 0))
			{
				wchar_t filePath[MAX_PATH];
				wcscpy(filePath, moduleName);

				wchar_t* ptr = wcsrchr(filePath, L'\\');
				++ptr;
				*ptr = L'\0';

				if (wcscat_s(filePath, MAX_PATH, fd.cFileName) || wcscat_s(filePath, MAX_PATH, L"\\") || wcscat_s(filePath, MAX_PATH, fd.cFileName) || wcscat_s(filePath, MAX_PATH, L".armour"))
				{
					eLog::Message(ELogLevel::Error, "Unable to access folder: %ws! This program does not support paths over 260 characters!", fd.cFileName);
					continue;
				}

				wchar_t charNameW[MAX_PATH];
				int i = 0;
				do
				{
					wchar_t armourSectionW[512];
					swprintf(armourSectionW, _countof(armourSectionW), L"Armour%d", i);

					if (GetPrivateProfileStringW(armourSectionW, L"Character", L"", charNameW, MAX_PATH, filePath) == MAX_PATH - 1)
					{
						PrintUnableToReadMsg("Character", armourSectionW, filePath);
						break;
					}
					else if (wcscmp(charNameW, L"") == 0) // file not found or invalid
						break;

					if (AddArmourEntry(std::string(charNameW, charNameW + wcslen(charNameW)), armourSectionW, filePath))
						eLog::Message(ELogLevel::Info, "Successfully added %ws from: %ws.", armourSectionW, fd.cFileName);

					++i;
				} while (wcscmp(charNameW, L"") != 0);
			}
		} while (FindNextFileW(hFind, &fd));

		FindClose(hFind);
	}
}