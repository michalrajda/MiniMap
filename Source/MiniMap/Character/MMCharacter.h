// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "MMCharacter.generated.h"

class UMMAbilitySystemComponent;
class UHealthPawnComponent;
class UWeaponManagerComponent;
class UWeaponDefinition;
class UDefaultPawnDataComponent;

UCLASS()
class MINIMAP_API AMMCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	
	AMMCharacter();

	virtual void PossessedBy(AController* NewController) override;

	virtual void UnPossessed() override;

	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	void ToogleCrouch();

	FORCEINLINE UMMAbilitySystemComponent* GetMMAbilitySystemComponent() const { return AbilitySystemComponent; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:

	void InitializeWithAbilitySystem();

	void UninitializeFromAbilitySystem();

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UMMAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Health)
	TObjectPtr<UHealthPawnComponent> HealthComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Equipment)
	TObjectPtr<UWeaponManagerComponent> WeaponManager = nullptr;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = DefaultData)
	TObjectPtr<UDefaultPawnDataComponent> PawnData = nullptr;
};
