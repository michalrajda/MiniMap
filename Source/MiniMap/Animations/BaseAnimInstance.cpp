// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/BaseAnimInstance.h"
#include "AbilitySystem/MMAbilitySystemComponent.h"
#include "Character/MMCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBaseAnimInstance::LinkAnimClassLayers(TSubclassOf<UAnimInstance> InClass)
{
	SavePoseSnapshot(SavedPoseSnapshot);
	Super::LinkAnimClassLayers(InClass);
	bWasLayerChanged = true;
}

void UBaseAnimInstance::NativeInitializeAnimation()
{
	if (AMMCharacter* Char = Cast<AMMCharacter>(GetOwningActor()))
	{
		UCharacterMovementComponent* MovComp = Char->GetCharacterMovement();
		if (IsValid(MovComp))
		{
			Character = Char;
			MovementComponent = Char->GetCharacterMovement();
			MaxWalkSpeed = FMath::Max(1.0f, MovementComponent->MaxWalkSpeed);
			MaxCrouchedSpeed = FMath::Max(1.0f, MovementComponent->MaxWalkSpeedCrouched);
		}
		USkeletalMeshComponent* SkelMesh = Char->GetMesh();
		if (IsValid(SkelMesh))
		{
			Mesh = SkelMesh;
		}
	}
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (IsValid(MovementComponent))
	{
		Velocity = MovementComponent->Velocity;
		Acceleration = MovementComponent->GetCurrentAcceleration();
		bIsFalling = MovementComponent->IsFalling();
		bIsCrouching = Character->bIsCrouched;
	}
	CalculateIKRigGoals(DeltaSeconds);
}

void UBaseAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (IsValid(MovementComponent))
	{
		const float GroundSpeed = Velocity.Size2D();
		AlfaSpeed = bIsCrouching ? (GroundSpeed / MaxCrouchedSpeed) : (GroundSpeed / MaxWalkSpeed);
		bShouldMove = (GroundSpeed > MinVelocityTreshold) && (!Acceleration.IsNearlyZero());
		bIsJumping = (Velocity.Z > MinVelocityForJumping) && bIsFalling;
		bIsDoingAnything = bShouldMove || bIsFalling || bIsCrouching;
	}
}

void UBaseAnimInstance::CalculateIKRigGoals(float DeltaSeconds)
{
	if (IsValid(Mesh))
	{
		float LeftFootIKGoal = 0.0f;
		float RightFootIKGoal = 0.0f;
		if (!bIsFalling)
		{
			const float RootZLocation = Mesh->GetSocketLocation(RootBoneName).Z;
			LeftFootIKGoal = GetFootGrountDistance(Mesh->GetSocketLocation(LeftFootBoneName), RootZLocation);
			RightFootIKGoal = GetFootGrountDistance(Mesh->GetSocketLocation(RightFootBoneName), RootZLocation);
			if (LeftFootIKGoal == 0.0f)
			{
				LeftFootIKGoal = RightFootIKGoal;
			}
			if (RightFootIKGoal == 0.0f)
			{
				RightFootIKGoal = LeftFootIKGoal;
			}
		}
		float Step = DeltaSeconds * IKInterpSpeed;
		LeftFootIKPosition.Z = InterpConstant(LeftFootIKPosition.Z, LeftFootIKGoal, Step);
		RightFootIKPosition.Z = InterpConstant(RightFootIKPosition.Z, RightFootIKGoal, Step);
		PelvisIKPosition.Z = LeftFootIKPosition.Z > RightFootIKPosition.Z ? RightFootIKPosition.Z : LeftFootIKPosition.Z;
	}
}

float UBaseAnimInstance::GetFootGrountDistance(const FVector FootLocation, const float RootLocationZ)
{
	if (UWorld* World = GetWorld())
	{
		FVector StartLocation(FootLocation.X, FootLocation.Y, RootLocationZ + 30.f);
		const float EndZLocation = bIsCrouching ? (RootLocationZ - SphereTraceCrouchDistance) : (RootLocationZ - SphereTraceStandDistance);
		FVector EndLocation(FootLocation.X, FootLocation.Y, EndZLocation);
		FCollisionQueryParams QueryParams(TEXT("GrountDistance"), SCENE_QUERY_STAT_ONLY(UBaseAnimInstance_GetFootGrountDistance), false, Character);
		FCollisionResponseParams ResponseParam;
		FHitResult HitResult;
		bool bHit = World->SweepSingleByChannel(HitResult, StartLocation, EndLocation, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(3.0f), QueryParams, ResponseParam);
		return bHit ? HitResult.ImpactPoint.Z - RootLocationZ : 0.0f;
	}
	return 0.0f;
}
