// Fill out your copyright notice in the Description page of Project Settings.


#include "MMAnimInstanceLayer.h"
#include "Character/MMCharacter.h"
#include "Animations/BaseAnimInstance.h"

void UMMAnimInstanceLayer::NativeInitializeAnimation()
{
	if (AMMCharacter* Char = Cast<AMMCharacter>(GetOwningActor()))
	{
		USkeletalMeshComponent* Mesh = Char->GetMesh();
		if (IsValid(Mesh))
		{
			if (UBaseAnimInstance* BaseAnim = Cast<UBaseAnimInstance>(Mesh->GetAnimInstance()))
			{
				BaseAnimInstance = BaseAnim;
			}
		}
	}
}