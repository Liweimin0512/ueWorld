// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "unWorld.h"
#include "UObject/NoExportTypes.h"
#include "Inventory/InventoryType.h"
#include "AbilityEventData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNWORLD_API UAbilityEventData : public UObject
{
	GENERATED_BODY()
public:
	UAbilityEventData();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FInventoryItem ItemData;
};

