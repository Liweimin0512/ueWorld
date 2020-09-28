// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemDataAsset.h"
#include "InventoryType.h"
#include "InventorySystemComponent.generated.h"


UCLASS(Blueprintable ,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNWORLD_API UInventorySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventorySystemComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	/** Map of all items owned by this player, from definition to data */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
		TArray<FInventoryItem> InventoryData;

	/** Map of slot, from type/num to item, initialized from ItemSlotsPerType on RPGGameInstanceBase */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
		TMap<FItemSlot, UItemDataAsset*> SlottedItems;

	const TArray<FInventoryItem>& GetInventoryDataMap() const;
	const TMap<FItemSlot, UItemDataAsset*>& GetSlottedItemMap() const;

protected:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Inventory)
	int32 AmountOfInventory;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool IsInventoryEmpty(int32 SlotIndex) const;
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void GetItemByIndex(int32 index, bool& bEmpty, UItemDataAsset*& item,int32& Amount) const;
};
