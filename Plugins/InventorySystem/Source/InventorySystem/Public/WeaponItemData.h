// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "WeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UWeaponItemData : public UItemDataAsset
{
	GENERATED_BODY()

public:
    
    UWeaponItemData()
    {
        ItemType = UAssetManagerBase::WeaponItemType;
    }

    /** Weapon actor to spawn */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
    TSubclassOf<AActor> WeaponActor;
};
