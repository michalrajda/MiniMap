// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManagerComponent.h"
#include "Weapons/WeaponDefinition.h"
#include "Weapons/WeaponInstance.h"
#include "AbilitySystem/MMAbilitySystemComponent.h"
#include "MMCharacter.h"
#include "NativeTags/Tags.h"

FWeaponEntry::FWeaponEntry(UWeaponDefinition* InWeaponDefinition, UWeaponInstance* InWeaponInstance): WeaponDefinition(InWeaponDefinition), WeaponInstance(InWeaponInstance){}

void UWeaponManagerComponent::AddWeapons(const TArray<TSubclassOf<UWeaponDefinition>>& WeaponsDefinitionsToAdd)
{
	for (const TSubclassOf<UWeaponDefinition> WeaponDefinition : WeaponsDefinitionsToAdd)
	{
		AddWeapon(WeaponDefinition);
	}
}

void UWeaponManagerComponent::AddWeapon(const TSubclassOf<UWeaponDefinition> WeaponDefinitionToAdd)
{
	if (IsValid(WeaponDefinitionToAdd))
	{
		if (UWeaponDefinition* WeaponCDO = WeaponDefinitionToAdd->GetDefaultObject<UWeaponDefinition>())
		{
			if (AMMCharacter* Pawn = GetPawn<AMMCharacter>())
			{
				TSubclassOf<UWeaponInstance> InstanceType = WeaponCDO->InstanceType;
				if (InstanceType == nullptr)
				{
					InstanceType = UWeaponInstance::StaticClass();
				}
				if (UWeaponInstance* WeaponInstance = NewObject<UWeaponInstance>(Pawn, InstanceType))
				{
					SpawnedWeapons.Add(FWeaponEntry(WeaponCDO, WeaponInstance));
					WeaponInstance->SpawnEquipmentActorWeapons(WeaponCDO->ActorsToSpawn);
				}
			}
		}
	}
}

void UWeaponManagerComponent::RemoveWeapon(const int32 NumberWeaponToRemove)
{
	if (SpawnedWeapons.IsValidIndex(NumberWeaponToRemove))
	{
		FWeaponEntry& EntryToRemove = SpawnedWeapons[NumberWeaponToRemove];
		UWeaponInstance* InstanceToRemove = EntryToRemove.WeaponInstance;
		if (IsValid(InstanceToRemove))
		{
			if (NumberWeaponToRemove == ActiveWeaponIndex && bIsWeaponEquipped)
			{
				InstanceToRemove->OnUnequip();
				UMMAbilitySystemComponent* MMASC = GetMMAbilitySystemComponent();
				if (IsValid(MMASC))
				{
					EntryToRemove.GrantedHandles.TakeFromAbilitySystem(MMASC);
				}
				ActiveWeaponIndex = -1;
				bIsWeaponEquipped = false;
			}
			InstanceToRemove->MarkAsGarbage();
		}
		SpawnedWeapons.RemoveAt(NumberWeaponToRemove);
	}
}

void UWeaponManagerComponent::UninitializeAndRemoveAllWeapons()
{
	UMMAbilitySystemComponent* MMASC = GetMMAbilitySystemComponent();
	for (FWeaponEntry& Entry : SpawnedWeapons)
	{
		if (IsValid(Entry.WeaponInstance))
		{
			Entry.WeaponInstance->DestroyEquipmentActorWeapons();
		}
		Entry.GrantedHandles.TakeFromAbilitySystem(MMASC);
	}
}

