// Fill out your copyright notice in the Description page of Project Settings.


#include "MMGameplayAbility_SwapWeapon.h"
#include "AbilitySystem/Tasks/MMAbilityTask_WaitGameplayTag.h"
#include "Character/WeaponManagerComponent.h"
#include "Weapons/WeaponInstance.h"
#include "AbilitySystem/MMAbilitySystemComponent.h"
#include "NativeTags/Tags.h"

UMMGameplayAbility_SwapWeapon::UMMGameplayAbility_SwapWeapon()
{
	bAutoActivate = true;
}

void UMMGameplayAbility_SwapWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	WaitGameplayEventTask = UMMAbilityTask_WaitGameplayEvent::CreateWaitGameplayEventTask(this, MMGameplayTags::GameplayEvent_SwapWeapon);
	if (IsValid(WaitGameplayEventTask))
	{
		WaitGameplayEventTask->EventReceived.BindUObject(this, &UMMGameplayAbility_SwapWeapon::OnGameplayEventReceived);
		WaitGameplayEventTask->ReadyForActivation();
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
	else
	{
		bool bReplicateEndAbility = true;
		bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
}

void UMMGameplayAbility_SwapWeapon::OnGameplayEventReceived(FGameplayEventData EventData)
{
	const FGameplayTag& EventTag = EventData.EventTag;
	if (UMMAbilitySystemComponent* MMASC = GetMMAbilitySystemComponentFromActorInfo())
	{
		if (!MMASC->HasAnyMatchingGameplayTags(BlockingInputEventTags))
		{
			if (EventTag.IsValid() && EventTag.MatchesTagExact(MMGameplayTags::GameplayEvent_SwapWeapon))
			{
				if (UWeaponManagerComponent* WeaponManager = UWeaponManagerComponent::FindWeaponManagerComponent(GetAvatarActorFromActorInfo()))
				{
					NextActiveWeaponIndex = FMath::TruncToInt(EventData.EventMagnitude);
					if (!WeaponManager->ChangeActiveWeapon(NextActiveWeaponIndex))
					{
						if (IsValid(WaitAbilityEndedTask))
						{
							WaitAbilityEndedTask->EndTask();
						}
						FGameplayEventData NewEventData;
						NewEventData.EventTag = MMGameplayTags::GameplayEvent_SwapEquipping;
						FScopedPredictionWindow NewScopedWindow(MMASC, true);
						if (MMASC->HandleGameplayEvent(MMGameplayTags::GameplayEvent_SwapEquipping, &NewEventData))
						{
							WaitAbilityEndedTask = UMMAbilityTask_WaitAbilityEnded::CreateWaitAbilityEndedTask(this, MMGameplayTags::GameplayAbility_SwapEquipping, true);
							if (IsValid(WaitAbilityEndedTask))
							{
								WaitAbilityEndedTask->OnAbilityEnded.BindUObject(this, &UMMGameplayAbility_SwapWeapon::OnGameplayAbilityEndedReceived, true);
								WaitAbilityEndedTask->OnAbilityCancelled.BindUObject(this, &UMMGameplayAbility_SwapWeapon::OnGameplayAbilityEndedReceived, false);
								WaitAbilityEndedTask->ReadyForActivation();
							}
						}
					}
				}
			}
		}
	}
}



//void UMMGameplayAbility_SwapWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
//{
//	bool bWasCancelled = true;
//	if (TriggerEventData)
//	{
//		if (UWeaponManagerComponent* WeaponManager = UWeaponManagerComponent::FindWeaponManagerComponent(GetAvatarActorFromActorInfo()))
//		{
//			NextActiveWeaponIndex = FMath::TruncToInt(TriggerEventData->EventMagnitude);
//			if (!WeaponManager->ChangeActiveWeapon(NextActiveWeaponIndex))
//			{
//				if (UMMAbilitySystemComponent* MMASC = GetMMAbilitySystemComponentFromActorInfo())
//				{
//					if (IsValid(WaitAbilityEndedTask))
//					{
//						WaitAbilityEndedTask->EndTask();
//					}
//					FGameplayEventData NewEventData;
//					NewEventData.EventTag = MMGameplayTags::GameplayEvent_SwapEquipping;
//					FScopedPredictionWindow NewScopedWindow(MMASC, true);
//					if (MMASC->HandleGameplayEvent(MMGameplayTags::GameplayEvent_SwapEquipping, &NewEventData))
//					{
//						WaitAbilityEndedTask = UMMAbilityTask_WaitAbilityEnded::CreateWaitAbilityEndedTask(this, MMGameplayTags::GameplayAbility_SwapEquipping, true);
//						if (IsValid(WaitAbilityEndedTask))
//						{
//							WaitAbilityEndedTask->OnAbilityEnded.BindUObject(this, &UMMGameplayAbility_SwapWeapon::OnGameplayAbilityEndedReceived, true);
//							WaitAbilityEndedTask->OnAbilityCancelled.BindUObject(this, &UMMGameplayAbility_SwapWeapon::OnGameplayAbilityEndedReceived, false);
//							WaitAbilityEndedTask->ReadyForActivation();
//							Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
//							return;
//						}
//					}
//				}
//			}
//			Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
//			bWasCancelled = false;
//		}
//	}
//	bool bReplicateEndAbility = true;
//	EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
//}

void UMMGameplayAbility_SwapWeapon::OnGameplayAbilityEndedReceived(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (UWeaponManagerComponent* WeaponManager = UWeaponManagerComponent::FindWeaponManagerComponent(GetAvatarActorFromActorInfo()))
		{
			if (WeaponManager->ChangeActiveWeapon(NextActiveWeaponIndex))
			{
				if (UMMAbilitySystemComponent* MMASC = GetMMAbilitySystemComponentFromActorInfo())
				{
					FGameplayEventData NewEventData;
					NewEventData.EventTag = MMGameplayTags::GameplayEvent_SwapEquipping;
					FScopedPredictionWindow NewScopedWindow(MMASC, true);
					MMASC->HandleGameplayEvent(MMGameplayTags::GameplayEvent_SwapEquipping, &NewEventData);
				}
			}
		}
	}
	//bool bReplicateEndAbility = true;
	//bool bWasCancelled = !bWasSuccessful;
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
