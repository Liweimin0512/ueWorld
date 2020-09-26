// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RPGAbilitySystemComponent.h"
#include "unWorld\Public/unWorldCharacter.h"
#include "AbilitySystemGlobals.h"

URPGAbilitySystemComponent::URPGAbilitySystemComponent() {}

void URPGAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UGameplayAbilityBase*>& ActiveAbilities)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	// Iterate the list of all ability specs
	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		// Iterate all instances on this ability spec
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<UGameplayAbilityBase>(ActiveAbility));
		}
	}
}

int32 URPGAbilitySystemComponent::GetDefaultAbilityLevel() const
{
	AunWorldCharacter* OwningCharacter = Cast<AunWorldCharacter>(OwnerActor);

	if (OwningCharacter)
	{
		return OwningCharacter->GetCharacterLevel();
	}
	return 1;
}

URPGAbilitySystemComponent* URPGAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent)
{
	return Cast<URPGAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}
