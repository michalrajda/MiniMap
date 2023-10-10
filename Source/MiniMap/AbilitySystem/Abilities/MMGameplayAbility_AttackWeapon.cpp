// Fill out your copyright notice in the Description page of Project Settings.


#include "MMGameplayAbility_AttackWeapon.h"
#include "AbilitySystem/Tasks/AbilityTask_PlayMontage.h"
#include "Character/WeaponManagerComponent.h"
#include "Weapons/WeaponInstance.h"

void UMMGameplayAbility_AttackWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (UWeaponManagerComponent* WeaponManager = UWeaponManagerComponent::FindWeaponManagerComponent(GetAvatarActorFromActorInfo()))
	{
		UWeaponInstance* WI = WeaponManager->GetActiveWeaponInstance();
		if (IsValid(WI))
		{
			PlayMontageTask = UAbilityTask_PlayMontage::CreatePlayMontage(this, WI->GetAnimMontageForAttacking(GetAbilityLevel()));
			if (IsValid(PlayMontageTask))
			{
				PlayMontageTask->OnCancelled.BindUObject(this, &UMMGameplayAbility_AttackWeapon::OnMontageEnded, false);
				PlayMontageTask->OnInterrupted.BindUObject(this, &UMMGameplayAbility_AttackWeapon::OnMontageEnded, false);
				PlayMontageTask->OnCompleted.BindUObject(this, &UMMGameplayAbility_AttackWeapon::OnMontageEnded, true);
				PlayMontageTask->ReadyForActivation();
				Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
				return;
			}
		}
	}
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMMGameplayAbility_AttackWeapon::OnMontageEnded(bool bWasSuccessful)
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = !bWasSuccessful;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
