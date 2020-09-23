// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "unWorldCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AunWorldCharacter

AunWorldCharacter::AunWorldCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

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
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	//InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	//InventoryComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UCoreAttributeSet>(TEXT("AttributeSet"));
	
	CharacterLevel = 1;
	bAbilitiesInitialized = false;
}

void AunWorldCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

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

//////////////////////////////////////////////////////////////////////////
// Input

void AunWorldCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	//check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//PlayerInputComponent->BindAxis("MoveForward", this, &AunWorldCharacter::MoveForward);
	//PlayerInputComponent->BindAxis("MoveRight", this, &AunWorldCharacter::MoveRight);

	//// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	//// "turn" handles devices that provide an absolute delta, such as a mouse.
	//// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnRate", this, &AunWorldCharacter::TurnAtRate);
	//PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &AunWorldCharacter::LookUpAtRate);

	//// handle touch devices
	//PlayerInputComponent->BindTouch(IE_Pressed, this, &AunWorldCharacter::TouchStarted);
	//PlayerInputComponent->BindTouch(IE_Released, this, &AunWorldCharacter::TouchStopped);

	// VR headset functionality
	//PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AunWorldCharacter::OnResetVR);
}

void AunWorldCharacter::AddStartupGameplayAbilities()
{
	check(AbilitySystemComponent)

	if ( bAbilitiesInitialized ) return;

	// 技能初始化
	for (TSubclassOf<UGameplayAbilityBase>& StartupAbility : GameplayAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility,CharacterLevel,INDEX_NONE,this));
	}

	// 效果初始化
	for (TSubclassOf<UGameplayEffect>& gameplayEffect : PassiveGameplayEffects)
	{
		FGameplayEffectContextHandle effectHandle = AbilitySystemComponent->MakeEffectContext();
		effectHandle.AddSourceObject(this);

		FGameplayEffectSpecHandle newHandle = AbilitySystemComponent->MakeOutgoingSpec(gameplayEffect,CharacterLevel,effectHandle);
		if (newHandle.IsValid())
		{
			FActiveGameplayEffectHandle activeGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*newHandle.Data.Get(),AbilitySystemComponent);	
		}
	}



	bAbilitiesInitialized = true;
}

void AunWorldCharacter::HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
	{
		OnHealthChanged(DeltaValue, EventTags);
	}
}


//void AunWorldCharacter::OnResetVR()
//{
//	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
//}

void AunWorldCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AunWorldCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AunWorldCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AunWorldCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AunWorldCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AunWorldCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

bool AunWorldCharacter::ActivateAbilitiesWithItemSlot(TSubclassOf<UGameplayAbility> ItemAbility,bool bAllowRemoteActivation) {
	
	FGameplayAbilitySpecHandle FoundHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(ItemAbility, CharacterLevel, INDEX_NONE, this));
	
	if ( &FoundHandle && AbilitySystemComponent)
	{
		return AbilitySystemComponent->TryActivateAbility(FoundHandle, bAllowRemoteActivation);
	}

	return false;
}

int32 AunWorldCharacter::GetCharacterLevel()
{
	return 1;
}

void AunWorldCharacter::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UGameplayAbilityBase*>& ActiveAbilities)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);
	}
}

//void AunWorldCharacter::RefreshSlottedGameplayAbilities() {
//	if (bAbilitiesInitialized)
//	{
//		// 刷新任何无效能力并添加新的
//		RemoveSlottedGameplayAbilities(false);
//		AddSlottedGameplayAbilities();
//	}
//}
//
//void AunWorldCharacter::RemoveSlottedGameplayAbilities(bool bRemoveAll)
//{
//	TMap<FRPGItemSlot, FGameplayAbilitySpec> SlottedAbilitySpecs;
//
//	if (!bRemoveAll)
//	{
//		// Fill in map so we can compare
//		FillSlottedAbilitySpecs(SlottedAbilitySpecs);
//	}
//
//	for (TPair<FRPGItemSlot, FGameplayAbilitySpecHandle>& ExistingPair : SlottedAbilities)
//	{
//		FGameplayAbilitySpec* FoundSpec = AbilitySystemComponent->FindAbilitySpecFromHandle(ExistingPair.Value);
//		bool bShouldRemove = bRemoveAll || !FoundSpec;
//
//		if (!bShouldRemove)
//		{
//			// Need to check desired ability specs, if we got here FoundSpec is valid
//			FGameplayAbilitySpec* DesiredSpec = SlottedAbilitySpecs.Find(ExistingPair.Key);
//
//			if (!DesiredSpec || DesiredSpec->Ability != FoundSpec->Ability || DesiredSpec->SourceObject != FoundSpec->SourceObject)
//			{
//				bShouldRemove = true;
//			}
//		}
//
//		if (bShouldRemove)
//		{
//			if (FoundSpec)
//			{
//				// Need to remove registered ability
//				AbilitySystemComponent->ClearAbility(ExistingPair.Value);
//			}
//
//			// Make sure handle is cleared even if ability wasn't found
//			ExistingPair.Value = FGameplayAbilitySpecHandle();
//		}
//	}
//}

//void AunWorldCharacter::AddSlottedGameplayAbilities()
//{
//	TMap<FRPGItemSlot, FGameplayAbilitySpec> SlottedAbilitySpecs;
//	FillSlottedAbilitySpecs(SlottedAbilitySpecs);
//
//	// Now add abilities if needed
//	for (const TPair<FRPGItemSlot, FGameplayAbilitySpec>& SpecPair : SlottedAbilitySpecs)
//	{
//		FGameplayAbilitySpecHandle& SpecHandle = SlottedAbilities.FindOrAdd(SpecPair.Key);
//
//		if (!SpecHandle.IsValid())
//		{
//			SpecHandle = AbilitySystemComponent->GiveAbility(SpecPair.Value);
//		}
//	}
//}

bool AunWorldCharacter::ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation)
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, bAllowRemoteActivation);
	}

	return false;
}
