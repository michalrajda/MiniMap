// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMAttributeSet.h"
#include "MMCombatSet.generated.h"


UCLASS()
class MINIMAP_API UMMCombatSet : public UMMAttributeSet
{
	GENERATED_BODY()

public:

	UMMCombatSet();

	ATTRIBUTE_ACCESSORS(UMMCombatSet, BaseDamage);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "MiniMap|Combat")
	FGameplayAttributeData BaseDamage;
};
