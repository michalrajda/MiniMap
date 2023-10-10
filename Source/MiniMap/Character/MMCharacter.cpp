// Fill out your copyright notice in the Description page of Project Settings.


#include "MMCharacter.h"
#include "AbilitySystem/MMAbilitySystemComponent.h"
#include "HealthPawnComponent.h"
#include "MMPlayerState.h"
#include "NativeTags/Tags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MMPlayerState.h"
#include "WeaponManagerComponent.h"
#include "Components/CapsuleComponent.h"
#include "DefaultPawnDataComponent.h"
#include "Weapons/WeaponInstance.h"

AMMCharacter::AMMCharacter()
{
	HealthComponent = CreateDefaultSubobject<UHealthPawnComponent>(TEXT("HealthComponent"));
	WeaponManager = CreateDefaultSubobject<UWeaponManagerComponent>(TEXT("WeaponManager"));
	PawnData = CreateDefaultSubobject<UDefaultPawnDataComponent>(TEXT("PawnData"));
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	UCharacterMovementComponent* CharMovement = GetCharacterMovement();
	if (CharMovement)
	{
		CharMovement->bOrientRotationToMovement = true;
		CharMovement->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
		CharMovement->JumpZVelocity = 500.f;
		CharMovement->AirControl = 0.35f;
		CharMovement->MaxWalkSpeed = 500.f;
		CharMovement->MaxWalkSpeedCrouched = 200.0f;
		CharMovement->MinAnalogWalkSpeed = 20.f;
		CharMovement->BrakingDecelerationWalking = 2000.f;
		CharMovement->bCanWalkOffLedges = true;
		CharMovement->bCanWalkOffLedgesWhenCrouching = true;
		CharMovement->NavAgentProps.bCanCrouch = true;
		CharMovement->NavAgentProps.bCanJump = true;
	}
}

void AMMCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitializeWithAbilitySystem();
}

void AMMCharacter::UnPossessed()
{
	UninitializeFromAbilitySystem();
	Super::UnPossessed();
}

void AMMCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(MMGameplayTags::Status_Crouching, 0);
	}
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void AMMCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(MMGameplayTags::Status_Crouching, 1);
	}
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void AMMCharacter::ToogleCrouch()
{
	if (UCharacterMovementComponent* CharMovementComponent = GetCharacterMovement())
	{
		if (bIsCrouched || CharMovementComponent->bWantsToCrouch)
		{
			CharMovementComponent->bWantsToCrouch = false;
		}
		else if (CharMovementComponent->IsMovingOnGround())
		{
			CharMovementComponent->bWantsToCrouch = true;
		}
	}
}

void AMMCharacter::InitializeWithAbilitySystem()
{
	if (AMMPlayerState* PS = GetPlayerState<AMMPlayerState>())
	{
		UMMAbilitySystemComponent* MMASC = PS->GetMMAbilitySystemComponent();
		if (IsValid(MMASC))
		{
			if (AMMCharacter* OtherAvatar = Cast<AMMCharacter>(MMASC->GetAvatarActor()))
			{
				if (OtherAvatar != this)
				{
					OtherAvatar->UninitializeFromAbilitySystem();
				}
				else
				{
					UninitializeFromAbilitySystem();
				}
			}
			AbilitySystemComponent = MMASC;
			AbilitySystemComponent->InitAbilityActorInfo(PS, this);
			if (IsValid(HealthComponent))
			{
				HealthComponent->InitializeWithAbilitySystemComponent(MMASC);
			}
			if (IsValid(PawnData))
			{
				PawnData->InitializeWithAbilitySystemComponent(MMASC, this);
				if (IsValid(WeaponManager))
				{
					WeaponManager->AddWeapons(PawnData->GetWeaponsToSpawn());
					WeaponManager->ChangeActiveWeapon(0);
					UWeaponInstance* WI = WeaponManager->GetActiveWeaponInstance();
					if (IsValid(WI))
					{
						WI->OnUnequip();
					}
				}
			}
		}
	}
}

void AMMCharacter::UninitializeFromAbilitySystem()
{
	if (IsValid(AbilitySystemComponent))
	{
		if (AbilitySystemComponent->GetAvatarActor() == this)
		{
			FGameplayTagContainer AbilityTypesToIgnore{ MMGameplayTags::GameplayAbility_Behavior_SurvivesDeath };
			AbilitySystemComponent->CancelAbilities(nullptr, &AbilityTypesToIgnore);
			AbilitySystemComponent->ClearAbilityInput();
			AbilitySystemComponent->RemoveAllGameplayCues();
			if (IsValid(HealthComponent))
			{
				HealthComponent->UninitializeFromAbilitySystem(AbilitySystemComponent);
			}
			if (IsValid(PawnData))
			{
				PawnData->UninitializeFromAbilitySystem(AbilitySystemComponent);
			}
			if (IsValid(WeaponManager))
			{
				WeaponManager->UninitializeAndRemoveAllWeapons();
				WeaponManager->UnbindDelegates();
			}
			if (IsValid(AbilitySystemComponent->GetOwnerActor()))
			{
				AbilitySystemComponent->SetAvatarActor(nullptr);
			}
			else
			{
				AbilitySystemComponent->ClearActorInfo();
			}
		}
	}
	AbilitySystemComponent = nullptr;
}


UAbilitySystemComponent* AMMCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}