void UWeaponManagerComponent::OnWeaponEquipped(bool bWasSuccessfully)
{
	if (SpawnedWeapons.IsValidIndex(ActiveWeaponIndex))
	{
		FWeaponEntry& ActEntry = SpawnedWeapons[ActiveWeaponIndex];
		UWeaponInstance* ActWeaponInstance = ActEntry.WeaponInstance;
		UWeaponDefinition* ActWeaponDefinition = ActEntry.WeaponDefinition;
		if (IsValid(ActWeaponInstance) && IsValid(ActWeaponDefinition))
		{
			if (bWasSuccessfully)
			{
				FMMAbilitySet_GrantedHandles& ActGrantedSet = ActEntry.GrantedHandles;
				const TArray<TObjectPtr<const UMMAbilitySet>>& Abilities = ActWeaponDefinition->AbilitySetsToGrant;
				UMMAbilitySystemComponent* MMASC = GetMMAbilitySystemComponent();
				if (ActGrantedSet.IsGrantedToAbilitySystem())
				{
					ActGrantedSet.TakeFromAbilitySystem(MMASC);
				}
				for (const TObjectPtr<const UMMAbilitySet>& Ability : Abilities)
				{
					if (IsValid(Ability))
					{
						Ability->GiveToAbilitySystem(MMASC, &ActGrantedSet, ActWeaponInstance);
					}
				}
				bIsWeaponEquipped = true;
				OnChangedEquippingState.ExecuteIfBound(bIsWeaponEquipped);
			}
			else if(AMMCharacter* Char = GetPawn<AMMCharacter>())
			{
				ActWeaponInstance->OnUnequipWithAttaching(Char->GetMesh(), ActWeaponDefinition->ActorsToSpawn);
			}
		}
	}
}

void UWeaponManagerComponent::OnWeaponUnequipped(bool bWasSuccessfully)
{
	if (SpawnedWeapons.IsValidIndex(ActiveWeaponIndex))
	{
		if (bWasSuccessfully)
		{
			SpawnedWeapons[ActiveWeaponIndex].GrantedHandles.TakeFromAbilitySystem(GetMMAbilitySystemComponent());
			bIsWeaponEquipped = false;
			OnChangedEquippingState.ExecuteIfBound(bIsWeaponEquipped);
		}
		else
		{
			const FWeaponEntry& ActEntry = SpawnedWeapons[ActiveWeaponIndex];
			UWeaponInstance* ActWeaponInstance = ActEntry.WeaponInstance;
			UWeaponDefinition* ActWeaponDefinition = ActEntry.WeaponDefinition;
			if (IsValid(ActWeaponInstance) && IsValid(ActWeaponDefinition))
			{
				if (AMMCharacter* Char = GetPawn<AMMCharacter>())
				{
					ActWeaponInstance->OnEquipWithAttaching(Char->GetMesh(),ActWeaponDefinition->ActorsToSpawn);
				}
			}
		}
	}
}

UMMAbilitySystemComponent* UWeaponManagerComponent::GetMMAbilitySystemComponent()
{
	if (AMMCharacter* Pawn = GetPawn<AMMCharacter>())
	{
		return Pawn->GetMMAbilitySystemComponent();
	}
	return nullptr;
}

void UWeaponManagerComponent::AttachActiveWeapon(USceneComponent* Mesh, bool bShouldBeEquipped)
{
	if (SpawnedWeapons.IsValidIndex(ActiveWeaponIndex))
	{
		const FWeaponEntry& ActEntry = SpawnedWeapons[ActiveWeaponIndex];
		UWeaponInstance* ActWeaponInstance = ActEntry.WeaponInstance;
		UWeaponDefinition* ActWeaponDefinition = ActEntry.WeaponDefinition;
		if (IsValid(ActWeaponInstance) && IsValid(ActWeaponDefinition))
		{
			ActWeaponInstance->AttachActorWeapons(Mesh, ActWeaponDefinition->ActorsToSpawn, bShouldBeEquipped);
		}
	}
}

bool UWeaponManagerComponent::ChangeActiveWeapon(const int32 NumberWeaponToSwitch)
{
	if (NumberWeaponToSwitch != ActiveWeaponIndex)
	{
		if (SpawnedWeapons.IsValidIndex(NumberWeaponToSwitch))
		{
			UWeaponInstance* WI = nullptr;
			if (SpawnedWeapons.IsValidIndex(ActiveWeaponIndex))
			{
				if (bIsWeaponEquipped)
				{
					return false;
				}
				else
				{
					WI = SpawnedWeapons[ActiveWeaponIndex].WeaponInstance;
					if (IsValid(WI))
					{
						WI->ChangeVisibilitySpawnedActorWeapons(false);
					}
				}
			}
			WI = SpawnedWeapons[NumberWeaponToSwitch].WeaponInstance;
			if (IsValid(WI))
			{
				WI->ChangeVisibilitySpawnedActorWeapons(true);
			}
			OnActiveWeaponChanged.ExecuteIfBound(ActiveWeaponIndex, NumberWeaponToSwitch);
			ActiveWeaponIndex = NumberWeaponToSwitch;
		}
	}
	return true;
}