// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/CoreAttributeSet.h"
#include "Abilities/RPGAbilitySystemComponent.h"
#include "unWorldCharacter.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

UCoreAttributeSet::UCoreAttributeSet()
	:Health(1.f)
	,MaxHealth(1.f)
	,San(1.f)
	,MaxSan(1.f)
	,AttackPower(1.f)
	,DefensePower(1.0f)
	,Damage(0.0f)
	,CurrentExp(0.0f)
	,MaxExp(0.0f)
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
	DOREPLIFETIME(UCoreAttributeSet, AttackPower);
	DOREPLIFETIME(UCoreAttributeSet, DefensePower);
	DOREPLIFETIME(UCoreAttributeSet, CurrentExp);
	DOREPLIFETIME(UCoreAttributeSet, MaxExp);
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

void UCoreAttributeSet::OnRep_AttackPower()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreAttributeSet, AttackPower);
}

void UCoreAttributeSet::OnRep_DefensePower()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreAttributeSet, DefensePower);
}

void UCoreAttributeSet::OnRep_CurrentExp()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreAttributeSet, CurrentExp);

}

void UCoreAttributeSet::OnRep_MaxExp() {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCoreAttributeSet, MaxExp);

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

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	float DeltaValue = 0;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
	{
		// If this was additive, store the raw delta value to be passed along later
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	AunWorldCharacter* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<AunWorldCharacter>(TargetActor);
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Get the Source actor
		AActor* SourceActor = nullptr;
		AController* SourceController = nullptr;
		AunWorldCharacter* SourceCharacter = nullptr;

		if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
			SourceController = Source->AbilityActorInfo->PlayerController.Get();
			if (SourceController == nullptr && SourceActor != nullptr)
			{
				if (APawn* Pawn = Cast<APawn>(SourceActor))
				{
					SourceController = Pawn->GetController();
				}
			}

			// use the controller to find the source pawn
			if (SourceController)
			{
				SourceCharacter = Cast<AunWorldCharacter>(SourceController->GetPawn());
			}
			else
			{
				SourceCharacter = Cast<AunWorldCharacter>(SourceActor);
			}

			// set the causer actor based on context if it's set
			if (Context.GetEffectCauser())
			{
				SourceActor = Context.GetEffectCauser();
			}
		}

		//Try to extract a hit result
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0)
		{
			// Apply the health change and then clamp it
			const float OldHealth = GetHealth();
			SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth()));

			if (TargetCharacter)
			{
				// This is proper damage
				TargetCharacter->HandleDamage(LocalDamageDone, HitResult, SourceTags, SourceCharacter, SourceActor);

				// Call for all health changes
				TargetCharacter->HandleHealthChanged(-LocalDamageDone, SourceTags);
			}
		}


	}else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(),0.f,GetMaxHealth()));

		if (!TargetCharacter) return;

		//TargetCharacter->HandleHealthChanged(DeltaValue,SourceTags);
		TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
	}else if (Data.EvaluatedData.Attribute == GetCurrentExpAttribute())
	{
		// 当前经验值改变时，判断是否升级
		const float currentExp = GetCurrentExp();
		
		if (!TargetCharacter) return;

		if (GetCurrentExp()> GetMaxExp())
		{
			/* code */
			TargetCharacter->HandleLevelChanged();
			SetCurrentExp(GetCurrentExp() - GetMaxExp());
		}else
		{
			SetCurrentExp(FMath::Clamp(GetCurrentExp(),0.f,GetMaxExp()));		
		}
	}
}