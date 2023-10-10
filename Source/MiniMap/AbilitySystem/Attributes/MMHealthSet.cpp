// Fill out your copyright notice in the Description page of Project Settings.

#include "MMHealthSet.h"
#include "GameplayEffectExtension.h"
#include "NativeTags/Tags.h"
#include "AbilitySystem/MMAbilitySystemComponent.h"


UMMHealthSet::UMMHealthSet()
{
	Health = 100.0f;
	MaxHealth = 100.0f;
	DamageReduction = 0.0f;
	Damage = 0.0f;
}

bool UMMHealthSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f && GetDamageReduction() == 1.0f)
		{
			if (Data.Target.HasMatchingGameplayTag(MMGameplayTags::TAG_Gameplay_DamageImmunity))
			{
				// Do not take away any health.
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}
		}
	}
	return true;
}

void UMMHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), 0.0f, GetMaxHealth()));
		SetDamage(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
}

void UMMHealthSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	ClampAttribute(Attribute, NewValue);
}

void UMMHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	ClampAttribute(Attribute, NewValue);
}

void UMMHealthSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	if (Attribute == GetMaxHealthAttribute())
	{
		if (UMMAbilitySystemComponent* MMASC = GetMMAbilitySystemComponent())
		{
			//const float NewCurrentHealth = (OldValue > 0.0f) ? GetHealth() * NewValue / OldValue : NewValue;
			MMASC->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
			//SetHealth(FMath::Clamp(NewCurrentHealth, 0, GetMaxHealth()));
		}
	}
}

void UMMHealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		// Do not allow health to go negative or above max health.
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// Do not allow max health to drop below 1.
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	else if (Attribute == GetDamageReductionAttribute())
	{
		//Do not allow damage reduction drop below 0% and go above 100%
		NewValue = FMath::Clamp(NewValue, 0.0f, 1.0f);
	}
}

void UMMHealthSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	if (UMMAbilitySystemComponent* MMASC = GetMMAbilitySystemComponent())
	{
		const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
		if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue))
		{
			// Change current value to maintain the current Val / Max percent
			const float CurrentValue = AffectedAttribute.GetCurrentValue();
			float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;
			MMASC->ApplyModToAttribute(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
		}
	}
}
