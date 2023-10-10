// Fill out your copyright notice in the Description page of Project Settings.


#include "MMGameplayAbility_SwapEquipping.h"
#include "AbilitySystem/Tasks/AbilityTask_PlayMontage.h"
#include "Character/WeaponManagerComponent.h"
#include "Weapons/WeaponInstance.h"

void UMMGameplayAbility_SwapEquipping::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (UWeaponManagerComponent* WeaponManager = UWeaponManagerComponent::FindWeaponManagerComponent(GetAvatarActorFromActorInfo()))
	{
		bShouldBeWeaponEquipped = !WeaponManager->IsWeaponEuipped();
		UWeaponInstance* WI = WeaponManager->GetActiveWeaponInstance();
		if (IsValid(WI))
		{
			PlayMontageTask = UAbilityTask_PlayMontage::CreatePlayMontage(this, WI->GetAnimMontageForEquipping(bShouldBeWeaponEquipped));
			if (IsValid(PlayMontageTask))
			{
				PlayMontageTask->OnCancelled.BindUObject(this, &UMMGameplayAbility_SwapEquipping::OnMontageEnded, false);
				PlayMontageTask->OnInterrupted.BindUObject(this, &UMMGameplayAbility_SwapEquipping::OnMontageEnded, false);
				PlayMontageTask->OnCompleted.BindUObject(this, &UMMGameplayAbility_SwapEquipping::OnMontageEnded, true);
				if (bShouldBeWeaponEquipped)
				{
					WI->OnEquip();
				}
				else
				{
					WI->OnUnequip();
				}
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

void UMMGameplayAbility_SwapEquipping::OnMontageEnded(bool bWasSuccessful)
{
	if (UWeaponManagerComponent* WeaponManager = UWeaponManagerComponent::FindWeaponManagerComponent(GetAvatarActorFromActorInfo()))
	{
		if (bShouldBeWeaponEquipped)
		{
			WeaponManager->OnWeaponEquipped(bWasSuccessful);
		}
		else
		{
			WeaponManager->OnWeaponUnequipped(bWasSuccessful);
		}
	}
	bool bReplicateEndAbility = true;
	bool bWasCancelled = !bWasSuccessful;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
