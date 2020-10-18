// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "unWorldCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Inventory/ItemDataAsset.h"
#include "Inventory/InventorySystemComponent.h"

//////////////////////////////////////////////////////////////////////////
// AunWorldCharacter

AunWorldCharacter::AunWorldCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	//BaseTurnRate = 45.f;
	//BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	//CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	//// Create a follow camera
	//FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);


	InventoryComponent = CreateDefaultSubobject<UInventorySystemComponent>(TEXT("InventoryComponent"));
	InventoryComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UCoreAttributeSet>(TEXT("AttributeSet"));
	
	CharacterLevel = 1;
	bAbilitiesInitialized = false;
}

void AunWorldCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (InventoryComponent)
	{
		InventoryUpdateHandle = InventoryComponent->OnSlottedItemChangedNative.AddUObject(this, &AunWorldCharacter::OnItemSlotChanged);
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		// 启动技能初始化
		AddStartupGameplayAbilities();
	}
}

void AunWorldCharacter::UnPossessed()
{
	Super::UnPossessed();

}

void AunWorldCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AunWorldCharacter,CharacterLevel);
}

UAbilitySystemComponent* AunWorldCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float AunWorldCharacter::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float AunWorldCharacter::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float AunWorldCharacter::GetSan() const
{
	return AttributeSet->GetSan();
}

float AunWorldCharacter::GetMaxSan() const
{
	return AttributeSet->GetMaxSan();
}

float AunWorldCharacter::GetAttackPower() const
{
	return AttributeSet->GetAttackPower();
}

void AunWorldCharacter::AddStartupGameplayAbilities()
{
	check(AbilitySystemComponent)

	if (Role == ROLE_Authority && !bAbilitiesInitialized)
	{
		// 技能初始化
		for (TSubclassOf<UGameplayAbilityBase>& StartupAbility : GameplayAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, CharacterLevel, INDEX_NONE, this));
		}

		// 效果初始化
		for (TSubclassOf<UGameplayEffect>& gameplayEffect : PassiveGameplayEffects)
		{
			FGameplayEffectContextHandle effectHandle = AbilitySystemComponent->MakeEffectContext();
			effectHandle.AddSourceObject(this);

			FGameplayEffectSpecHandle newHandle = AbilitySystemComponent->MakeOutgoingSpec(gameplayEffect, CharacterLevel, effectHandle);
			if (newHandle.IsValid())
			{
				FActiveGameplayEffectHandle activeGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*newHandle.Data.Get(), AbilitySystemComponent);
			}
		}

		AddSlottedGameplayAbilities();
		bAbilitiesInitialized = true;
		UE_LOG(LogUnWorld, Log, TEXT("AbilitiesInitialized = true"));
	}
	else
	{
		UE_LOG(LogUnWorld, Warning, TEXT("AddStartupGameplayAbilities: bAbilitiesInitialized || Role != ROLE_Authority"));
	}
}

void AunWorldCharacter::HandleDamage(float DamageAmount, const FHitResult& HitInfo, const FGameplayTagContainer& DamageTags, 
	AunWorldCharacter* InstigatorCharacter, AActor* DamageCauser)
{
	OnDamaged(DamageAmount,HitInfo,DamageTags,InstigatorCharacter,DamageCauser);
}

void AunWorldCharacter::HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
	{
		OnHealthChanged(DeltaValue, EventTags);
	}
}

void AunWorldCharacter::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UGameplayAbilityBase*>& ActiveAbilities)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);
	}
}

bool AunWorldCharacter::ActivateAbilitiesWithItemSlot(FItemSlot ItemSlot, bool bAllowRemoteActivation)
{
	FGameplayAbilitySpecHandle* FoundHandle = SlottedAbilities.Find(ItemSlot);

	if (FoundHandle && AbilitySystemComponent)
	{
		//UE_LOG(LogUnWorld, Log, TEXT("Can Found Ability: %s !"), *FoundHandle->ToString());
		return AbilitySystemComponent->TryActivateAbility(*FoundHandle, bAllowRemoteActivation);
	}
	UE_LOG(LogUnWorld,Warning,TEXT("Can not Found Ability!"));
	return false;
}

void AunWorldCharacter::RemoveSlottedGameplayAbilities(bool bRemoveAll)
{
	TMap<FItemSlot, FGameplayAbilitySpec> SlottedAbilitySpecs;

	if (!bRemoveAll)
	{
		// Fill in map so we can compare
		FillSlottedAbilitySpecs(SlottedAbilitySpecs);
	}

	for (TPair<FItemSlot, FGameplayAbilitySpecHandle>& ExistingPair : SlottedAbilities)
	{
		FGameplayAbilitySpec* FoundSpec = AbilitySystemComponent->FindAbilitySpecFromHandle(ExistingPair.Value);
		bool bShouldRemove = bRemoveAll || !FoundSpec;

		if (!bShouldRemove)
		{
			// Need to check desired ability specs, if we got here FoundSpec is valid
			FGameplayAbilitySpec* DesiredSpec = SlottedAbilitySpecs.Find(ExistingPair.Key);

			if (!DesiredSpec || DesiredSpec->Ability != FoundSpec->Ability || DesiredSpec->SourceObject != FoundSpec->SourceObject)
			{
				bShouldRemove = true;
			}
		}

		if (bShouldRemove)
		{
			if (FoundSpec)
			{
				// Need to remove registered ability
				AbilitySystemComponent->ClearAbility(ExistingPair.Value);
			}

			// Make sure handle is cleared even if ability wasn't found
			ExistingPair.Value = FGameplayAbilitySpecHandle();
		}
	}
}

