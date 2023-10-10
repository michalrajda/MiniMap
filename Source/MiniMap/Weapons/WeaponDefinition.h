// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponDefinition.generated.h"

class ABaseWeapon;
class UWeaponInstance;
class UMMAbilitySet;

USTRUCT(BlueprintType)
struct FWeaponToSpawn
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<ABaseWeapon> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category = Equipment)
	FName UnequippedAttachSocket;

	UPROPERTY(EditAnywhere, Category = Equipment)
	FName EquippedAttachSocket;

	UPROPERTY(EditAnywhere, Category = Equipment)
	FTransform UnequippedAttachTransform;

	UPROPERTY(EditAnywhere, Category = Equipment)
	FTransform EquippedAttachTransform;
};

UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class MINIMAP_API UWeaponDefinition : public UObject
{
	GENERATED_BODY()

public:

	UWeaponDefinition();

	UPROPERTY(EditDefaultsOnly, noClear, Category = Equipment)
	TSubclassOf<UWeaponInstance> InstanceType = nullptr;

	UPROPERTY(EditDefaultsOnly, noClear, Category = Equipment)
	TArray<TObjectPtr<const UMMAbilitySet>> AbilitySetsToGrant;

	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TArray<FWeaponToSpawn> ActorsToSpawn;
};
