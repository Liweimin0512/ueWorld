// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "CoreAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * �������Լ�
 */
UCLASS()
class UNWORLD_API UCoreAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCoreAttributeSet();
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps)const override;

	UPROPERTY(BlueprintReadOnly,Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSet,Health)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "San", ReplicatedUsing = OnRep_San)
	FGameplayAttributeData San;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSet, San)

	UPROPERTY(BlueprintReadOnly, Category = "San", ReplicatedUsing = OnRep_MaxSan)
	FGameplayAttributeData MaxSan;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSet, MaxSan)

	/** AttackPower of the attacker is multiplied by the base Damage to reduce health, so 1.0 means no bonus */
	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_AttackPower)
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSet, AttackPower)

	/** Base Damage is divided by DefensePower to get actual damage done, so 1.0 means no bonus */
	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_DefensePower)
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSet, DefensePower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_StdDefensePower)
	FGameplayAttributeData StdDefensePower;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSet, StdDefensePower)


	/** Damage is a 'temporary' attribute used by the DamageExecution to calculate final damage, which then turns into -Health */
	UPROPERTY(BlueprintReadOnly, Category = "Mana", meta = (HideFromLevelInfos))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSet, Damage)

		UPROPERTY(BlueprintReadOnly, Category = "Exp", ReplicatedUsing = OnRep_CurrentExp)
		FGameplayAttributeData CurrentExp;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSet, CurrentExp)

		UPROPERTY(BlueprintReadOnly, Category = "Exp", ReplicatedUsing = OnRep_MaxExp)
		FGameplayAttributeData MaxExp;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSet, MaxExp)

protected:
	void AdjustAttributeFormMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute,
		float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

	UFUNCTION()
	virtual void OnRep_Health();
	
	UFUNCTION()
	virtual void OnRep_MaxHealth();

	UFUNCTION()
	virtual void OnRep_San();

	UFUNCTION()
	virtual void OnRep_MaxSan();

	UFUNCTION()
		virtual void OnRep_AttackPower();

	UFUNCTION()
		virtual void OnRep_DefensePower();

	UFUNCTION()
		virtual void OnRep_CurrentExp();

	UFUNCTION()
		virtual void OnRep_MaxExp();

	UFUNCTION()
		virtual void OnRep_StdDefensePower();

};