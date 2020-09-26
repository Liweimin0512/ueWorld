// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataAsset.h"

bool UItemDataAsset::IsConsumable() const
{
	if (MaxCount <= 0)
	{
		return true;
	}
	return false;
}

FString UItemDataAsset::GetIdentifierString() const
{
	return FPrimaryAssetId().ToString();
}

FPrimaryAssetId UItemDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId();
}
