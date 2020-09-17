// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ItemDataAsset.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UInventoryComponent::IsSlotEmpty(int32 SlotIndex)
{
	//return FInventoryItem.IsDataValid(InventoryData[SlotIndex].ItemID);
	return false;
}

void UInventoryComponent::GetItemByIndex(bool& bEmpty, UItemDataAsset*& ItemData, int32& Amount, int32 index)
{
	if (IsSlotEmpty(index))
	{
		bEmpty = false;
		ItemData = nullptr;
		Amount = 0;
	}
	else
	{
		bEmpty = true;
		ItemData = InventoryData[index].ItemID;
		Amount = InventoryData[index].ItemAmount;
	}
}

//bool UInventoryComponent::AddInventoryItem(UItemDataAsset* NewItem, int32 ItemCount, int32 ItemLevel, bool bAutoSlot)
//{
//	bool bChanged = false;
//	if (!NewItem) {
//		// 报错，直接返回
//		UE_LOG(LogInventory, Warning, TEXT("AddInventoryItem: Failed trying to add null Item!"));
//		return false;
//	}
//
//	if (ItemCount <=0 || ItemLevel <= 0)
//	{
//		UE_LOG(LogInventory, Warning, TEXT("AddInventoryItem: Failed trying to add Item %s with negative count or level!"), *NewItem->GetName());
//		return false;
//	}
//
//	FItemData OldData;
//	GetInventoryItemData(NewItem, OldData);
//
//	// Find modified data
//	FItemData NewData = OldData;
//	NewData.UpdateItemData(FItemData(ItemCount,ItemLevel), NewItem->MaxCount,NewItem->MaxLevel);
//
//	if (OldData != NewData)
//	{
//		// If data changed, need to update storage and call callback
//		InventoryData.Add(NewItem, NewData);
//		NotifyInventoryItemChanged(true, NewItem);
//		bChanged = true;
//	}
//
//	if (bAutoSlot)
//	{
//		// Slot Item if required
//		bChanged |= FillEmptySlotWithItem(NewItem);
//	}
//
//	if (bChanged)
//	{
//		//DOTO If anything changed, write to save game
//		//SaveInventory();
//		return true;
//	}
//
//	return false;
//}

//bool UInventoryComponent::RemoveInventoryItem(UItemDataAsset* RemovedItem, int32 RemoveCount)
//{
//	if (!RemovedItem)
//	{
//		//UE_LOG(LogInventory, Warning, TEXT("RemoveInventoryItem: Failed trying to remove null Item!"));
//		return false;
//	}
//
//	FItemData NewData;
//	GetInventoryItemData(RemovedItem,NewData);
//
//	if (!NewData.IsValid())
//	{
//		// 没找到
//		return false;
//	}
//
//	// 省略删除数量，代表删除全部
//	if (RemoveCount <= 0)
//	{
//		NewData.ItemCount = 0;
//	}
//	else
//	{
//		NewData.ItemCount -= RemoveCount;
//	}
//
//	if (NewData.ItemCount > 0)
//	{
//		// 更新数据
//		InventoryData.Add(RemovedItem,NewData);
//	}
//	else
//	{
//		// 完全删除数据，确保没有占用槽位
//		InventoryData.Remove(RemovedItem);
//
//		for (TPair<FItemSlot,UItemDataAsset*> Pair : SlottedItems)
//		{
//			if (Pair.Value == RemovedItem)
//			{
//				Pair.Value = nullptr;
//				//TODO 装备槽改变通知
//				NotifySlottedItemChanged(Pair.Key,Pair.Value);
//			}
//		}
//	}
//
//	NotifyInventoryItemChanged(false, RemovedItem);
//
//	//TODO SaveGame
//
//	return true;
//}

//void UInventoryComponent::GetInventoryItems(TArray<UItemDataAsset*>& Items, FPrimaryAssetType ItemType)
//{
//	for ( const TPair<UItemDataAsset*,FItemData>& Pair : InventoryData)
//	{
//		if (Pair.Key)
//		{
//			FPrimaryAssetId assetId = Pair.Key->GetPrimaryAssetId();
//
//			if (assetId.PrimaryAssetType == ItemType || !ItemType.IsValid())
//			{
//				Items.Add(Pair.Key);
//			}
//		}
//	}
//}
//
//int32 UInventoryComponent::GetInventoryItemCount(UItemDataAsset* Item) const
//{
//	const FItemData* FoundItem = InventoryData.Find(Item);
//
//	if (FoundItem)
//	{
//		return FoundItem->ItemCount;
//	}
//	return 0;
//}

