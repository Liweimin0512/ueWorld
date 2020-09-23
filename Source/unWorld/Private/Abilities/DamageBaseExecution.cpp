// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/DamageBaseExecution.h"
#include "Abilities/CoreAttributeSet.h"
#include "AbilitySystemComponent.h"

struct RPGDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	RPGDamageStatics()
	{
		// Capture the Target's DefensePower attribute. Do not snapshot it, because we want to use the health value at the moment we apply the execution.
		//DEFINE_ATTRIBUTE_CAPTUREDEF(UCoreAttributeSet, DefensePower, Target, false);

		//// Capture the Source's AttackPower. We do want to snapshot this at the moment we create the GameplayEffectSpec that will execute the damage.
		//// (imagine we fire a projectile: we create the GE Spec when the projectile is fired. When it hits the target, we want to use the AttackPower at the moment
		//// the projectile was launched, not when it hits).
		//DEFINE_ATTRIBUTE_CAPTUREDEF(UCoreAttributeSet, AttackPower, Source, true);

		//// Also capture the source's raw Damage, which is normally passed in directly via the execution
		//DEFINE_ATTRIBUTE_CAPTUREDEF(UCoreAttributeSet, Damage, Source, true);
	}
};

static const RPGDamageStatics& DamageStatics()
{
	static RPGDamageStatics DmgStatics;
	return DmgStatics;
}

UDamageBaseExecution::UDamageBaseExecution()
{
	//RelevantAttributesToCapture.Add(DamageStatics().DefensePowerDef);
	//RelevantAttributesToCapture.Add(DamageStatics().AttackPowerDef);
	//RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
}

void UDamageBaseExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	//UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	//AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->AvatarActor : nullptr;
	//AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->AvatarActor : nullptr;

	//const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	//// Gather the tags from the source and target as that can affect which buffs should be used
	//const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	//const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//FAggregatorEvaluateParameters EvaluationParameters;
	//EvaluationParameters.SourceTags = SourceTags;
	//EvaluationParameters.TargetTags = TargetTags;

	//// --------------------------------------
	////	Damage Done = Damage * AttackPower / DefensePower
	////	If DefensePower is 0, it is treated as 1.0
	//// --------------------------------------

	//float DefensePower = 0.f;
	//ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefensePowerDef, EvaluationParameters, DefensePower);
	//if (DefensePower == 0.0f)
	//{
	//	DefensePower = 1.0f;
	//}

	//float AttackPower = 0.f;
	//ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPowerDef, EvaluationParameters, AttackPower);

	//float Damage = 0.f;
	//ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);

	//float DamageDone = Damage * AttackPower / DefensePower;
	//if (DamageDone > 0.f)
	//{
	//	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, DamageDone));
	//}
}