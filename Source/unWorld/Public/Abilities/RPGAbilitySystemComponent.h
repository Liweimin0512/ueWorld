// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilityBase.h"
#include "RPGAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNWORLD_API URPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Constructors and overrides
	URPGAbilitySystemComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		FString AbilityClassPath;

	/** Returns a list of currently active ability instances that match the tags */
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UGameplayAbilityBase*>& ActiveAbilities);

	/** Returns the default level used for ability activations, derived from the character */
	int32 GetDefaultAbilityLevel() const;

	/** Version of function in AbilitySystemGlobals that returns correct type */
	static URPGAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);

	UFUNCTION(BlueprintCallable, Category = Inventory)
		FActiveGameplayEffectHandle ApplyGameplayEffectByEquip(TMap<FGameplayAttribute,float> AttrubuteMap);
};
