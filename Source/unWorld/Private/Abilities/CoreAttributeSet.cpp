// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/CoreAttributeSet.h"
#include "Abilities/RPGAbilitySystemComponent.h"
#include "unWorldCharacter.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

UCoreAttributeSet::UCoreAttributeSet():
	Health(1.f),
	MaxHealth(1.f),
	San(1.f),
	MaxSan(1.f)
{
}

// 属性同步
void UCoreAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCoreAttributeSet, Health);
	DOREPLIFETIME(UCoreAttributeSet, MaxHealth);
	DOREPLIFETIME(UCoreAttributeSet, San);
	DOREPLIFETIME(UCoreAttributeSet, MaxSan);

}

void UCoreAttributeSet::OnRep_Health() {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreAttributeSet,Health);
}

void UCoreAttributeSet::OnRep_MaxHealth() {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreAttributeSet, MaxHealth);
}

void UCoreAttributeSet::OnRep_San() {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreAttributeSet, San);
}

void UCoreAttributeSet::OnRep_MaxSan() {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreAttributeSet, MaxSan);
}

void UCoreAttributeSet::AdjustAttributeFormMaxChange(FGameplayAttributeData& AffectedAttribute, \
	const FGameplayAttributeData& MaxAttribute, float NewMaxValue, 
	const FGameplayAttribute& AffectedAttributeProperty) 
{
	UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();

	if (FMath::IsNearlyEqual(CurrentMaxValue,NewMaxValue) && AbilityComponent ) return;

	const float CurrentValue = AffectedAttribute.GetCurrentValue();
	//float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) : NewMaxValue;
	float NewDelta = NewMaxValue;

	AbilityComponent->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
}

void UCoreAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) 
{
	Super::PreAttributeChange(Attribute,NewValue);

	// 在生命值改变前，判断是否在合理范围内
	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeFormMaxChange(Health,MaxHealth,NewValue,GetHealthAttribute());
	}
}

void UCoreAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) 
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	float DeltaValue = 0;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		// If this was additive, store the raw delta value to be passed along later
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	AunWorldCharacter* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetCharacter = Cast<AunWorldCharacter>(Data.Target.AbilityActorInfo->AvatarActor.Get());
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(),0.f,GetMaxHealth()));

		if (!TargetCharacter) return;

		//TargetCharacter->HandleHealthChanged(DeltaValue,SourceTags);
		TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
	}
}