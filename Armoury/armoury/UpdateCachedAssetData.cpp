#include "pch.h"
#include "Armoury.h"
#include "../unreal/FName.h"
#include "../unreal/TArray.h"
#include "../unreal/FSoftObjectPath.h"
#include "../unreal/FPrimaryAssetId.h"
#include "../unreal/FAssetBundleEntry.h"

void Armoury::UAssetManager_UpdateCachedAssetData_Hook(void* a1, void* a2, uintptr_t a3, bool a4)
{
	FPrimaryAssetId* primaryAssetId = reinterpret_cast<FPrimaryAssetId*>(a2);

	if (primaryAssetId->PrimaryAssetType.Name == "MKCharacter" || primaryAssetId->PrimaryAssetType.Name == "MKKameo")
	{
		FString charName;
		primaryAssetId->PrimaryAssetName.ToString(&charName);

		wchar_t* charNameW = charName.GetStr();
		auto it = characterItems.find(std::string(charNameW, charNameW + wcslen(charNameW)));
		if (it != characterItems.end())
		{
			TArray<FAssetBundleEntry>*& bundleEntries = *reinterpret_cast<TArray<FAssetBundleEntry>**>(a3 + 0x30);

			std::vector<ArmourEntry>& armours = it->second;
			size_t newSize = bundleEntries->Count + armours.size();

			if (bundleEntries->Max < newSize)
				bundleEntries->ResizeTo(newSize);

			if (bundleEntries->Max >= newSize)
			{
				for (size_t i = 0; i < armours.size(); ++i)
				{
					ArmourEntry& armourEntry = armours[i];

					TArray<FSoftObjectPath> bundleAssets;
					bundleAssets.ResizeTo(1u);
					bundleAssets.Add(FSoftObjectPath{ FName(armourEntry.BundleAsset.c_str(), FNAME_Add) });

					bundleEntries->Add(FAssetBundleEntry{
						.BundleName = FName(armourEntry.BundleName.c_str(), FNAME_Add),
						.BundleAssets = bundleAssets
					});
				}
			}
			else
				eLog::Message(ELogLevel::Error, "Could not add to bundle entries array!");
		}

		charName.Destroy();
	}

	uintptr_t vtable = *(uintptr_t*)a1;
	(*(void(**)(void*, void*, uintptr_t, bool))(vtable + 1176))(a1, a2, a3, a4);
}