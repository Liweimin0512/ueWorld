// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerController.h"

void ARPGPlayerController::AddItem(FString ItemName, int32 ItemAmount)
{
	UItemDataAsset* ItemData = (UItemDataAsset*)LoadObject<UItemDataAsset>(NULL, TEXT("/Game/unWorld/Item/%s"), *ItemName);
	FString strName = ItemName;
	if (ItemData)
	{
		FText ItemDataName = ItemData->ItemName;
		FString pathName = ItemDataName.ToString();
		UKismetSystemLibrary::PrintString(this, TEXT(""));
	}
}
