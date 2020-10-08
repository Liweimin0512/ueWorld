// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGPlayerController.h"
#include "unWorldCharacter.h"
#include "Kismet/GameplayStatics.h"

void ARPGPlayerController::AddItem(FString ItemName, int32 ItemAmount)
{
	AunWorldCharacter* PlayerCharacter = Cast<AunWorldCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (PlayerCharacter)
	{
		UInventorySystemComponent* InventoryComponent = PlayerCharacter->InventoryComponent;
		InventoryComponent->AddItemByName(ItemName, ItemAmount);
	}
}
