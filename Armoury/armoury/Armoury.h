#pragma once
#include <unordered_map>
#include <vector>
#include <string>

struct ArmourEntry
{
	std::string CharacterName;
	std::wstring BundleName;
	std::wstring BundleAsset;
};

struct Armoury
{
public:
	static void LoadArmours(const wchar_t* folderName);
private:
	static bool AddArmourEntry(const std::string& charName, const wchar_t* armourSection, const wchar_t* filePath);
	static std::unordered_map<std::string, std::vector<ArmourEntry>> characterItems;

public:
	// hooks
	static void UAssetManager_UpdateCachedAssetData_Hook(void*, void*, uintptr_t, bool);
	static inline void (*oUAssetManager_UpdateCachedAssetData)(uintptr_t, void*, uintptr_t, bool) = nullptr;

	static void* OfflineInvFirstCheck_Hook(void*);
	static inline void* (*oOfflineInvFirstCheck)(void*) = nullptr;
};