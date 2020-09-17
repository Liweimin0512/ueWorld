// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryType.h"
#include "InventoryComponent.generated.h"

class UItemDataAsset;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	int32 AmountofSlots;

	/** Map of all items owned by this player, from definition to data */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TArray<FInventoryItem> InventoryData;
	//TMap<int32, FInventoryItem> InventoryMapData;

	/** Map of slot, from type/num to item, initialized from ItemSlotsPerType on RPGGameInstanceBase */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FItemSlot, UItemDataAsset*> SlottedItems;

public:
	
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool IsSlotEmpty(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void GetItemByIndex(bool& bEmpty, UItemDataAsset*& ItemData, int32& Amount, int32 index);

	//UFUNCTION(BlueprintCallable, Category = Inventory)
	//void GetInventoryItems(TArray<FInventoryItem>& items);

	/** Delegate called when an inventory item has been added or removed */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnInventoryItemChanged OnInventoryItemChanged;

	/** Native version above, called before BP delegate */
	FOnInventoryItemChangedNative OnInventoryItemChangedNative;

	/** Delegate called when an inventory slot has changed */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnSlottedItemChanged OnSlottedItemChanged;

	/** Native version above, called before BP delegate */
	FOnSlottedItemChangedNative OnSlottedItemChangedNative;

	//UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
	//void InventoryItemChanged(bool bAdded,UItemDataAsset* item);

	///** Called after an item was equipped and we notified all delegates */
	//UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
	//void SlottedItemChanged(FItemSlot ItemSlot, UItemDataAsset* Item);

	///** Adds a new inventory item, will add it to an empty slot if possible. If the item supports count you can add more than one count. It will also update the level when adding if required */
	//UFUNCTION(BlueprintCallable, Category = Inventory)
	//bool AddInventoryItem(UItemDataAsset* NewItem, int32 ItemCount = 1, int32 ItemLevel = 1, bool bAutoSlot = true);

	///** Remove an inventory item, will also remove from slots. A remove count of <= 0 means to remove all copies */
	//UFUNCTION(BlueprintCallable, Category = Inventory)
	//bool RemoveInventoryItem(UItemDataAsset* RemovedItem, int32 RemoveCount = 1);

	///** Returns all inventory items of a given type. If none is passed as type it will return all */
	//UFUNCTION(BlueprintCallable, Category = Inventory)
	//void GetInventoryItems(TArray<UItemDataAsset*>& Items, FPrimaryAssetType ItemType);

	///** Returns number of instances of this item found in the inventory. This uses count from GetItemData */
	//UFUNCTION(BlueprintPure, Category = Inventory)
	//int32 GetInventoryItemCount(UItemDataAsset* Item) const;

	///** Returns the item data associated with an item. Returns false if none found */
	//UFUNCTION(BlueprintPure, Category = Inventory)
	//bool GetInventoryItemData(UItemDataAsset* Item, FItemData& ItemData) const;

	///** Sets slot to item, will remove from other slots if necessary. If passing null this will empty the slot */
	//UFUNCTION(BlueprintCallable, Category = Inventory)
	//bool SetSlottedItem(FItemSlot ItemSlot , UItemDataAsset* Item);

	///** Returns item in slot, or null if empty */
	//UFUNCTION(BlueprintPure, Category = Inventory)
	//UItemDataAsset* GetSlottedItem(FItemSlot ItemSlot) const;

	///** Returns all slotted items of a given type. If none is passed as type it will return all */
	//UFUNCTION(BlueprintCallable, Category = Inventory)
	//void GetSlottedItems(TArray<UItemDataAsset*>& Items, FPrimaryAssetType ItemType, bool bOutputEmptyIndexes);

	///** Fills in any empty slots with items in inventory */
	//UFUNCTION(BlueprintCallable, Category = Inventory)
	//void FillEmptySlots();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//bool FillEmptySlotWithItem(UItemDataAsset* NewItem);

	/** Calls the inventory update callbacks */
	void NotifyInventoryItemChanged(bool bAdded, UItemDataAsset* Item);
	void NotifySlottedItemChanged(FItemSlot ItemSlot,UItemDataAsset* item);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};