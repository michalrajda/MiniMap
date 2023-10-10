// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttachUnequipWeapon.h"
#include "Character/WeaponManagerComponent.h"

void UAnimNotify_AttachUnequipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (IsValid(MeshComp))
	{
		if (UWeaponManagerComponent* WeaponManager = UWeaponManagerComponent::FindWeaponManagerComponent(MeshComp->GetOwner()))
		{
			WeaponManager->AttachActiveWeapon(MeshComp, false);
		}
	}
}
