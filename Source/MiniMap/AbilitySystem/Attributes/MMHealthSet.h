// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMAttributeSet.h"
#include "MMHealthSet.generated.h"

struct FGameplayEffectModCallbackData;

UCLASS()
class MINIMAP_API UMMHealthSet : public UMMAttributeSet
{
	GENERATED_BODY()

public:

	UMMHealthSet();

	ATTRIBUTE_ACCESSORS(UMMHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UMMHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UMMHealthSet, DamageReduction);
	ATTRIBUTE_ACCESSORS(UMMHealthSet, Damage);

protected:

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

	UPROPERTY(BlueprintReadOnly, Category = "MiniMap|Health")
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "MiniMap|Health")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "MiniMap|Health")
	FGameplayAttributeData DamageReduction;
	
	UPROPERTY(BlueprintReadOnly, Category = "MiniMap|Health")
	FGameplayAttributeData Damage;
};
