// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "unWorld.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "Abilities/RPGAbilityTypes.h"
#include "GameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class UNWORLD_API UGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	// Constructor and overrides
	UGameplayAbilityBase();

	/** Map of gameplay tags to gameplay effect containers */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GameplayEffects)
	TMap<FGameplayTag, FRPGGameplayEffectContainer> EffectContainerMap;

	

};
