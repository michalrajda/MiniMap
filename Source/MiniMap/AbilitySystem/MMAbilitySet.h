// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MMAbilitySet.generated.h"

class UMMGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
struct FGameplayAbilitySpecHandle;
struct FActiveGameplayEffectHandle;
class UMMAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FMMAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMMGameplayAbility> Ability = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};


USTRUCT(BlueprintType)
struct FMMAbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float EffectLevel = 1.0f;
};


USTRUCT(BlueprintType)
struct FMMAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);

	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);

	void AddAttributeSet(UAttributeSet* Set);

	void TakeFromAbilitySystem(UMMAbilitySystemComponent* MMASC);

	FORCEINLINE bool IsGrantedToAbilitySystem() const { return !GrantedAbilitySpecHandles.IsEmpty() || !GrantedGameplayEffectHandles.IsEmpty() || !GrantedGrantedAttributeSets.IsEmpty(); }

protected:

	UPROPERTY(BlueprintreadOnly, Transient)
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;

	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<FActiveGameplayEffectHandle> GrantedGameplayEffectHandles;

	UPROPERTY(BlueprintReadOnly, Transient)
	TArray<TObjectPtr<UAttributeSet>> GrantedGrantedAttributeSets;
};

UCLASS()
class MINIMAP_API UMMAbilitySet : public UDataAsset
{
	GENERATED_BODY()

public:

	void GiveToAbilitySystem(UMMAbilitySystemComponent* MMASC, FMMAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject = nullptr) const;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty = Ability))
	TArray<FMMAbilitySet_GameplayAbility> GameplayAbilitiesToGrant;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta = (TitleProperty = GameplayEffect))
	TArray<FMMAbilitySet_GameplayEffect> GameplayEffectsToGrant;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta = (TitleProperty = AttributeSet))
	TArray<TSubclassOf<UAttributeSet>> AttributesToGrant;
};
