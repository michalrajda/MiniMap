// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMGameplayAbility.h"
#include "MMGameplayAbility_Jump.generated.h"

class UMMAbilityTask_WaitInputReleased;
class UMMAbilityTask_StartAbilityState;

UCLASS()
class MINIMAP_API UMMGameplayAbility_Jump : public UMMGameplayAbility
{
	GENERATED_BODY()

protected:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	void CharacterJumpStart();

	void CharacterJumpStop();

	void OnInputReleased();

	UPROPERTY(BlueprintReadOnly, Transient)
	UMMAbilityTask_WaitInputReleased* InputReleasedTask = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	UMMAbilityTask_StartAbilityState* StartAbilityStateTask = nullptr;
};
