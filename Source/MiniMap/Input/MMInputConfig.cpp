// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/MMInputConfig.h"

const UInputAction* UMMInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FMMInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
    return nullptr;
}

const UInputAction* UMMInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FMMInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}
    return nullptr;
}
