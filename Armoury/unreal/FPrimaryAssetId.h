#pragma once
#include "FName.h"

struct FPrimaryAssetType
{
	FName Name;
};

struct FPrimaryAssetId
{
	FPrimaryAssetType PrimaryAssetType;
	FName PrimaryAssetName;
};