void UInventoryComponent::NotifyInventoryItemChanged(bool bAdded, UItemDataAsset* Item)
{
	// Notify native before blueprint
	OnInventoryItemChangedNative.Broadcast(bAdded, Item);
	OnInventoryItemChanged.Broadcast(bAdded, Item);

	// Call BP update event
	//InventoryItemChanged(bAdded, Item);
}

void UInventoryComponent::NotifySlottedItemChanged(FItemSlot ItemSlot, UItemDataAsset* Item)
{
	OnSlottedItemChangedNative.Broadcast(ItemSlot, Item);
	OnSlottedItemChanged.Broadcast(ItemSlot, Item);

	//SlottedItemChanged(ItemSlot, Item);
}

//bool UInventoryComponent::GetInventoryItemData(UItemDataAsset* Item, FItemData& ItemData) const
//{
//	const FItemData* FoundItem = InventoryData.Find(Item);
//
//	if (FoundItem)
//	{
//		ItemData = *FoundItem;
//		return true;
//	}
//	ItemData = FItemData(0,0);
//	return false;
//}
//
//bool UInventoryComponent::SetSlottedItem(FItemSlot ItemSlot, UItemDataAsset* Item)
//{
//	// Iterate entire inventory because we need to remove from old slot
//	bool bFound = false;
//	for (TPair<FItemSlot, UItemDataAsset*>& Pair : SlottedItems)
//	{
//		if (Pair.Key == ItemSlot)
//		{
//			// Add to new slot
//			bFound = true;
//			Pair.Value = Item;
//			NotifySlottedItemChanged(Pair.Key, Pair.Value);
//		}
//		else if (Item != nullptr && Pair.Value == Item)
//		{
//			// If this item was found in another slot, remove it
//			Pair.Value = nullptr;
//			NotifySlottedItemChanged(Pair.Key, Pair.Value);
//		}
//	}
//
//	if (bFound)
//	{
//		//TODO SaveInventory();
//
//		return true;
//	}
//
//	return false;
//}
//
//UItemDataAsset* UInventoryComponent::GetSlottedItem(FItemSlot ItemSlot) const
//{
//	UItemDataAsset* const* FoundItem = SlottedItems.Find(ItemSlot);
//
//	if (FoundItem)
//	{
//		return *FoundItem;
//	}
//	return nullptr;
//}
//
//void UInventoryComponent::GetSlottedItems(TArray<UItemDataAsset*>& Items, FPrimaryAssetType ItemType, bool bOutputEmptyIndexes)
//{
//	for (TPair<FItemSlot, UItemDataAsset*>& Pair : SlottedItems)
//	{
//		if (Pair.Key.ItemType == ItemType || !ItemType.IsValid())
//		{
//			Items.Add(Pair.Value);
//		}
//	}
//}
//
//
//void UInventoryComponent::FillEmptySlots()
//{
//	bool bShouldSave = false;
//	for (const TPair<UItemDataAsset*,FItemData>& Pair :InventoryData)
//	{
//		bShouldSave |= FillEmptySlotWithItem(Pair.Key);
//	}
//
//	if (bShouldSave)
//	{
//		//TODO 储存数据
//	}
//}
//
//bool UInventoryComponent::FillEmptySlotWithItem(UItemDataAsset* NewItem)
//{
//	// Look for an empty Item slot to fill with this Item
//	FPrimaryAssetType NewItemType = NewItem->GetPrimaryAssetId().PrimaryAssetType;
//	FItemSlot EmptySlot;
//	for (TPair<FItemSlot, UItemDataAsset*>& Pair : SlottedItems)
//	{
//		if (Pair.Key.ItemType == NewItemType)
//		{
//			if (Pair.Value == NewItem)
//			{
//				// Item is already slotted
//				return false;
//			}
//			else if (Pair.Value == nullptr && (!EmptySlot.IsValid() || EmptySlot.SlotNumber > Pair.Key.SlotNumber))
//			{
//				// We found an empty slot worth filling
//				EmptySlot = Pair.Key;
//			}
//		}
//	}
//
//	if (EmptySlot.IsValid())
//	{
//		SlottedItems[EmptySlot] = NewItem;
//		NotifySlottedItemChanged(EmptySlot, NewItem);
//		return true;
//	}
//
//	return false;
//}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InventoryData.Reset(AmountofSlots);
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}