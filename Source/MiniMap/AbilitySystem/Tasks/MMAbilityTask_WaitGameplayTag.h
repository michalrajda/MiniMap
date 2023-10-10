// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "MMAbilityTask_WaitGameplayTag.generated.h"

DECLARE_DELEGATE(FWaitGameplayTagDelegate)

UCLASS()
class MINIMAP_API UMMAbilityTask_WaitGameplayTagAdded : public UAbilityTask
{
	GENERATED_BODY()

public:

	FWaitGameplayTagDelegate OnGameplayTagAdded;

	static UMMAbilityTask_WaitGameplayTagAdded* CreateWaitGameplayTagTask(UGameplayAbility* OwningAbility, FGameplayTag Tag, bool OnlyTriggerOnce = false);

protected:

	virtual void Activate() override;

	virtual void OnDestroy(bool AbilityIsEnding) override;

	UFUNCTION()
	void GameplayTagCallback(const FGameplayTag InTag, int32 NewCount);

	FGameplayTag Tag;

	FDelegateHandle DelegateHandle;

	bool RegisteredCallback = false;

	bool OnlyTriggerOnce = false;
};

UCLASS()
class MINIMAP_API UMMAbilityTask_WaitGameplayTagRemoved : public UAbilityTask
{
	GENERATED_BODY()

public:

	FWaitGameplayTagDelegate OnGameplayTagRemoved;

	static UMMAbilityTask_WaitGameplayTagRemoved* CreateWaitGameplayTagTask(UGameplayAbility* OwningAbility, FGameplayTag Tag, bool OnlyTriggerOnce = false);

protected:

	virtual void Activate() override;

	virtual void OnDestroy(bool AbilityIsEnding) override;

	UFUNCTION()
	void GameplayTagCallback(const FGameplayTag InTag, int32 NewCount);

	FGameplayTag Tag;

	FDelegateHandle DelegateHandle;

	bool RegisteredCallback = false;

	bool OnlyTriggerOnce = false;
};

DECLARE_DELEGATE_OneParam(FWaitGameplayEventDelegate, FGameplayEventData)

UCLASS()
class MINIMAP_API UMMAbilityTask_WaitGameplayEvent : public UAbilityTask
{
	GENERATED_BODY()

public:

	FWaitGameplayEventDelegate EventReceived;

	static UMMAbilityTask_WaitGameplayEvent* CreateWaitGameplayEventTask(UGameplayAbility* OwningAbility, FGameplayTag Tag, bool OnlyTriggerOnce = false);

protected:

	virtual void Activate() override;

	virtual void OnDestroy(bool AbilityIsEnding) override;

	void GameplayEventCallback(const FGameplayEventData* Payload);

	FGameplayTag Tag;

	FDelegateHandle DelegateHandle;

	bool RegisteredCallback = false;

	bool OnlyTriggerOnce = false;
};

DECLARE_DELEGATE(FWaitGameplayAbilityEndedDelegate)

UCLASS()
class MINIMAP_API UMMAbilityTask_WaitAbilityEnded : public UAbilityTask
{
	GENERATED_BODY()

public:

	FWaitGameplayAbilityEndedDelegate OnAbilityEnded;

	FWaitGameplayAbilityEndedDelegate OnAbilityCancelled;

	static UMMAbilityTask_WaitAbilityEnded* CreateWaitAbilityEndedTask(UGameplayAbility* OwningAbility, FGameplayTag InWithTag, bool OnlyTriggerOnce = false);

protected:

	virtual void Activate() override;

	virtual void OnDestroy(bool AbilityIsEnding) override;

	UFUNCTION()
	void GameplayEventCallback(const FAbilityEndedData& AbilityEndedData);

	FGameplayTag WithTag;

	FDelegateHandle DelegateHandle;

	bool RegisteredCallback = false;

	bool OnlyTriggerOnce = false;
};