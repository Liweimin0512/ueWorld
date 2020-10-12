// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "unWorld.h"
#include "Components/ActorComponent.h"
#include "ItemDataAsset.h"
#include "InventoryType.h"
#include "InventorySystemComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNWORLD_API UInventorySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventorySystemComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	const TArray<FInventoryItem>& GetInventoryData() const;
	UFUNCTION(BlueprintCallable, Category = Inventory)
	const TMap<FItemSlot, UItemDataAsset*>& GetSlottedItemMap() const;

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

	/** Delegate called when the inventory has been loaded/reloaded */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnInventoryLoaded OnInventoryLoaded;

	/** Native version above, called before BP delegate */
	FOnInventoryLoadedNative OnInventoryLoadedNative;

	/** Called after the inventory was changed and we notified all delegates */
	UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
	void InventoryItemChanged(bool bAdded, UItemDataAsset* Item);

	/** Called after an item was equipped and we notified all delegates */
	UFUNCTION(BlueprintImplementableEvent, Category = Slot)
	void SlottedItemChanged(FItemSlot ItemSlot, UItemDataAsset* Item);


	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool IsInventoryEmpty(int32 SlotIndex) const;
	UFUNCTION(BlueprintCallable, Category = Inventory)
		void GetItemByIndex(int32 index, bool& bEmpty, UItemDataAsset*& item, int32& Amount) const;
	UFUNCTION(BlueprintCallable, Category = Inventory)
		int32 GetInventoryItemCount(int32 index) const;

	/** Manually save the inventory, this is called from add/remove functions automatically */
	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool SaveInventory();

	/** Loads inventory from save game on game instance, this will replace arrays */
	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool LoadInventory();

	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool SearchEmptyInventorySlot(int32& SlotIndex);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool AddItem(FInventoryItem NewItem,int32 ItemAmount,int32& Reset);
	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool RmoveItem(int32 ItemIndex, int32 ItemAmount);

	UFUNCTION(BlueprintCallable,Category = Inventory)
		bool SearchFreeStack(FInventoryItem Item,int32& Index);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		bool AddItemByName(FString ItemName,int32 ItemAmount);

	UFUNCTION(BlueprintCallable, Category = Slot)
		bool EquipItemBySlot(FItemSlot ItemSlot, int32 ItemIndex);

	UFUNCTION(BlueprintCallable, Category = Slot)
		bool UnEquipItemBySlot(FItemSlot ItemSlot);

protected:

	/** Map of all items owned by this player, from definition to data */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
		TArray<FInventoryItem> InventoryData;

	/** Map of slot, from type/num to item, initialized from ItemSlotsPerType on RPGGameInstanceBase */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		TMap<FItemSlot, UItemDataAsset*> SlottedItems;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 AmountOfInventory;

	UFUNCTION(BlueprintCallable, Category = Slot)
		bool SetInventoryItem(int32 ItemIndex, FInventoryItem Item);

	/** Sets slot to item, will remove from other slots if necessary. If passing null this will empty the slot */
	UFUNCTION(BlueprintCallable, Category = Slot)
		bool SetSlottedItem(FItemSlot ItemSlot, UItemDataAsset* Item);

	/** Returns item in slot, or null if empty */
	UFUNCTION(BlueprintPure, Category = Slot)
		UItemDataAsset* GetSlottedItem(FItemSlot ItemSlot) const;

	bool FillEmptySlotWithItem(FInventoryItem NewItem);

	/** Calls the inventory update callbacks */
	void NotifyInventoryItemChanged(bool bAdded, UItemDataAsset* Item);
	void NotifySlottedItemChanged(FItemSlot ItemSlot, UItemDataAsset* Item);
	void NotifyInventoryLoaded();
};