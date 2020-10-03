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


bool UInventorySystemComponent::SaveInventory()
{
	return false;
}

bool UInventorySystemComponent::LoadInventory()
{
	return false;
}

// Called every frame
void UInventorySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInventorySystemComponent::FillEmptySlotWithItem(FInventoryItem NewItem)
{
	return false;
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

int32 UInventorySystemComponent::GetInventoryItemCount(int32 index) const
{
	FInventoryItem foundItem = InventoryData[index];
	if (IsValid(foundItem.ItemAsset)) {
		return foundItem.ItemAmount;
	}
	return int32();
}

bool UInventorySystemComponent::SetSlottedItem(FItemSlot ItemSlot, UItemDataAsset* Item)
{
	// Iterate entire inventory because we need to remove from old slot
	bool bFound = false;
	for (TPair<FItemSlot, UItemDataAsset*>& Pair : SlottedItems)
	{
		if (Pair.Key == ItemSlot)
		{
			// Add to new slot
			bFound = true;
			Pair.Value = Item;
			NotifySlottedItemChanged(Pair.Key, Pair.Value);
		}
		else if (Item != nullptr && Pair.Value == Item)
		{
			// If this item was found in another slot, remove it
			Pair.Value = nullptr;
			NotifySlottedItemChanged(Pair.Key, Pair.Value);
		}
	}

	if (bFound)
	{
		SaveInventory();
		return true;
	}

	return false;
}

UItemDataAsset* UInventorySystemComponent::GetSlottedItem(FItemSlot ItemSlot) const
{
	UItemDataAsset* const* FoundItem = SlottedItems.Find(ItemSlot);

	if (FoundItem)
	{
		return *FoundItem;
	}
	return nullptr;
}


void UInventorySystemComponent::NotifyInventoryItemChanged(bool bAdded, UItemDataAsset* Item)
{
	// Notify native before blueprint
	OnInventoryItemChangedNative.Broadcast(bAdded, Item);
	OnInventoryItemChanged.Broadcast(bAdded, Item);

	// Call BP update event
	InventoryItemChanged(bAdded, Item);
}

void UInventorySystemComponent::NotifySlottedItemChanged(FItemSlot ItemSlot, UItemDataAsset* Item)
{
	// Notify native before blueprint
	OnSlottedItemChangedNative.Broadcast(ItemSlot, Item);
	OnSlottedItemChanged.Broadcast(ItemSlot, Item);

	// Call BP update event
	SlottedItemChanged(ItemSlot, Item);
}

void UInventorySystemComponent::NotifyInventoryLoaded()
{
	// Notify native before blueprint
	OnInventoryLoadedNative.Broadcast();
	OnInventoryLoaded.Broadcast();
}