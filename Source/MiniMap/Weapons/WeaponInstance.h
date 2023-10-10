// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponInstance.generated.h"

class ABaseWeapon;
struct FWeaponToSpawn;
class UMMAnimLayerSelectionEntry;
class UMMAnimMontageSelectionEntry;
class UMMAtackMontageSelectionEntry;
class UAnimInstance;
class UAnimMontage;
class UWeaponManagerComponent;
class USceneComponent;
class UTexture2D;

UCLASS(BlueprintType, Blueprintable)
class MINIMAP_API UWeaponInstance : public UObject
{
	GENERATED_BODY()

public:

	virtual UWorld* GetWorld() const override final;

	virtual void BeginDestroy() override;

	FORCEINLINE UObject* GetInstigator() const { return Instigator; }

	FORCEINLINE void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }

	template <typename PawnClass>
	PawnClass* GetPawn() const { return Cast<PawnClass>(GetOuter()); }

	FORCEINLINE const TArray<TObjectPtr<ABaseWeapon>>& GetSpawnedActorWeapons() const { return SpawnedActorWeapons; }

	TSubclassOf<UAnimInstance> GetAnimInstanceClassForEquipping(bool bShouldBeEquipped);

	UAnimMontage* GetAnimMontageForEquipping(bool bShouldBeEquipped);

	void ChangeVisibilitySpawnedActorWeapons(bool bShouldBeVisible);

	void OnEquip();

	void OnEquipWithAttaching(USceneComponent* AttachTarget, const TArray<FWeaponToSpawn>& WeaponProperties);

	void OnUnequip();

	void OnUnequipWithAttaching(USceneComponent* AttachTarget, const TArray<FWeaponToSpawn>& WeaponProperties);

	void AttachActorWeapons(USceneComponent* AttachTarget, const TArray<FWeaponToSpawn>& WeaponProperties, bool bShouldBeEquipped);

	UAnimMontage* GetAnimMontageForAttacking(int32 AbilityLevel);

	FORCEINLINE const TSoftObjectPtr<UTexture2D>& GetWeaponHUDTexture() const { return WeaponHUDTexture; }

protected:

	void SpawnEquipmentActorWeapons(const TArray<FWeaponToSpawn>& WeaponsToSpawn);

	void DestroyEquipmentActorWeapons();

	void LinkAnimLayers(const TSubclassOf<UAnimInstance> AnimLayersToLink);

	UPROPERTY(EditDefaultsOnly, noClear, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> WeaponHUDTexture = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UObject> Instigator = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<TObjectPtr<ABaseWeapon>> SpawnedActorWeapons;

	UPROPERTY(EditDefaultsOnly, noClear, BlueprintReadOnly)
	TObjectPtr<UMMAtackMontageSelectionEntry> AttackMontagesSet;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, noClear)
	TObjectPtr<UMMAnimLayerSelectionEntry> AnimationsEquippingSet = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, noClear)
	TObjectPtr<UMMAnimMontageSelectionEntry> MontagesEquippingSet = nullptr;

	friend UWeaponManagerComponent;
};