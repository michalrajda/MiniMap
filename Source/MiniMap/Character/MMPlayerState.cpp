// Fill out your copyright notice in the Description page of Project Settings.


#include "MMPlayerState.h"
#include "AbilitySystem/MMAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/MMHealthSet.h"
#include "AbilitySystem/Attributes/MMCombatSet.h"

AMMPlayerState::AMMPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UMMAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	CreateDefaultSubobject<UMMHealthSet>(TEXT("HealthSet"));
	CreateDefaultSubobject<UMMCombatSet>(TEXT("CombatSet"));
}

UAbilitySystemComponent* AMMPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMMPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());
	}
}
