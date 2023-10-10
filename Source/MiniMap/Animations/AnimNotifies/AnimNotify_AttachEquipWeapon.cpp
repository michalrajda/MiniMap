// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttachEquipWeapon.h"
#include "Character/WeaponManagerComponent.h"

void UAnimNotify_AttachEquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (IsValid(MeshComp))
	{
		if (UWeaponManagerComponent* WeaponManager = UWeaponManagerComponent::FindWeaponManagerComponent(MeshComp->GetOwner()))
		{
			WeaponManager->AttachActiveWeapon(MeshComp, true);
		}
	}
}
