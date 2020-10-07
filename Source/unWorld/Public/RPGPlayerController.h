// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "unWorld.h"
#include "GameFramework/PlayerController.h"
#include "Inventory/ItemDataAsset.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNWORLD_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	UFUNCTION(Exec,Category = Inventory)
	void AddItem(FString ItemName,int32 ItemAmount);
};
