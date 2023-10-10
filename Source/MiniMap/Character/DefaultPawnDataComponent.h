// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "AbilitySystem/MMAbilitySet.h"
#include "DefaultPawnDataComponent.generated.h"

class UWeaponDefinition;

UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class MINIMAP_API UDefaultPawnDataComponent : public UPawnComponent
{
	GENERATED_BODY()

public:

	void InitializeWithAbilitySystemComponent(UMMAbilitySystemComponent* MMASC, UObject* SourceObject = nullptr);

	void UninitializeFromAbilitySystem(UMMAbilitySystemComponent* MMASC);

	FORCEINLINE const TArray<TSubclassOf<UWeaponDefinition>>& GetWeaponsToSpawn() const { return Weapons; }

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, noClear, Category = Equippent)
	TArray<TSubclassOf<UWeaponDefinition>> Weapons;

	UPROPERTY(BlueprintReadOnly, Category = Abilities)
	FMMAbilitySet_GrantedHandles GrantedAbilities;

	UPROPERTY(EditDefaultsOnly, noClear, Category = Abilities)
	TArray<TObjectPtr<const UMMAbilitySet>> AbilitySetsToGrant;
};
