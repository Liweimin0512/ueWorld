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

void UInventoryComponent::OnItemSlotChanged(FItemSlot ItemSlot, UItemDataAsset* Item)
{
	RefreshSlottedGameplayAbilities();
}

void UInventoryComponent::RefreshSlottedGameplayAbilities()
{
	if (true)
	{
		RemoveSlottedGameplayAbilities(false);
		AddSlottedGameplayAbilities();
	}
}

void UInventoryComponent::RemoveSlottedGameplayAbilities(bool bRemoveAll)
{
	//TMap<FItemSlot, FGameplayAbilitySpec> SlottedAbilitySpecs;

	//if (!bRemoveAll)
	//{
	//	// Fill in map so we can compare
	//	// FillSlottedAbilitySpecs(SlottedAbilitySpecs);
	//}

	//for (TPair<FItemSlot, FGameplayAbilitySpecHandle>& ExistingPair : SlottedAbilities)
	//{
	//	FGameplayAbilitySpec* FoundSpec = AbilitySystemComponent->FindAbilitySpecFromHandle(ExistingPair.Value);
	//	bool bShouldRemove = bRemoveAll || !FoundSpec;

	//	if (!bShouldRemove)
	//	{
	//		// Need to check desired ability specs, if we got here FoundSpec is valid
	//		FGameplayAbilitySpec* DesiredSpec = SlottedAbilitySpecs.Find(ExistingPair.Key);

	//		if (!DesiredSpec || DesiredSpec->Ability != FoundSpec->Ability || DesiredSpec->SourceObject != FoundSpec->SourceObject)
	//		{
	//			bShouldRemove = true;
	//		}
	//	}

	//	if (bShouldRemove)
	//	{
	//		if (FoundSpec)
	//		{
	//			// Need to remove registered ability
	//			AbilitySystemComponent->ClearAbility(ExistingPair.Value);
	//		}

	//		// Make sure handle is cleared even if ability wasn't found
	//		ExistingPair.Value = FGameplayAbilitySpecHandle();
	//	}
	//}
}

void UInventoryComponent::AddSlottedGameplayAbilities()
{
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InventoryData.Reset(AmountofSlots);
	//AbilitySystemComponent = 
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}