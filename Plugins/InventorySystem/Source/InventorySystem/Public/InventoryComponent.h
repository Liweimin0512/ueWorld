// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryType.h"
//#include "AbilitySystemComponent.h"
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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//UPROPERTY()
	//UAbilitySystemComponent* AbilitySystemComponent;

	//bool FillEmptySlotWithItem(UItemDataAsset* NewItem);

	/** Calls the inventory update callbacks */
	void NotifyInventoryItemChanged(bool bAdded, UItemDataAsset* Item);
	void NotifySlottedItemChanged(FItemSlot ItemSlot,UItemDataAsset* item);

	/** Map of slot to ability granted by that slot. I may refactor this later */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	//TMap<FItemSlot, FGameplayAbilitySpecHandle> SlottedAbilities;

	/** Called when slotted items change, bound to delegate on interface */
	void OnItemSlotChanged(FItemSlot ItemSlot, UItemDataAsset* Item);
	void RefreshSlottedGameplayAbilities();
	/** Remove slotted gameplay abilities, if force is false it only removes invalid ones */
	void RemoveSlottedGameplayAbilities(bool bRemoveAll);
	/** Adds slotted item abilities if needed */
	void AddSlottedGameplayAbilities();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};