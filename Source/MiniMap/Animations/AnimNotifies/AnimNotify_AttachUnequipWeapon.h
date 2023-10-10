// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AttachUnequipWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MINIMAP_API UAnimNotify_AttachUnequipWeapon : public UAnimNotify
{
	GENERATED_BODY()

public:

#if WITH_EDITOR
	virtual bool ShouldFireInEditor() override { return false; }
#endif
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
