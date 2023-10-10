// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Animation/AnimInstance.h"
#include "AbilityTask_PlayMontage.generated.h"

DECLARE_DELEGATE(FMontageWaitDelegate)

UCLASS()
class MINIMAP_API UAbilityTask_PlayMontage : public UAbilityTask
{
	GENERATED_BODY()

public:

	virtual void ExternalCancel() override;

	FMontageWaitDelegate OnCompleted;

	FMontageWaitDelegate OnInterrupted;

	FMontageWaitDelegate OnCancelled;

	FMontageWaitDelegate OnBlendOut;

	static UAbilityTask_PlayMontage* CreatePlayMontage(UGameplayAbility* OwningAbility, UAnimMontage* MontageToPlay, FName TaskInstanceName = NAME_None, float Rate = 1.f, FName StartSection = NAME_None, bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f, float StartTimeSeconds = 0.f);

protected:

	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

	bool StopPlayingMontage();

	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnMontageInterrupted();

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UAnimMontage> MontageToPlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	float Rate = 0.0f;

	UPROPERTY(BlueprintReadOnly, Transient)
	FName StartSection = NAME_None;

	UPROPERTY()
	float AnimRootMotionTranslationScale = 0.0f;

	UPROPERTY(BlueprintReadOnly, Transient)
	float StartTimeSecond = 0.0f;

	UPROPERTY(BlueprintReadOnly, Transient)
	bool bStopWhenAbilityEnds = false;

	UPROPERTY(BlueprintReadOnly, Transient)
	float StartTimeSeconds = 0.0f;

	FOnMontageBlendingOutStarted BlendingOutDelegate;

	FOnMontageEnded MontageEndedDelegate;

	FDelegateHandle InterruptedHandle;
};