bool AunWorldCharacter::ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation)
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, bAllowRemoteActivation);
	}

	return false;
}

void AunWorldCharacter::AddSlottedGameplayAbilities()
{
	TMap<FItemSlot, FGameplayAbilitySpec> SlottedAbilitySpecs;
	FillSlottedAbilitySpecs(SlottedAbilitySpecs);

	// Now add abilities if needed
	for (const TPair<FItemSlot, FGameplayAbilitySpec>& SpecPair : SlottedAbilitySpecs)
	{
		FGameplayAbilitySpecHandle& SpecHandle = SlottedAbilities.FindOrAdd(SpecPair.Key);

		if (!SpecHandle.IsValid())
		{
			SpecHandle = AbilitySystemComponent->GiveAbility(SpecPair.Value);
		}
	}
}

void AunWorldCharacter::FillSlottedAbilitySpecs(TMap<FItemSlot, FGameplayAbilitySpec>& SlottedAbilitySpecs)
{
	// First add default ones
	for (const TPair<FItemSlot, TSubclassOf<UGameplayAbilityBase>>& DefaultPair : DefaultSlottedAbilities)
	{
		if (DefaultPair.Value.Get())
		{
			SlottedAbilitySpecs.Add(DefaultPair.Key, FGameplayAbilitySpec(DefaultPair.Value, GetCharacterLevel(), INDEX_NONE, this));
		}
	}

	// Now potentially override with inventory
	if (InventoryComponent)
	{
		const TMap<FItemSlot, UItemDataAsset*>& SlottedItemMap = InventoryComponent->GetSlottedItemMap();

		for (const TPair<FItemSlot, UItemDataAsset*>& ItemPair : SlottedItemMap)
		{
			UItemDataAsset* SlottedItem = ItemPair.Value;

			// Use the character level as default
			int32 AbilityLevel = GetCharacterLevel();

			if (SlottedItem && SlottedItem->ItemType.GetName() == FName(TEXT("Weapon")))
			{
				// Override the ability level to use the data from the slotted item
				AbilityLevel = SlottedItem->AbilityLevel;
			}

			if (SlottedItem && !SlottedItem->AbilityName.IsEmpty())
			{
				// This will override anything from default
				FString AbilityAssetPath = FString::Printf(TEXT("Blueprint'/Game/unWorld/Abilities/%s.%s_C'")
					, *SlottedItem->AbilityName, *SlottedItem->AbilityName);
				TSubclassOf<UGameplayAbilityBase> GrantedAbility = LoadClass<UGameplayAbilityBase>(NULL, *AbilityAssetPath);
				if (!GrantedAbility)
				{
					UE_LOG(LogUnWorld, Warning, TEXT("Can not Found Ability On Path: %s"), *GrantedAbility->GetName());
					return;
				}
				//UE_LOG(LogUnWorld, Warning, TEXT("Get Ability By Name: %s"), *GrantedAbility->GetFName().ToString());
				SlottedAbilitySpecs.Add(ItemPair.Key, FGameplayAbilitySpec(GrantedAbility, AbilityLevel, INDEX_NONE, SlottedItem));
			}
		}
	}
}

int32 AunWorldCharacter::GetCharacterLevel() const
{
	return CharacterLevel;
}

bool AunWorldCharacter::SetCharacterLevel(int32 NewLevel)
{
	if (CharacterLevel != NewLevel && NewLevel > 0)
	{
		// Our level changed so we need to refresh abilities
		// TODO RemoveStartupGameplayAbilities();
		CharacterLevel = NewLevel;
		AddStartupGameplayAbilities();

		return true;
	}
	return false;
}

int32 AunWorldCharacter::GetCurrentExp() const
{
	return 1;
}

int32 AunWorldCharacter::GetMaxExp() const
{
	return 1;
}

bool AunWorldCharacter::UpgradeLevel()
{
	return false;
}

void AunWorldCharacter::OnItemSlotChanged(FItemSlot ItemSlot, UItemDataAsset* Item)
{
	UE_LOG(LogUnWorld, Log, TEXT("OnItemSlotChanged!"));
	RefreshSlottedGameplayAbilities();
}

void AunWorldCharacter::RefreshSlottedGameplayAbilities()
{
	if (bAbilitiesInitialized)
	{
		// Refresh any invalid abilities and adds new ones
		RemoveSlottedGameplayAbilities(false);
		AddSlottedGameplayAbilities();
	}
	else {
		UE_LOG(LogUnWorld, Warning, TEXT("bAbilitiesInitialized is False!"));
	}
}
