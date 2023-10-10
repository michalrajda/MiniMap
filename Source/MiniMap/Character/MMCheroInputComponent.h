// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "MMCheroInputComponent.generated.h"

struct FGameplayTag;
struct FInputActionValue;
class UInputMappingContext;
class UMMInputConfig;

UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class MINIMAP_API UMMCheroInputComponent : public UPawnComponent
{
	GENERATED_BODY()
	
public:

	void InitializePlayerInput(UInputComponent* PlayerInputComponent);

	FORCEINLINE void SetTimerForMove()
	{
		if (UWorld* World = GetWorld())
		{
			FTimerManager& TM = World->GetTimerManager();
			TM.SetTimer(TimerHandleForMove, this, &UMMCheroInputComponent::NotivyPlayerEndMowing, TimerRatio, true);
			TM.PauseTimer(TimerHandleForMove);
		}
	}

	FORCEINLINE void ClearTimerForMove()
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(TimerHandleForMove);
		}
	}

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);

	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	void Input_StartMove();

	void Input_Move(const FInputActionValue& InputActionValue);

	void Input_EndMove();

	void Input_LookMouse(const FInputActionValue& InputActionValue);

	void Input_SwapWeapon(const FInputActionValue& InputActionValue);

	void Input_Crouch();

	void NotivyPlayerEndMowing();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, noClear, Category = Input)
	UInputMappingContext* DefaultMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, noClear, Category = Input)
	UMMInputConfig* DefaultInputConfig = nullptr;

	FTimerHandle TimerHandleForMove;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float TimerRatio = 0.05f;
};
