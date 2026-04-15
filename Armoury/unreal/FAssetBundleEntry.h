#pragma once
#include "FName.h"
#include "TArray.h"
#include "FSoftObjectPath.h"

struct FAssetBundleEntry
{
	FName BundleName;
	TArray<FSoftObjectPath> BundleAssets;
};