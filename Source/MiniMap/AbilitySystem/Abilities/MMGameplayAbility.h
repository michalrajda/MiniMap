// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MMGameplayAbility.generated.h"

class UMMAbilitySystemComponent;
class AMMCharacter;

UCLASS()
class MINIMAP_API UMMGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	FORCEINLINE UMMAbilitySystemComponent* GetMMAbilitySystemComponentFromActorInfo() const
	{
		return (CurrentActorInfo ? Cast<UMMAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr);
	}

	AMMCharacter* GetMMCharacterFromActorInfo() const
	{
		return (CurrentActorInfo ? Cast<AMMCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
	}

	FORCEINLINE bool ShouldAutoActivate() const { return bAutoActivate; }

protected:
	
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	bool bAutoActivate = false;
};
