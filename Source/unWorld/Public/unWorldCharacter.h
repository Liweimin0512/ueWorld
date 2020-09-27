// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/RPGAbilitySystemComponent.h"
#include "Abilities/GameplayAbilityBase.h"
#include "Abilities/CoreAttributeSet.h"
#include "Inventory/InventorySystemComponent.h"
#include "Inventory/InventoryType.h"
#include "unWorldCharacter.generated.h"

UCLASS(config = Game)
class AunWorldCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	// Constructor and overrides
	AunWorldCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Implement IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

#pragma region Attribut

	UFUNCTION(BlueprintCallable)
		virtual float GetHealth() const;
	UFUNCTION(BlueprintCallable)
		virtual float GetMaxHealth() const;

#pragma endregion Attribut

#pragma region Abilities

	//UFUNCTION(BlueprintImplementableEvent)
	//void RefreshSlottedGameplayAbilities();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		int32 GetCharacterLevel();

	/** Returns a list of active abilities matching the specified tags. This only returns if the ability is currently running */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
		void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UGameplayAbilityBase*>& ActiveAbilities);

	/**
	 * Attempts to activate any ability in the specified item slot. Will return false if no activatable ability found or activation fails
	 * Returns true if it thinks it activated, but it may return false positives due to failure later in activation.
	 * If bAllowRemoteActivation is true, it will remotely activate local/server abilities, if false it will only try to locally activate the ability
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool ActivateAbilitiesWithItemSlot(FItemSlot ItemSlot, bool bAllowRemoteActivation = true);

#pragma endregion Abilities



	///** Camera boom positioning the camera behind the character */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//	class USpringArmComponent* CameraBoom;

	///** Follow camera */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//	class UCameraComponent* FollowCamera;


public:


	///** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	//	float BaseTurnRate;

	///** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	//	float BaseLookUpRate;



protected:

	UPROPERTY(EditAnywhere, Replicated, Category = Abilities)
		int32 CharacterLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
		TArray<TSubclassOf<UGameplayAbilityBase>> GameplayAbilities;

	/** Map of item slot to gameplay ability class, these are bound before any abilities added by the inventory */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
		TMap<FItemSlot, TSubclassOf<UGameplayAbilityBase>> DefaultSlottedAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
		TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	/** The component used to handle ability system interactions */
	UPROPERTY(BlueprintReadOnly)
		URPGAbilitySystemComponent* AbilitySystemComponent;

	/** List of attributes modified by the ability system */
	UPROPERTY()
		UCoreAttributeSet* AttributeSet;

	/** Cached pointer to the inventory source for this character, can be null */
	UPROPERTY(BlueprintReadOnly)
		UInventorySystemComponent* InventoryComponent;

	/** If true we have initialized our abilities */
	UPROPERTY()
		int32 bAbilitiesInitialized;

	/** Map of slot to ability granted by that slot. I may refactor this later */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
		TMap<FItemSlot, FGameplayAbilitySpecHandle> SlottedAbilities;


#pragma region AttributeChange

	/**
	 * Called when health is changed, either from healing or from being damaged
	 * For damage this is called in addition to OnDamaged/OnKilled
	 *
	 * @param DeltaValue Change in health value, positive for heal, negative for cost. If 0 the delta is unknown
	 * @param EventTags The gameplay tags of the event that changed mana
	 */
	UFUNCTION(BlueprintImplementableEvent)
		void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

#pragma endregion AttributeChange
	
	/** Apply the startup gameplay abilities and effects */
	void AddStartupGameplayAbilities();

	///** Returns CameraBoom subobject **/
	//FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	///** Returns FollowCamera subobject **/
	//FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/**
	 * Attempts to activate all abilities that match the specified tags
	 * Returns true if it thinks it activated, but it may return false positives due to failure later in activation.
	 * If bAllowRemoteActivation is true, it will remotely activate local/server abilities, if false it will only try to locally activate the ability
	 */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
		bool ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation = true);

	/** Adds slotted item abilities if needed */
	void AddSlottedGameplayAbilities();

	/** Fills in with ability specs, based on defaults and inventory */
	void FillSlottedAbilitySpecs(TMap<FItemSlot, FGameplayAbilitySpec>& SlottedAbilitySpecs);

	// Called from CoreAttributeSet, these call BP events above
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	// Friended to allow access to handle functions above
	friend UCoreAttributeSet;
};

