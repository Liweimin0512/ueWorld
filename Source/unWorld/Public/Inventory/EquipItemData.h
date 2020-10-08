// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/ItemDataAsset.h"
#include "EquipItemData.generated.h"

/**
 * 
 */
UCLASS()
class UNWORLD_API UEquipItemData : public UItemDataAsset
{
	GENERATED_BODY()
	
public:
	
	/** Constructor */
	UEquipItemData()
	{
		ItemType = UAssetManagerBase::EquipItemType;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equip)
	USkeleton* EquipMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equip)
	int32 EquipType;
};
