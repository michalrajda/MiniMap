// Fill out your copyright notice in the Description page of Project Settings.


#include "MMGameplayAbility_Jump.h"
#include "Character/MMCharacter.h"
#include "AbilitySystem/Tasks/MMAbilityTask_WaitInput.h"
#include "AbilitySystem/Tasks/MMAbilityTask_StartAbilityState.h"

bool UMMGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const AMMCharacter* Pawn = ActorInfo ? Cast<AMMCharacter>(ActorInfo->AvatarActor.Get()) : nullptr;
	if (Pawn)
	{
		if (Pawn->CanJump())
		{
			return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
		}
	}
	return false;
}

void UMMGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	InputReleasedTask = UMMAbilityTask_WaitInputReleased::CreateWaitInputReleasedTask(this, true);
	StartAbilityStateTask = UMMAbilityTask_StartAbilityState::CreateStartAbilityStateTask(this, TEXT("Jumping"), true);
	if (IsValid(InputReleasedTask) && IsValid(StartAbilityStateTask))
	{
		InputReleasedTask->OnInputReleased.BindUObject(this, &UMMGameplayAbility_Jump::OnInputReleased);
		StartAbilityStateTask->OnStateEnded.BindUObject(this, &UMMGameplayAbility_Jump::CharacterJumpStop);
		StartAbilityStateTask->OnStateInterrupted.BindUObject(this, &UMMGameplayAbility_Jump::CharacterJumpStop);
		CharacterJumpStart();
		StartAbilityStateTask->ReadyForActivation();
		InputReleasedTask->ReadyForActivation();
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
	else
	{
		bool bReplicateEndAbility = true;
		bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
}

void UMMGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    CharacterJumpStop();
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMMGameplayAbility_Jump::CharacterJumpStart()
{
	if (AMMCharacter* Pawn = GetMMCharacterFromActorInfo())
	{
		if (Pawn->IsLocallyControlled() && !Pawn->bPressedJump)
		{
			Pawn->Jump();
		}
	}
}

void UMMGameplayAbility_Jump::CharacterJumpStop()
{
	if (AMMCharacter* Pawn = GetMMCharacterFromActorInfo())
	{
		if (Pawn->IsLocallyControlled() && Pawn->bPressedJump)
		{
			Pawn->StopJumping();
		}
	}
}

void UMMGameplayAbility_Jump::OnInputReleased()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
