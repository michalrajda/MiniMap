// Fill out your copyright notice in the Description page of Project Settings.


#include "MMAbilitySet.h"
#include "MMAbilitySystemComponent.h"
#include "Abilities/MMGameplayAbility.h"

void FMMAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		GrantedAbilitySpecHandles.Add(Handle);
	}
}

void FMMAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GrantedGameplayEffectHandles.Add(Handle);
	}
}

void FMMAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	if (IsValid(Set))
	{
		GrantedGrantedAttributeSets.Add(Set);
	}
}

void FMMAbilitySet_GrantedHandles::TakeFromAbilitySystem(UMMAbilitySystemComponent* MMASC)
{
	if (IsValid(MMASC))
	{
		for (const FGameplayAbilitySpecHandle& Handle : GrantedAbilitySpecHandles)
		{
			if (Handle.IsValid())
			{
				MMASC->ClearAbility(Handle);
			}
		}
		for (const FActiveGameplayEffectHandle& Handle : GrantedGameplayEffectHandles)
		{
			if (Handle.IsValid())
			{
				MMASC->RemoveActiveGameplayEffect(Handle);
			}
		}
		for (UAttributeSet* Set : GrantedGrantedAttributeSets)
		{
			MMASC->RemoveSpawnedAttribute(Set);
		}
	}
	GrantedAbilitySpecHandles.Reset();
	GrantedGameplayEffectHandles.Reset();
	GrantedGrantedAttributeSets.Reset();
}

void UMMAbilitySet::GiveToAbilitySystem(UMMAbilitySystemComponent* MMASC, FMMAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	if (IsValid(MMASC))
	{
		for (int32 AbilityIndex = 0; AbilityIndex < GameplayAbilitiesToGrant.Num(); ++AbilityIndex)
		{
			const FMMAbilitySet_GameplayAbility& AbilityToGrant = GameplayAbilitiesToGrant[AbilityIndex];
			if (IsValid(AbilityToGrant.Ability))
			{
				UMMGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UMMGameplayAbility>();
				FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
				AbilitySpec.SourceObject = SourceObject;
				AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);
				const FGameplayAbilitySpecHandle AbilitySpecHandle = MMASC->GiveAbility(AbilitySpec);
				if (AbilityCDO->ShouldAutoActivate())
				{
					MMASC->TryActivateAbility(AbilitySpecHandle);
				}
				if (OutGrantedHandles)
				{
					OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
				}
			}
		}
		for (int32 EffectIndex = 0; EffectIndex < GameplayEffectsToGrant.Num(); ++EffectIndex)
		{
			const FMMAbilitySet_GameplayEffect& EffectToGrant = GameplayEffectsToGrant[EffectIndex];
			if (IsValid(EffectToGrant.GameplayEffect))
			{
				const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
				const FActiveGameplayEffectHandle GameplayEffectHandle = MMASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, MMASC->MakeEffectContext());
				if (OutGrantedHandles)
				{
					OutGrantedHandles->AddGameplayEffectHandle(GameplayEffectHandle);
				}
			}
		}
		for (int32 SetIndex = 0; SetIndex < AttributesToGrant.Num(); ++SetIndex)
		{
			const TSubclassOf<UAttributeSet>& SetToGrant = AttributesToGrant[SetIndex];
			if (IsValid(SetToGrant))
			{
				UAttributeSet* NewSet = NewObject<UAttributeSet>(MMASC->GetOwner(), SetToGrant);
				MMASC->AddAttributeSetSubobject(NewSet);
				if (OutGrantedHandles)
				{
					OutGrantedHandles->AddAttributeSet(NewSet);
				}
			}
		}
	}
}
