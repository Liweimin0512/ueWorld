// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerController.h"

void ARPGPlayerController::AddItem(FString ItemName, int32 ItemAmount)
{
	FString ItemAssetPath = FString::Printf(TEXT("/Game/unWorld/Item/%s"), *ItemName);
	UItemDataAsset* ItemData = (UItemDataAsset*)LoadObject<UItemDataAsset>(NULL, *ItemAssetPath);
	FString strName = ItemName;
	if (ItemData)
	{
		FString ItemDataName = ItemData->ItemName.ToString();
		UKismetSystemLibrary::PrintString(this, *ItemDataName);
	}
}
