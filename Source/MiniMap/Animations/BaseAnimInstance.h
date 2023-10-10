// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayEffectTypes.h"
#include "BaseAnimInstance.generated.h"

class AMMCharacter;
class UCharacterMovementComponent;
class UMMAnimInstanceLayer;

UCLASS()
class MINIMAP_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void LinkAnimClassLayers(TSubclassOf<UAnimInstance> InClass) override;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:

	void CalculateIKRigGoals(float DeltaSeconds);

	FORCEINLINE float InterpConstant(float Current, float Goal, float Step)
	{
		return Current + FMath::Clamp(Goal - Current, -Step, Step);
	}

	float GetFootGrountDistance(const FVector FootLocation, const float RootLocationZ);

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly)
	TObjectPtr<AMMCharacter> Character = nullptr;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly)
	TObjectPtr<UCharacterMovementComponent> MovementComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float MinVelocityTreshold = 3.0f;

	UPROPERTY(EditDefaultsOnly)
	float MinVelocityForJumping = 100.0f;

	UPROPERTY(EditDefaultsOnly)
	float SphereTraceCrouchDistance = 20.0f;

	UPROPERTY(EditDefaultsOnly)
	float SphereTraceStandDistance = 50.0f;

	float MaxWalkSpeed = 1.0f;

	float MaxCrouchedSpeed = 1.0f;

	FVector Velocity = FVector::ZeroVector;

	FVector Acceleration = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly)
	float AlfaSpeed = 0.0f;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly)
	bool bShouldMove = false;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly)
	bool bIsFalling = false;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly)
	bool bIsJumping = false;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly)
	bool bIsCrouching = false;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadWrite)
	bool bWasLayerChanged = false;

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly)
	bool bIsDoingAnything = false;

	UPROPERTY(BlueprintReadOnly, Transient)
	FVector PelvisIKPosition = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Transient)
	FVector LeftFootIKPosition = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Transient)
	FVector RightFootIKPosition = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float IKInterpSpeed = 15.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SavedPoseSnapshot = TEXT("SavedPose");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName LeftFootBoneName = TEXT("foot_l");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName RightFootBoneName = TEXT("foot_r");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName RootBoneName = TEXT("root");

	friend UMMAnimInstanceLayer;
};
