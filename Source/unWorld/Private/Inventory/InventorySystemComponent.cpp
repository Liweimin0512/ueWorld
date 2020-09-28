// Fill out your copyright notice in the Description page of Project Settings.

#include "inventory/InventorySystemComponent.h"

// Sets default values for this component's properties
UInventorySystemComponent::UInventorySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...


}


// Called when the game starts
void UInventorySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AmountOfInventory = 40;
	InventoryData.SetNumUninitialized(AmountOfInventory);
}


// Called every frame
void UInventorySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

const TArray<FInventoryItem>& UInventorySystemComponent::GetInventoryDataMap() const
{
	// TODO: insert return statement here
	return InventoryData;
}

const TMap<FItemSlot, UItemDataAsset*>& UInventorySystemComponent::GetSlottedItemMap() const
{
	// TODO: insert return statement here
	return SlottedItems;
}

bool UInventorySystemComponent::IsInventoryEmpty(int32 SlotIndex) const
{
	FInventoryItem SlotData = 	InventoryData[SlotIndex];
	return !IsValid(SlotData.ItemAsset);	
}

void UInventorySystemComponent::GetItemByIndex(int32 index, bool& bEmpty, UItemDataAsset*& item, int32& Amount) const
{
	if (IsInventoryEmpty(index))
	{
		bEmpty = true;
		item = nullptr;
		Amount = 0;
	}
	else
	{
		bEmpty = false;
		item = InventoryData[index].ItemAsset;
		Amount = InventoryData[index].ItemAmount;
	}
}

