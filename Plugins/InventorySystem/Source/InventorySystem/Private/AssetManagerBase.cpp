// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetManagerBase.h"
#include "ItemDataAsset.h"
//#include "AbilitySystemGlobals.h"

const FPrimaryAssetType	UAssetManagerBase::WeaponItemType = TEXT("Weapon");

UAssetManagerBase& UAssetManagerBase::Get()
{
	UAssetManagerBase* This = Cast<UAssetManagerBase>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		//UE_LOG(LogInventory, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be RPGAssetManager!"));
		return *NewObject<UAssetManagerBase>(); // never calls this
	}
}

UItemDataAsset* UAssetManagerBase::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	// This does a synchronous load and may hitch
	UItemDataAsset* LoadedItem = Cast<UItemDataAsset>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		//UE_LOG(LogInventory, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}

void UAssetManagerBase::StartInitialLoading()
{
	Super::StartInitialLoading();

	//UAbilitySystemGlobals::Get().InitGlobalData();
}

