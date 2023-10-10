// Fill out your copyright notice in the Description page of Project Settings.

#include "MMAttributeSet.h"
#include "AbilitySystem/MMAbilitySystemComponent.h"

UMMAbilitySystemComponent* UMMAttributeSet::GetMMAbilitySystemComponent() const
{
    return Cast<UMMAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
