// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipItemData.h"
#include "WeaponItemData.generated.h"

/**
 * ������һ������װ��
 */
UCLASS()
class UNWORLD_API UWeaponItemData : public UEquipItemData
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
