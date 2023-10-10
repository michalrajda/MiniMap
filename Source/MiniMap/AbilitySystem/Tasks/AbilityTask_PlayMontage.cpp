// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask_PlayMontage.h"
#include "AbilitySystemGlobals.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

void UAbilityTask_PlayMontage::ExternalCancel()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnCancelled.ExecuteIfBound();
	}
	Super::ExternalCancel();
}

UAbilityTask_PlayMontage* UAbilityTask_PlayMontage::CreatePlayMontage(UGameplayAbility* OwningAbility, UAnimMontage* MontageToPlay, FName TaskInstanceName, float Rate, FName StartSection, bool bStopWhenAbilityEnds, float AnimRootMotionTranslationScale, float StartTimeSeconds)
{
	if (IsValid(MontageToPlay))
	{
		UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);
		UAbilityTask_PlayMontage* MyObj = NewAbilityTask<UAbilityTask_PlayMontage>(OwningAbility, TaskInstanceName);
		if (MyObj)
		{
			MyObj->MontageToPlay = MontageToPlay;
			MyObj->Rate = Rate;
			MyObj->StartSection = StartSection;
			MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
			MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
			MyObj->StartTimeSeconds = StartTimeSeconds;
			return MyObj;
		}
	}
	return nullptr;
}

void UAbilityTask_PlayMontage::Activate()
{
	if (IsValid(Ability))
	{
		bool bPlayedMontage = false;
		if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
		{
			if (const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo())
			{
				if (UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance())
				{
					if (ASC->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection, StartTimeSeconds) > 0.f)
					{
						if (ShouldBroadcastAbilityTaskDelegates())
						{
							InterruptedHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UAbilityTask_PlayMontage::OnMontageInterrupted);
							BlendingOutDelegate.BindUObject(this, &UAbilityTask_PlayMontage::OnMontageBlendingOut);
							AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);
							MontageEndedDelegate.BindUObject(this, &UAbilityTask_PlayMontage::OnMontageEnded);
							AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);
							if (ACharacter* Character = Cast<ACharacter>(GetAvatarActor()))
							{
								if (Character->GetLocalRole() == ROLE_Authority || (Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted))
								{
									Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
								}
							}
							bPlayedMontage = true;
						}
						else
						{
							return;
						}
					}
				}
			}
		}
		if (!bPlayedMontage)
		{
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				OnCancelled.ExecuteIfBound();
			}
		}
		SetWaitingOnAvatar();
	}
}

void UAbilityTask_PlayMontage::OnDestroy(bool bInOwnerFinished)
{
	if (IsValid(Ability))
	{
		Ability->OnGameplayAbilityCancelled.Remove(InterruptedHandle);
		if (bInOwnerFinished && bStopWhenAbilityEnds)
		{
			StopPlayingMontage();
		}
	}
	Super::OnDestroy(bInOwnerFinished);
}

bool UAbilityTask_PlayMontage::StopPlayingMontage()
{
	if (IsValid(Ability))
	{
		if (const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo())
		{
			if (UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance())
			{
				if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
				{
					if (ASC->GetAnimatingAbility() == Ability && ASC->GetCurrentMontage() == MontageToPlay)
					{
						FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
						if (MontageInstance)
						{
							MontageInstance->OnMontageBlendingOutStarted.Unbind();
							MontageInstance->OnMontageEnded.Unbind();
						}
						ASC->CurrentMontageStop();
						return true;
					}
				}
			}
		}
	}
	return false;
}

void UAbilityTask_PlayMontage::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (IsValid(Ability))
	{
		if (Ability->GetCurrentMontage() == MontageToPlay)
		{
			if (MontageToPlay == Montage)
			{
				if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
				{
					ASC->ClearAnimatingAbility(Ability);
				}
				if (ACharacter* Character = Cast<ACharacter>(GetAvatarActor()))
				{
					if (Character->GetLocalRole() == ROLE_Authority || (Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted))
					{
						Character->SetAnimRootMotionTranslationScale(1.f);
					}
				}
			}
		}
	}
	if (bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnInterrupted.ExecuteIfBound();
		}
	}
	else
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnBlendOut.ExecuteIfBound();
		}
	}
}

void UAbilityTask_PlayMontage::OnMontageInterrupted()
{
	if (StopPlayingMontage())
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnInterrupted.ExecuteIfBound();
		}
	}
}

void UAbilityTask_PlayMontage::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCompleted.ExecuteIfBound();
		}
	}
	EndTask();
}
