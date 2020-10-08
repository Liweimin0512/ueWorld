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
	InventoryData.SetNum(AmountOfInventory);
}


bool UInventorySystemComponent::SaveInventory()
{
	return false;
}

bool UInventorySystemComponent::LoadInventory()
{
	return false;
}


bool UInventorySystemComponent::AddItem(FInventoryItem NewItem, int32 NewAmount, int32& Reset)
{
	UItemDataAsset* ItemData = NewItem.ItemAsset;
	
	int32 foundIndex;
	int32 Rest;

	// Stacked = 0 表示不能堆叠
	if (ItemData->Stacked != 0 )
	{
		if (SearchFreeStack(NewItem,foundIndex))
		{
			// 如果当前还有富裕的堆叠空间,则计算总数是否超过堆叠上限
			int32 totalAmount = InventoryData[foundIndex].ItemAmount + NewAmount;
			if ( totalAmount > ItemData->Stacked )
			{
				InventoryData[foundIndex] = FInventoryItem(ItemData, ItemData->Stacked);
				AddItem(NewItem, totalAmount - ItemData->Stacked, Rest);
				NotifyInventoryItemChanged(true,ItemData);
				return true;
			}
			else
			{
				InventoryData[foundIndex] = FInventoryItem(ItemData, totalAmount);
				NotifyInventoryItemChanged(true, ItemData);
				return true;
			}
		}
		else
		{
			if (SearchEmptyInventorySlot(foundIndex))
			{
				if (NewAmount > ItemData->Stacked)
				{
					//如果一次添加的数量过多，则进行递归操作
					InventoryData[foundIndex] = FInventoryItem(ItemData, ItemData->Stacked);
					
					AddItem(NewItem, NewAmount - ItemData->Stacked, Rest);
					NotifyInventoryItemChanged(true, ItemData);
					return true;
				}
				else
				{
					InventoryData[foundIndex] = FInventoryItem(ItemData, NewAmount);
					NotifyInventoryItemChanged(true, ItemData);
					return true;
				}
			}
			// 找不到空槽就返回失败
			return false;
		}
	}
	else
	{
		if (SearchEmptyInventorySlot(foundIndex))
		{
			InventoryData[foundIndex] = FInventoryItem(ItemData, 1);

			if (NewAmount > 1)
			{
				AddItem(NewItem, NewAmount -1, Rest);
			}
			NotifyInventoryItemChanged(true, ItemData);
			return true;
		}
		// 找不到空槽就返回失败
		return false;
	}
}

// 搜索还有富裕的堆叠空间
bool UInventorySystemComponent::SearchFreeStack(FInventoryItem Item, int32& Index)
{
	UItemDataAsset* ItemData = Item.ItemAsset;
	for (int i = 0; i < InventoryData.Num(); i++)
	{
		if (!IsInventoryEmpty(i))
		{
			if (InventoryData[i].ItemAsset == ItemData && InventoryData[i].ItemAmount < ItemData->Stacked)
			{
				Index = i;
				return true;
			}
		}
	}
	Index = -1;
	return false;
}

// 搜索空的背包格
bool UInventorySystemComponent::SearchEmptyInventorySlot(int32& SlotIndex)
{
	for (int i = 0; i < InventoryData.Num(); i++)
	{
		if (IsInventoryEmpty(i))
		{
			SlotIndex = i;
			return true;
		}
	}
	SlotIndex = -1;
	return false;
}

bool UInventorySystemComponent::AddItemByName(FString ItemName, int32 ItemAmount)
{
	int32 Rset;
	FString ItemAssetPath = FString::Printf(TEXT("/Game/unWorld/Item/%s"), *ItemName);
	UItemDataAsset* ItemData = (UItemDataAsset*)LoadObject<UItemDataAsset>(NULL, *ItemAssetPath);

	if (ItemData)
	{
		return AddItem(FInventoryItem(ItemData,ItemAmount), ItemAmount, Rset);
	}

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

const TArray<FInventoryItem>& UInventorySystemComponent::GetInventoryData() const
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
	if (InventoryData.IsValidIndex(index))
	{
		FInventoryItem foundItem = InventoryData[index];
		if (IsValid(foundItem.ItemAsset)) {
			return foundItem.ItemAmount;
		}
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