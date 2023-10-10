// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MMAnimLayerSelectionEntry.generated.h"

class UAnimInstance;
class UAnimMontage;

UCLASS()
class MINIMAP_API UMMAnimLayerSelectionEntry : public UDataAsset
{
	GENERATED_BODY()

public:

	FORCEINLINE const TSubclassOf<UAnimInstance> GetAnimLayer(bool bShouldBeEquppied) const { return bShouldBeEquppied ? EquippedAnimLayer : UnequippedAnimLayer; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, noClear)
	TSubclassOf<UAnimInstance> EquippedAnimLayer = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, noClear)
	TSubclassOf<UAnimInstance> UnequippedAnimLayer = nullptr;
};

UCLASS()
class MINIMAP_API UMMAnimMontageSelectionEntry : public UDataAsset
{
	GENERATED_BODY()

public:

	FORCEINLINE UAnimMontage* GetAnimMontage(bool bShouldBeEquppied) const { return bShouldBeEquppied ? EquippedAnimMontage : UnequippedAnimMontage; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, noClear)
	TObjectPtr<UAnimMontage> EquippedAnimMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, noClear)
	TObjectPtr<UAnimMontage> UnequippedAnimMontage = nullptr;
};

UCLASS()
class MINIMAP_API UMMAtackMontageSelectionEntry : public UDataAsset
{
	GENERATED_BODY()

public:

	FORCEINLINE UAnimMontage* GetAnimMontage(int32 AbilityLevel)
	{
		if (TObjectPtr<UAnimMontage>* AttackM = AttackMontages.Find(AbilityLevel))
		{
			return *AttackM;
		}
		return nullptr;
	}
protected:

	UPROPERTY(EditDefaultsOnly, noClear, BlueprintReadOnly)
	TMap<int32, TObjectPtr<UAnimMontage>> AttackMontages;
};

