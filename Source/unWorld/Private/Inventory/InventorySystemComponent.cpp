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

	if (ItemData->Stacked == 0)
	{
		for (int32 i = 0; i < InventoryData.Num(); i++)
		{
			if (InventoryData[i].ItemAsset == ItemData)
			{
				// �ѵ�����Ϊ0����ʾ��������
				SetInventoryItem(i, FInventoryItem(ItemData, InventoryData[i].ItemAmount + NewAmount));
				return true;
			}
		}
		if (SearchEmptyInventorySlot(foundIndex))
		{
			SetInventoryItem(foundIndex, FInventoryItem(ItemData, NewAmount));
			return true;
		}
		return false;
	}
	else if (ItemData->Stacked != 1 )
	{
		if (SearchFreeStack(ItemData,foundIndex))
		{
			// �����ǰ���и�ԣ�Ķѵ��ռ�,����������Ƿ񳬹��ѵ�����
			int32 totalAmount = InventoryData[foundIndex].ItemAmount + NewAmount;

			if ( totalAmount > ItemData->Stacked )
			{
				SetInventoryItem(foundIndex, FInventoryItem(ItemData, ItemData->Stacked));
				AddItem(NewItem, totalAmount - ItemData->Stacked, Rest);
				//NotifyInventoryItemChanged(true,ItemData);
				return true;
			}
			else
			{
				SetInventoryItem(foundIndex, FInventoryItem(ItemData, totalAmount));
				//NotifyInventoryItemChanged(true, ItemData);
				return true;
			}
		}
		else
		{
			if (SearchEmptyInventorySlot(foundIndex))
			{

				if (NewAmount > ItemData->Stacked)
				{
					//���һ����ӵ��������࣬����еݹ����
					SetInventoryItem(foundIndex, FInventoryItem(ItemData, ItemData->Stacked));
					AddItem(NewItem, NewAmount - ItemData->Stacked, Rest);
					//NotifyInventoryItemChanged(true, ItemData);
					return true;
				}
				else
				{
					SetInventoryItem(foundIndex, FInventoryItem(ItemData, NewAmount));
					//NotifyInventoryItemChanged(true, ItemData);
					return true;
				}
			}
			// �Ҳ����ղ۾ͷ���ʧ��
			return false;
		}
	}
	else
	{
		if (SearchEmptyInventorySlot(foundIndex))
		{
			//InventoryData[foundIndex] = FInventoryItem(ItemData, 1);
			SetInventoryItem(foundIndex, FInventoryItem(ItemData, 1));

			if (NewAmount > 1)
			{
				AddItem(NewItem, NewAmount -1, Rest);
			}
			//NotifyInventoryItemChanged(true, ItemData);
			return true;
		}
		// �Ҳ����ղ۾ͷ���ʧ��
		return false;
	}
}

bool UInventorySystemComponent::RmoveItemByIndex(int32 ItemIndex, int32 ItemAmount)
{
	bool HasFoundItem;
	UItemDataAsset* FoundItemData;
	int32 FoundItemAmount;

	GetItemByIndex(ItemIndex, HasFoundItem, FoundItemData, FoundItemAmount);
	if (HasFoundItem)
	{
		if (FoundItemAmount <= 1)
		{
			if (SetInventoryItem(ItemIndex, FInventoryItem(nullptr, 0)))
			{
				NotifyInventoryItemChanged(false, FoundItemData);
				return true;
			}
			return false;
		}
		else
		{
			if (SetInventoryItem(ItemIndex, FInventoryItem(FoundItemData, FoundItemAmount - ItemAmount)))
			{
				NotifyInventoryItemChanged(false, FoundItemData);
				return true;
			}
			return false;
		}
	}
	return false;
}

// �������и�ԣ�Ķѵ��ռ�
bool UInventorySystemComponent::SearchFreeStack(UItemDataAsset* Item, int32& Index)
{
	for (int32 i = 0; i < InventoryData.Num(); i++)
	{
		if (!IsInventoryEmpty(i) && InventoryData[i].ItemAsset == Item)
		{
			// �ѵ�����Ϊ0����ʾ��������
			//if (InventoryData[i].ItemAsset == Item && Item->Stacked == 0)
			//{
			//	Index = i;
			//	return true;
			//}
			if (InventoryData[i].ItemAmount < Item->Stacked)
			{
				Index = i;
				return true;
			}
		}
	}
	Index = -1;
	return false;
}

