// Fill out your copyright notice in the Description page of Project Settings.


#include "MMAbilityTask_WaitInput.h"
#include "AbilitySystem/MMAbilitySystemComponent.h"

UMMAbilityTask_WaitInputPressed* UMMAbilityTask_WaitInputPressed::CreateWaitInputPressedTask(UGameplayAbility* OwningAbility, bool bTestAlreadyPressed)
{
	UMMAbilityTask_WaitInputPressed* MyObj = NewAbilityTask<UMMAbilityTask_WaitInputPressed>(OwningAbility);
	if (IsValid(MyObj))
	{
		MyObj->bTestInitialState = bTestAlreadyPressed;
	}
	return MyObj;
}

void UMMAbilityTask_WaitInputPressed::Activate()
{
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		if (IsValid(Ability))
		{
			if (bTestInitialState && IsLocallyControlled())
			{
				FGameplayAbilitySpec* Spec = Ability->GetCurrentAbilitySpec();
				if (Spec && Spec->InputPressed)
				{
					InputPressedCallback();
					return;
				}
			}
			DelegateHandle = ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputPressed, GetAbilitySpecHandle(), GetActivationPredictionKey()).AddUObject(this, &UMMAbilityTask_WaitInputPressed::InputPressedCallback);
			if (IsForRemoteClient())
			{
				if (!ASC->CallReplicatedEventDelegateIfSet(EAbilityGenericReplicatedEvent::InputPressed, GetAbilitySpecHandle(), GetActivationPredictionKey()))
				{
					SetWaitingOnRemotePlayerData();
				}
			}
		}
	}
}

void UMMAbilityTask_WaitInputPressed::InputPressedCallback()
{
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		if (IsValid(Ability))
		{
			ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputPressed, GetAbilitySpecHandle(), GetActivationPredictionKey()).Remove(DelegateHandle);
			FScopedPredictionWindow ScopedPrediction(ASC, IsPredictingClient());
			if (IsPredictingClient())
			{
				ASC->ServerSetReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, GetAbilitySpecHandle(), GetActivationPredictionKey(), ASC->ScopedPredictionKey);
			}
			else
			{
				ASC->ConsumeGenericReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, GetAbilitySpecHandle(), GetActivationPredictionKey());
			}
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				OnInputPressed.ExecuteIfBound();
			}
		}
	}
	EndTask();
}

UMMAbilityTask_WaitInputReleased* UMMAbilityTask_WaitInputReleased::CreateWaitInputReleasedTask(UGameplayAbility* OwningAbility, bool bTestAlreadyReleased)
{
	UMMAbilityTask_WaitInputReleased* MyObj = NewAbilityTask<UMMAbilityTask_WaitInputReleased>(OwningAbility);
	if (IsValid(MyObj))
	{
		MyObj->bTestInitialState = bTestAlreadyReleased;
	}
	return MyObj;
}

void UMMAbilityTask_WaitInputReleased::Activate()
{
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		if (IsValid(Ability))
		{
			if (bTestInitialState && IsLocallyControlled())
			{
				FGameplayAbilitySpec* Spec = Ability->GetCurrentAbilitySpec();
				if (Spec && !Spec->InputPressed)
				{
					InputReleasedCallback();
					return;
				}
			}
			DelegateHandle = ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey()).AddUObject(this, &UMMAbilityTask_WaitInputReleased::InputReleasedCallback);
			if (IsForRemoteClient())
			{
				if (!ASC->CallReplicatedEventDelegateIfSet(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey()))
				{
					SetWaitingOnRemotePlayerData();
				}
			}
		}
	}
}

void UMMAbilityTask_WaitInputReleased::InputReleasedCallback()
{
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		if (IsValid(Ability))
		{
			ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey()).Remove(DelegateHandle);
			FScopedPredictionWindow ScopedPrediction(ASC, IsPredictingClient());
			if (IsPredictingClient())
			{
				ASC->ServerSetReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey(), ASC->ScopedPredictionKey);
			}
			else
			{
				ASC->ConsumeGenericReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey());
			}
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				OnInputReleased.ExecuteIfBound();
			}
		}
	}
	EndTask();
}
