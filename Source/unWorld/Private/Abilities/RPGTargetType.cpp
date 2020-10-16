// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RPGTargetType.h"
#include "Abilities/GameplayAbilityBase.h"
#include "unWorldCharacter.h"

void URPGTargetType::GetTargets_Implementation(AunWorldCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData,
	TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	return;
}


void URPGTargetType_UseOwner::GetTargets_Implementation(AunWorldCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, 
	TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	OutActors.Add(TargetingCharacter);
}

void URPGTargetType_UseEventData::GetTargets_Implementation(AunWorldCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, 
	TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
	if (FoundHitResult)
	{
		OutHitResults.Add(*FoundHitResult);
	}
	else if (EventData.Target)
	{
		OutActors.Add(const_cast<AActor*>(EventData.Target));
	}
}