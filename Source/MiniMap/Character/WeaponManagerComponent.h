// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "AbilitySystem/MMAbilitySet.h"
#include "WeaponManagerComponent.generated.h"

class UWeaponDefinition;
class UWeaponInstance;
class UMMAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWeaponEntry
{
	GENERATED_BODY()

public:

	FWeaponEntry(UWeaponDefinition* InWeaponDefinition = nullptr, UWeaponInstance* InWeaponInstance = nullptr);

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UWeaponDefinition> WeaponDefinition = nullptr;

	UPROPERTY(BlueprintreadOnly, Transient)
	TObjectPtr<UWeaponInstance> WeaponInstance = nullptr;

	UPROPERTY(BlueprintreadOnly)
	FMMAbilitySet_GrantedHandles GrantedHandles;
};

DECLARE_DELEGATE_TwoParams(FOnActiveWeaponChangedSignature, int32/*PrevWeaponIndex*/, int32/*ActWeaponIndex*/)
DECLARE_DELEGATE_OneParam(FOnChangedEquippingStateSignature,bool)

UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class MINIMAP_API UWeaponManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:

	static UWeaponManagerComponent* FindWeaponManagerComponent(const AActor* Actor) { return IsValid(Actor) ? Actor->FindComponentByClass<UWeaponManagerComponent>() : nullptr; }

	void AddWeapons(const TArray<TSubclassOf<UWeaponDefinition>>& WeaponsDefinitionsToAdd);

	void AddWeapon(const TSubclassOf<UWeaponDefinition> WeaponDefinitionToAdd);

	void RemoveWeapon(const int32 NumberWeaponToRemove);

	void UninitializeAndRemoveAllWeapons();

	bool ChangeActiveWeapon(const int32 NumberWeaponToSwitch);

	void OnWeaponEquipped(bool bWasSuccessfully = true);

	void OnWeaponUnequipped(bool bWasSuccessfully = true);

	UMMAbilitySystemComponent* GetMMAbilitySystemComponent();

	void AttachActiveWeapon(USceneComponent* Mesh, bool bShouldBeEquipped);

	FORCEINLINE UWeaponInstance* GetActiveWeaponInstance() const { return SpawnedWeapons.IsValidIndex(ActiveWeaponIndex) ? SpawnedWeapons[ActiveWeaponIndex].WeaponInstance : nullptr; }

	FORCEINLINE bool IsWeaponEuipped() const { return bIsWeaponEquipped && SpawnedWeapons.IsValidIndex(ActiveWeaponIndex); }

	FORCEINLINE int32 GetActiveWeaponIndex() const { return ActiveWeaponIndex; }

	FORCEINLINE const TArray<FWeaponEntry>& GetSpawnedWeapons()const { return SpawnedWeapons; }

	FORCEINLINE void UnbindDelegates()
	{
		OnActiveWeaponChanged.Unbind();
		OnChangedEquippingState.Unbind();
	}

	FOnActiveWeaponChangedSignature OnActiveWeaponChanged;

	FOnChangedEquippingStateSignature OnChangedEquippingState;

protected:

	bool bIsWeaponEquipped = false;

	int32 ActiveWeaponIndex = -1;
	
	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<FWeaponEntry> SpawnedWeapons;
};
