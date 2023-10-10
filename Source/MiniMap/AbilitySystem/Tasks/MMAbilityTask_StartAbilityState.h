// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MMAbilityTask_StartAbilityState.generated.h"

DECLARE_DELEGATE(FAbilityStateDelegate)

UCLASS()
class MINIMAP_API UMMAbilityTask_StartAbilityState : public UAbilityTask
{
	GENERATED_BODY()

public:

	FAbilityStateDelegate OnStateEnded;

	FAbilityStateDelegate OnStateInterrupted;

	static UMMAbilityTask_StartAbilityState* CreateStartAbilityStateTask(UGameplayAbility* OwningAbility, FName StateName, bool bEndCurrentState = true);

protected:

	virtual void Activate() override;

	virtual void ExternalCancel() override;

	virtual void OnDestroy(bool AbilityEnded) override;

	void OnEndState(FName StateNameToEnd);

	void OnInterruptState();

	FDelegateHandle EndStateHandle;

	FDelegateHandle InterruptStateHandle;

	bool bWasEnded = false;

	bool bWasInterrupted = false;

	bool bEndCurrentState = true;
};
