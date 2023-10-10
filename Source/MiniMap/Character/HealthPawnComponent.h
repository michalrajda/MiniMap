// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "HealthPawnComponent.generated.h"

class UMMAbilitySystemComponent;

UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class MINIMAP_API UHealthPawnComponent : public UPawnComponent
{
	GENERATED_BODY()

public:

	void InitializeWithAbilitySystemComponent(UMMAbilitySystemComponent* MMASC);

	void UninitializeFromAbilitySystem(UMMAbilitySystemComponent* MMASC);
};