// �����յı�����
bool UInventorySystemComponent::SearchEmptyInventorySlot(int32& SlotIndex)
{
	for (int32 i = 0; i < InventoryData.Num(); i++)
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

bool UInventorySystemComponent::RemoveItem(UItemDataAsset* itemData, int32 ItemAmount)
{
	if (ItemAmount < GetInventoryItemCount(itemData))
	{
		return false;
	} 
	
	for (int32 i=0;i<InventoryData.Num();i++)
	{
		if (InventoryData[i].ItemAsset == itemData)
		{
			if (InventoryData[i].ItemAmount > ItemAmount)
			{
				int32 NewAmount = InventoryData[i].ItemAmount - ItemAmount;
				SetInventoryItem(i, FInventoryItem(itemData, NewAmount));
				return true;
			}
			else {
				int32 NewAmount = ItemAmount - InventoryData[i].ItemAmount;
				SetInventoryItem(i,FInventoryItem(nullptr,0));
				RemoveItem(itemData, NewAmount);
				return true;
			}
		}
	}
	return false;
}

bool UInventorySystemComponent::UseItem(UItemDataAsset* itemData, int32 ItemAmount)
{
	NotifyItemUsed(itemData, ItemAmount);
	return false;
}

bool UInventorySystemComponent::EquipItemBySlot(FItemSlot ItemSlot, int32 ItemIndex)
{
	UItemDataAsset* NewItemData;
	bool IsEmptyItem;
	int32 NewItemAmount;

	GetItemByIndex(ItemIndex, IsEmptyItem,NewItemData,NewItemAmount);

	if (IsEmptyItem)
	{
		UE_LOG(LogUnWorld,Warning,TEXT("װ���ĵ��߲���Ϊ�գ�"));
		return false;
	}
	// �жϵ�ǰװ�������Ƿ��Ѿ���װ��
	// �еĻ�����λ��
	UItemDataAsset* OldItemData = GetSlottedItem(ItemSlot);
	if (OldItemData)
	{
		 if (SetSlottedItem(ItemSlot, NewItemData))
		 {
			 SetInventoryItem(ItemIndex, FInventoryItem(OldItemData, 1));
			 return true;
		 }
		 return false; 
	}
	// ����ֱ������
	else
	{
		if (SetSlottedItem(ItemSlot, NewItemData))
		{
			// װ���ɹ��ˣ������ߴӱ������Ƴ�
			SetInventoryItem(ItemIndex, FInventoryItem(nullptr, 0));
			return true;
		}
		return false;
	}
	return false;
}

bool UInventorySystemComponent::UnEquipItemBySlot(FItemSlot ItemSlot)
{
	int32 FoundIndex;
	UItemDataAsset* SlotItemData = GetSlottedItem(ItemSlot);
	
	if (!SlotItemData)
	{
		UE_LOG(LogUnWorld,Warning,TEXT("Can not Fount SlotItem!"));
	}
	// Ѱ�ұ����п�λ�ã��Ҳ�������ж��
	if (SearchEmptyInventorySlot(FoundIndex))
	{
		if (SetSlottedItem(ItemSlot, nullptr))
		{
			SetInventoryItem(FoundIndex, FInventoryItem(SlotItemData, 1));
		}
		return true;
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

int32 UInventorySystemComponent::GetInventoryItemCountByIndex(int32 index) const
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

int32 UInventorySystemComponent::GetInventoryItemCount(UItemDataAsset* ItemData) const
{
	int32 FoundAmount = 0;
	for (int32 i=0;i < InventoryData.Num();i++)
	{
		if (InventoryData[i].ItemAsset == ItemData)
		{
			FoundAmount += InventoryData[i].ItemAmount;
		}
	}
	return FoundAmount;
}

bool UInventorySystemComponent::SetInventoryItem(int32 ItemIndex, FInventoryItem Item)
{
	// ��������ɾ�͸����ֲ���
	if (Item.IsValid())
	{
		InventoryData[ItemIndex] = Item;

		if (Item.ItemAsset->bAutoUse)
		{
			// �����Զ�ʹ��
			UseItem(Item.ItemAsset, Item.ItemAmount);
			return true;
		}

		NotifyInventoryItemChanged(false,Item.ItemAsset);
		return true;
	}
	else
	{
		InventoryData[ItemIndex] = FInventoryItem(nullptr,0);
		NotifyInventoryItemChanged(false, nullptr);
		return true;
	}
	return false;
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

void UInventorySystemComponent::NotifyItemUsed(UItemDataAsset* ItemData, int32 ItemAmount)
{
	OnItemUsedNative.Broadcast(ItemData,ItemAmount);
	OnItemUsed.Broadcast(ItemData,ItemAmount);
}
