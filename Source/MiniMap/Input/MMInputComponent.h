// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "MMInputConfig.h"
#include "MMInputComponent.generated.h"


UCLASS()
class MINIMAP_API UMMInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	template<typename UserClass, typename FuncType>
	void BindNativeAction(const UMMInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func);

	template<typename UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UMMInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);

	void RemoveBinds(TArray<uint32>& BindHandles);
};

template<typename UserClass, typename FuncType>
inline void UMMInputComponent::BindNativeAction(const UMMInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
{
	if (IsValid(InputConfig))
	{
		if (const UInputAction* InputAction = InputConfig->FindNativeInputActionForTag(InputTag))
		{
			BindAction(InputAction, TriggerEvent, Object, Func);
		}
	}
}

template<typename UserClass, typename PressedFuncType, typename ReleasedFuncType>
inline void UMMInputComponent::BindAbilityActions(const UMMInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	if (IsValid(InputConfig))
	{
		for (const FMMInputAction& AbilityAction : InputConfig->GetAbillityInputActions())
		{
			if (IsValid(AbilityAction.InputAction) && AbilityAction.InputTag.IsValid())
			{
				if (PressedFunc)
				{
					BindHandles.Add(BindAction(AbilityAction.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, AbilityAction.InputTag).GetHandle());
				}
				if (ReleasedFunc)
				{
					BindHandles.Add(BindAction(AbilityAction.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, AbilityAction.InputTag).GetHandle());
				}
			}
		}
	}
}
