// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MMGameplayAbility.h"
#include "MMGameplayAbility_SwapEquipping.generated.h"

class UAbilityTask_PlayMontage;

UCLASS()
class MINIMAP_API UMMGameplayAbility_SwapEquipping : public UMMGameplayAbility
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageEnded(bool bWasSuccessful);

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UAbilityTask_PlayMontage> PlayMontageTask = nullptr;

	bool bShouldBeWeaponEquipped = false;
};
