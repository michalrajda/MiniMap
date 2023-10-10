// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPawnDataComponent.h"

void UDefaultPawnDataComponent::InitializeWithAbilitySystemComponent(UMMAbilitySystemComponent* MMASC, UObject* SourceObject)
{
	for (const TObjectPtr<const UMMAbilitySet>& Ability : AbilitySetsToGrant)
	{
		if (IsValid(Ability))
		{
			Ability->GiveToAbilitySystem(MMASC, &GrantedAbilities, SourceObject);
		}
	}
}

void UDefaultPawnDataComponent::UninitializeFromAbilitySystem(UMMAbilitySystemComponent* MMASC)
{
	GrantedAbilities.TakeFromAbilitySystem(MMASC);
}
