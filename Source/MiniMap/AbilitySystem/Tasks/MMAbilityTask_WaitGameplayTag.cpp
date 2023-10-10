// Fill out your copyright notice in the Description page of Project Settings.


#include "MMAbilityTask_WaitGameplayTag.h"
#include "AbilitySystemComponent.h"

UMMAbilityTask_WaitGameplayTagAdded* UMMAbilityTask_WaitGameplayTagAdded::CreateWaitGameplayTagTask(UGameplayAbility* OwningAbility, FGameplayTag Tag, bool OnlyTriggerOnce)
{
	UMMAbilityTask_WaitGameplayTagAdded* NewObj = NewAbilityTask<UMMAbilityTask_WaitGameplayTagAdded>(OwningAbility);
	if (IsValid(NewObj))
	{
		NewObj->Tag = Tag;
		NewObj->OnlyTriggerOnce = OnlyTriggerOnce;
	}
	return NewObj;
}

void UMMAbilityTask_WaitGameplayTagAdded::Activate()
{
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		if (ASC->HasMatchingGameplayTag(Tag))
		{
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				OnGameplayTagAdded.ExecuteIfBound();
			}
			if (OnlyTriggerOnce)
			{
				EndTask();
				return;
			}
		}
		DelegateHandle = ASC->RegisterGameplayTagEvent(Tag).AddUObject(this, &UMMAbilityTask_WaitGameplayTagAdded::GameplayTagCallback);
		RegisteredCallback = true;
	}
}

void UMMAbilityTask_WaitGameplayTagAdded::OnDestroy(bool AbilityIsEnding)
{
	if (RegisteredCallback)
	{
		if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
		{
			ASC->UnregisterGameplayTagEvent(DelegateHandle, Tag);
		}
	}
	Super::OnDestroy(AbilityIsEnding);
}

void UMMAbilityTask_WaitGameplayTagAdded::GameplayTagCallback(const FGameplayTag InTag, int32 NewCount)
{
	if (NewCount == 1)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnGameplayTagAdded.ExecuteIfBound();
		}
		if (OnlyTriggerOnce)
		{
			EndTask();
		}
	}
}

UMMAbilityTask_WaitGameplayTagRemoved* UMMAbilityTask_WaitGameplayTagRemoved::CreateWaitGameplayTagTask(UGameplayAbility* OwningAbility, FGameplayTag Tag, bool OnlyTriggerOnce)
{
	UMMAbilityTask_WaitGameplayTagRemoved* NewObj = NewAbilityTask<UMMAbilityTask_WaitGameplayTagRemoved>(OwningAbility);
	if (IsValid(NewObj))
	{
		NewObj->Tag = Tag;
		NewObj->OnlyTriggerOnce = OnlyTriggerOnce;
	}
	return NewObj;
}

void UMMAbilityTask_WaitGameplayTagRemoved::Activate()
{
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		if (!ASC->HasMatchingGameplayTag(Tag))
		{
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				OnGameplayTagRemoved.ExecuteIfBound();
			}
			if (OnlyTriggerOnce)
			{
				EndTask();
				return;
			}
		}
		DelegateHandle = ASC->RegisterGameplayTagEvent(Tag).AddUObject(this, &UMMAbilityTask_WaitGameplayTagRemoved::GameplayTagCallback);
		RegisteredCallback = true;
	}
}

void UMMAbilityTask_WaitGameplayTagRemoved::OnDestroy(bool AbilityIsEnding)
{
	if (RegisteredCallback)
	{
		if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
		{
			ASC->UnregisterGameplayTagEvent(DelegateHandle, Tag);
		}
	}
	Super::OnDestroy(AbilityIsEnding);
}

void UMMAbilityTask_WaitGameplayTagRemoved::GameplayTagCallback(const FGameplayTag InTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnGameplayTagRemoved.ExecuteIfBound();
		}
		if (OnlyTriggerOnce)
		{
			EndTask();
		}
	}
}

UMMAbilityTask_WaitGameplayEvent* UMMAbilityTask_WaitGameplayEvent::CreateWaitGameplayEventTask(UGameplayAbility* OwningAbility, FGameplayTag Tag, bool OnlyTriggerOnce)
{
	UMMAbilityTask_WaitGameplayEvent* MyObj = NewAbilityTask<UMMAbilityTask_WaitGameplayEvent>(OwningAbility);
	if (IsValid(MyObj))
	{
		MyObj->Tag = Tag;
		MyObj->OnlyTriggerOnce = OnlyTriggerOnce;
	}
	return MyObj;
}

void UMMAbilityTask_WaitGameplayEvent::Activate()
{
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		DelegateHandle = ASC->GenericGameplayEventCallbacks.FindOrAdd(Tag).AddUObject(this, &UMMAbilityTask_WaitGameplayEvent::GameplayEventCallback);
		RegisteredCallback = true;
	}
	Super::Activate();
}

void UMMAbilityTask_WaitGameplayEvent::OnDestroy(bool AbilityIsEnding)
{
	if (RegisteredCallback)
	{
		if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
		{
			ASC->GenericGameplayEventCallbacks.FindOrAdd(Tag).Remove(DelegateHandle);
		}
	}
	Super::OnDestroy(AbilityIsEnding);
}

void UMMAbilityTask_WaitGameplayEvent::GameplayEventCallback(const FGameplayEventData* Payload)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FGameplayEventData TempPayload = *Payload;
		TempPayload.EventTag = Tag;
		EventReceived.ExecuteIfBound(MoveTemp(TempPayload));
	}
	if (OnlyTriggerOnce)
	{
		EndTask();
	}
}

UMMAbilityTask_WaitAbilityEnded* UMMAbilityTask_WaitAbilityEnded::CreateWaitAbilityEndedTask(UGameplayAbility* OwningAbility, FGameplayTag InWithTag, bool OnlyTriggerOnce)
{
	UMMAbilityTask_WaitAbilityEnded* MyObj = NewAbilityTask<UMMAbilityTask_WaitAbilityEnded>(OwningAbility);
	if (IsValid(MyObj))
	{
		MyObj->WithTag = InWithTag;
		MyObj->OnlyTriggerOnce = OnlyTriggerOnce;
	}
	return MyObj;
}

void UMMAbilityTask_WaitAbilityEnded::Activate()
{
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
	{
		DelegateHandle = AbilitySystemComponent->OnAbilityEnded.AddUObject(this, &UMMAbilityTask_WaitAbilityEnded::GameplayEventCallback);
		RegisteredCallback = true;
	}
}

void UMMAbilityTask_WaitAbilityEnded::OnDestroy(bool AbilityIsEnding)
{
	if (RegisteredCallback)
	{
		if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get())
		{
			AbilitySystemComponent->OnAbilityEnded.Remove(DelegateHandle);
		}
	}
	Super::OnDestroy(AbilityIsEnding);
}

void UMMAbilityTask_WaitAbilityEnded::GameplayEventCallback(const FAbilityEndedData& AbilityEndedData)
{
	if (WithTag.IsValid())
	{
		UGameplayAbility* EndedAbility = AbilityEndedData.AbilityThatEnded;
		if (IsValid(EndedAbility) && EndedAbility->AbilityTags.HasTagExact(WithTag))
		{
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				if (AbilityEndedData.bWasCancelled)
				{
					OnAbilityCancelled.ExecuteIfBound();
				}
				else
				{
					OnAbilityEnded.ExecuteIfBound();
				}
			}
			if (OnlyTriggerOnce)
			{
				EndTask();
			}
		}
	}
}
