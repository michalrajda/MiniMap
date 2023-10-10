// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponInstance.h"
#include "BaseWeapon.h"
#include "Character/MMCharacter.h"
#include "WeaponDefinition.h"
#include "Animations/AnimLayers/MMAnimLayerSelectionEntry.h"

UWorld* UWeaponInstance::GetWorld() const
{
	if (APawn* Pawn = GetPawn<APawn>())
	{
		return Pawn->GetWorld();
	}
	return nullptr;
}

void UWeaponInstance::BeginDestroy()
{
	DestroyEquipmentActorWeapons();
	Super::BeginDestroy();
}

TSubclassOf<UAnimInstance> UWeaponInstance::GetAnimInstanceClassForEquipping(bool bShouldBeEquipped)
{
	if (IsValid(AnimationsEquippingSet))
	{
		return AnimationsEquippingSet->GetAnimLayer(bShouldBeEquipped);
	}
	return nullptr;
}

UAnimMontage* UWeaponInstance::GetAnimMontageForEquipping(bool bShouldBeEquipped)
{
	if (IsValid(MontagesEquippingSet))
	{
		return MontagesEquippingSet->GetAnimMontage(bShouldBeEquipped);
	}
	return nullptr;
}

void UWeaponInstance::ChangeVisibilitySpawnedActorWeapons(bool bShouldBeVisible)
{
	for (ABaseWeapon* Weapon : SpawnedActorWeapons)
	{
		if (IsValid(Weapon))
		{
			Weapon->SetActorHiddenInGame(!bShouldBeVisible);
			Weapon->SetActorEnableCollision(bShouldBeVisible);
		}
	}
}

void UWeaponInstance::OnEquip()
{
	if (IsValid(AnimationsEquippingSet))
	{
		LinkAnimLayers(AnimationsEquippingSet->GetAnimLayer(true));
	}
}

void UWeaponInstance::OnEquipWithAttaching(USceneComponent* AttachTarget, const TArray<FWeaponToSpawn>& WeaponProperties)
{
	OnEquip();
	AttachActorWeapons(AttachTarget, WeaponProperties, true);
}

void UWeaponInstance::OnUnequip()
{
	if (IsValid(AnimationsEquippingSet))
	{
		LinkAnimLayers(AnimationsEquippingSet->GetAnimLayer(false));
	}
}

void UWeaponInstance::OnUnequipWithAttaching(USceneComponent* AttachTarget, const TArray<FWeaponToSpawn>& WeaponProperties)
{
	OnUnequip();
	AttachActorWeapons(AttachTarget, WeaponProperties, false);
}

void UWeaponInstance::AttachActorWeapons(USceneComponent* AttachTarget, const TArray<FWeaponToSpawn>& WeaponProperties, bool bShouldBeEquipped)
{
	if (IsValid(AttachTarget))
	{
		for (ABaseWeapon* Weapon : SpawnedActorWeapons)
		{
			if (IsValid(Weapon))
			{
				for (const FWeaponToSpawn& WeaponProp : WeaponProperties)
				{
					if (Weapon->IsA(WeaponProp.ActorToSpawn))
					{
						if (bShouldBeEquipped)
						{
							Weapon->SetActorRelativeTransform(WeaponProp.EquippedAttachTransform);
							Weapon->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, WeaponProp.EquippedAttachSocket);
						}
						else
						{
							Weapon->SetActorRelativeTransform(WeaponProp.UnequippedAttachTransform);
							Weapon->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, WeaponProp.UnequippedAttachSocket);
						}
						break;
					}
				}
			}
		}
	}
}

UAnimMontage* UWeaponInstance::GetAnimMontageForAttacking(int32 AbilityLevel)
{
	if (IsValid(AttackMontagesSet))
	{
		return AttackMontagesSet->GetAnimMontage(AbilityLevel);
	}
	return nullptr;
}

void UWeaponInstance::SpawnEquipmentActorWeapons(const TArray<FWeaponToSpawn>& WeaponsToSpawn)
{
	if (AMMCharacter* OwningPawn = GetPawn<AMMCharacter>())
	{
		if (UWorld* World = GetWorld())
		{
			USceneComponent* AttachTarget = OwningPawn->GetMesh();
			if (IsValid(AttachTarget))
			{
				FActorSpawnParameters SpawnParam;
				SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParam.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
				SpawnParam.Owner = OwningPawn;
				SpawnParam.Instigator = OwningPawn;
				SpawnParam.bDeferConstruction = true;
				SpawnParam.ObjectFlags |= RF_Transient;
				for (const FWeaponToSpawn& SpawnInfo : WeaponsToSpawn)
				{
					if (ABaseWeapon* NewWeapon = World->SpawnActor<ABaseWeapon>(SpawnInfo.ActorToSpawn, FTransform::Identity, SpawnParam))
					{
						NewWeapon->SetActorHiddenInGame(true);
						NewWeapon->SetActorEnableCollision(false);
						NewWeapon->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
						NewWeapon->SetActorRelativeTransform(SpawnInfo.UnequippedAttachTransform);
						NewWeapon->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.UnequippedAttachSocket);
						SpawnedActorWeapons.Add(NewWeapon);
					}
				}
			}
		}
	}
}

void UWeaponInstance::DestroyEquipmentActorWeapons()
{
	for (ABaseWeapon* Weapon : SpawnedActorWeapons)
	{
		if (IsValid(Weapon))
		{
			Weapon->Destroy();
		}
	}
	SpawnedActorWeapons.Empty();
}

void UWeaponInstance::LinkAnimLayers(const TSubclassOf<UAnimInstance> AnimLayersToLink)
{
	if (IsValid(AnimLayersToLink))
	{
		if (AMMCharacter* Pawn = GetPawn<AMMCharacter>())
		{
			USkeletalMeshComponent* Mesh = Pawn->GetMesh();
			if (IsValid(Mesh))
			{
				UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
				if (IsValid(AnimInstance))
				{
					AnimInstance->LinkAnimClassLayers(AnimLayersToLink);
				}
			}
		}
	}
}
