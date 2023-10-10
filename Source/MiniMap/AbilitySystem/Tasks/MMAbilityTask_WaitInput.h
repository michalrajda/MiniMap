// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MMAbilityTask_WaitInput.generated.h"

DECLARE_DELEGATE(FInputDelegate);

UCLASS()
class MINIMAP_API UMMAbilityTask_WaitInputPressed : public UAbilityTask
{
	GENERATED_BODY()

public:

	FInputDelegate OnInputPressed;

	static UMMAbilityTask_WaitInputPressed* CreateWaitInputPressedTask(UGameplayAbility* OwningAbility, bool bTestAlreadyPressed = false);

protected:

	virtual void Activate() override;

	UFUNCTION()
	void InputPressedCallback();

	bool bTestInitialState = false;

	FDelegateHandle DelegateHandle;
};

UCLASS()
class MINIMAP_API UMMAbilityTask_WaitInputReleased : public UAbilityTask
{
	GENERATED_BODY()

public:

	FInputDelegate OnInputReleased;

	static UMMAbilityTask_WaitInputReleased* CreateWaitInputReleasedTask(UGameplayAbility* OwningAbility, bool bTestAlreadyReleased = false);

protected:

	virtual void Activate() override;

	UFUNCTION()
	void InputReleasedCallback();

	bool bTestInitialState = false;

	FDelegateHandle DelegateHandle;
};
