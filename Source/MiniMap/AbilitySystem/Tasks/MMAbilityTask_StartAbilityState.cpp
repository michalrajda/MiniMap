// Fill out your copyright notice in the Description page of Project Settings.


#include "MMAbilityTask_StartAbilityState.h"

UMMAbilityTask_StartAbilityState* UMMAbilityTask_StartAbilityState::CreateStartAbilityStateTask(UGameplayAbility* OwningAbility, FName StateName, bool bEndCurrentState)
{
	UMMAbilityTask_StartAbilityState* MyObj = NewAbilityTask<UMMAbilityTask_StartAbilityState>(OwningAbility, StateName);
	if (IsValid(MyObj))
	{
		MyObj->bEndCurrentState = bEndCurrentState;
	}
	return MyObj;
}

void UMMAbilityTask_StartAbilityState::Activate()
{
	if (IsValid(Ability))
	{
		if (bEndCurrentState && Ability->OnGameplayAbilityStateEnded.IsBound())
		{
			Ability->OnGameplayAbilityStateEnded.Broadcast(NAME_None);
		}
		EndStateHandle = Ability->OnGameplayAbilityStateEnded.AddUObject(this, &UMMAbilityTask_StartAbilityState::OnEndState);
		InterruptStateHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UMMAbilityTask_StartAbilityState::OnInterruptState);
	}
}

void UMMAbilityTask_StartAbilityState::ExternalCancel()
{
	bWasInterrupted = true;
	Super::ExternalCancel();
}

void UMMAbilityTask_StartAbilityState::OnDestroy(bool AbilityEnded)
{
	if (IsValid(Ability))
	{
		Ability->OnGameplayAbilityCancelled.Remove(InterruptStateHandle);
		Ability->OnGameplayAbilityStateEnded.Remove(EndStateHandle);
	}
	if (bWasInterrupted && OnStateInterrupted.IsBound())
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnStateInterrupted.Execute();
		}
	}
	else if ((bWasEnded || AbilityEnded) && OnStateEnded.IsBound())
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnStateEnded.Execute();
		}
	}
	Super::OnDestroy(AbilityEnded);
}

void UMMAbilityTask_StartAbilityState::OnEndState(FName StateNameToEnd)
{
	if (StateNameToEnd.IsNone() || StateNameToEnd == InstanceName)
	{
		bWasEnded = true;
		EndTask();
	}
}

void UMMAbilityTask_StartAbilityState::OnInterruptState()
{
	bWasInterrupted = true;
}
