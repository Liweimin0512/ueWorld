// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryType.h"
#include "InventoryComponent.generated.h"

class UItemDataAsset;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	/** Map of all items owned by this player, from definition to data */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<UItemDataAsset*, FItemData> InventoryData;

	/** Map of slot, from type/num to item, initialized from ItemSlotsPerType on RPGGameInstanceBase */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FItemSlot, UItemDataAsset*> SlottedItems;

	/** Delegate called when an inventory item has been added or removed */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnInventoryItemChanged OnInventoryItemChanged;

	/** Native version above, called before BP delegate */
	FOnInventoryItemChangedNative OnInventoryItemChangedNative;

	//UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
	//void InventoryItemChanged(bool bAdded,UItemDataAsset* item);

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


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//bool FillEmptySlotWithItem(UItemDataAsset* NewItem);

	///** Calls the inventory update callbacks */
	//void NotifyInventoryItemChanged(bool bAdded, UItemDataAsset* Item);


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
