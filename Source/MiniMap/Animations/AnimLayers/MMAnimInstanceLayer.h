// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MMAnimInstanceLayer.generated.h"

class UBaseAnimInstance;
class UAnimSequence;
class UBlendSpace;

UCLASS()
class MINIMAP_API UMMAnimInstanceLayer : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient)
	TObjectPtr<UBaseAnimInstance> BaseAnimInstance = nullptr;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, noClear)
	TObjectPtr<UBlendSpace> IdleRunStandBlendSpace = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, noClear)
	TObjectPtr<UBlendSpace> IdleRunCrouchBlendSpace = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, noClear)
	TObjectPtr<UAnimSequence> UpperArmRefPose = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, noClear)
	TObjectPtr<UAnimSequence> IdleStandAnimSequence = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, noClear)
	TObjectPtr<UAnimSequence> IdleCrouchAnimSequence = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, noClear)
	TObjectPtr<UAnimSequence> StartJumpAnimSequence = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, noClear)
	TObjectPtr<UAnimSequence> FallLoopAnimSequence = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, noClear)
	TObjectPtr<UAnimSequence> LandAnimSequence = nullptr;
};
