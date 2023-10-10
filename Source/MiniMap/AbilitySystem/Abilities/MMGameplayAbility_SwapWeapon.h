// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMGameplayAbility.h"
#include "MMGameplayAbility_SwapWeapon.generated.h"

class UMMAbilityTask_WaitAbilityEnded;
class UMMAbilityTask_WaitGameplayEvent;

UCLASS()
class MINIMAP_API UMMGameplayAbility_SwapWeapon : public UMMGameplayAbility
{
	GENERATED_BODY()

protected:

	UMMGameplayAbility_SwapWeapon();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void OnGameplayEventReceived(FGameplayEventData EventData);

	void OnGameplayAbilityEndedReceived(bool bWasSuccessful);

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UMMAbilityTask_WaitGameplayEvent> WaitGameplayEventTask = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UMMAbilityTask_WaitAbilityEnded> WaitAbilityEndedTask = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTagContainer BlockingInputEventTags;

	int32 NextActiveWeaponIndex = -1;
};
