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
 * ∫À–ƒ Ù–‘ºØ
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

	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_Attack)
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UCoreAttributeSet, Attack)


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
	virtual void OnRep_Attack();

};