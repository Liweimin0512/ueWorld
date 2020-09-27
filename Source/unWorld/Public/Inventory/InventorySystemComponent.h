// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemDataAsset.h"
#include "InventoryType.h"
#include "InventorySystemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
		TMap<UItemDataAsset*, FItemData> InventoryData;

	/** Map of slot, from type/num to item, initialized from ItemSlotsPerType on RPGGameInstanceBase */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
		TMap<FItemSlot, UItemDataAsset*> SlottedItems;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Implement IRPGInventoryInterface
	virtual const TMap<UItemDataAsset*, FItemData>& GetInventoryDataMap() const
	{
		return InventoryData;
	}
	virtual const TMap<FItemSlot, UItemDataAsset*>& GetSlottedItemMap() const
	{
		return SlottedItems;
	}

};
