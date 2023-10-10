// Fill out your copyright notice in the Description page of Project Settings.


#include "MMCheroInputComponent.h"
#include "MMPlayerCharacter.h"
#include "Input/MMInputComponent.h"
#include "AbilitySystem/MMAbilitySystemComponent.h"
#include "InputMappingContext.h"
#include "NativeTags/Tags.h"
#include "EnhancedInputSubsystems.h"

void UMMCheroInputComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	if (IsValid(DefaultMappingContext) && IsValid(DefaultInputConfig))
	{
		if (UMMInputComponent* MMInputComponent = Cast<UMMInputComponent>(PlayerInputComponent))
		{
			if (const APlayerController* PC = GetController<APlayerController>())
			{
				if (const ULocalPlayer* LP = PC->GetLocalPlayer())
				{
					if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
					{
						Subsystem->ClearAllMappings();
						Subsystem->AddMappingContext(DefaultMappingContext, 0);
						MMInputComponent->BindNativeAction(DefaultInputConfig, MMGameplayTags::InputTag_Move, ETriggerEvent::Started, this, &UMMCheroInputComponent::Input_StartMove);
						MMInputComponent->BindNativeAction(DefaultInputConfig, MMGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &UMMCheroInputComponent::Input_Move);
						MMInputComponent->BindNativeAction(DefaultInputConfig, MMGameplayTags::InputTag_Move, ETriggerEvent::Completed, this, &UMMCheroInputComponent::Input_EndMove);
						MMInputComponent->BindNativeAction(DefaultInputConfig, MMGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &UMMCheroInputComponent::Input_LookMouse);
						MMInputComponent->BindNativeAction(DefaultInputConfig, MMGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &UMMCheroInputComponent::Input_Crouch);
						MMInputComponent->BindNativeAction(DefaultInputConfig, MMGameplayTags::InputTag_SwapWeapon, ETriggerEvent::Triggered, this, &UMMCheroInputComponent::Input_SwapWeapon);
						TArray<uint32> BindHandles;
						MMInputComponent->BindAbilityActions(DefaultInputConfig, this, &UMMCheroInputComponent::Input_AbilityInputTagPressed, &UMMCheroInputComponent::Input_AbilityInputTagReleased, BindHandles);
					}
				}
			}
		}
	}
}

void UMMCheroInputComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ClearTimerForMove();
	Super::EndPlay(EndPlayReason);
}

void UMMCheroInputComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (AMMPlayerCharacter* Pawn = GetPawn<AMMPlayerCharacter>())
	{
		UMMAbilitySystemComponent* MMASC = Pawn->GetMMAbilitySystemComponent();
		if (IsValid(MMASC))
		{
			MMASC->AbilityInputTagPressed(InputTag);
		}
	}
}

void UMMCheroInputComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (AMMPlayerCharacter* Pawn = GetPawn<AMMPlayerCharacter>())
	{
		UMMAbilitySystemComponent* MMASC = Pawn->GetMMAbilitySystemComponent();
		if (IsValid(MMASC))
		{
			MMASC->AbilityInputTagReleased(InputTag);
		}
	}
}

void UMMCheroInputComponent::Input_StartMove()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().PauseTimer(TimerHandleForMove);
	}
}

void UMMCheroInputComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	if (AMMPlayerCharacter* Pawn = GetPawn<AMMPlayerCharacter>())
	{
		if (AController* Controller = Pawn->GetController())
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			Pawn->AddMovementInput(ForwardDirection, MovementVector.Y);
			Pawn->AddMovementInput(RightDirection, MovementVector.X);
			Pawn->OnPlayerMove();
		}
	}
}

void UMMCheroInputComponent::Input_EndMove()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().UnPauseTimer(TimerHandleForMove);
	}
}

void UMMCheroInputComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	if (AMMPlayerCharacter* Pawn = GetPawn<AMMPlayerCharacter>())
	{
		Pawn->AddControllerYawInput(LookAxisVector.X);
		Pawn->AddControllerPitchInput(LookAxisVector.Y);
		Pawn->OnPlayerControllerRotate();
	}
}

void UMMCheroInputComponent::Input_SwapWeapon(const FInputActionValue& InputActionValue)
{
	if (AMMPlayerCharacter* Pawn = GetPawn<AMMPlayerCharacter>())
	{
		UMMAbilitySystemComponent* MMASC = Pawn->GetMMAbilitySystemComponent();
		if (IsValid(MMASC))
		{
			FGameplayEventData NewEventData;
			NewEventData.EventTag = MMGameplayTags::GameplayEvent_SwapWeapon;
			NewEventData.EventMagnitude = InputActionValue.Get<float>();
			FScopedPredictionWindow NewScopedWindow(MMASC, true);
			MMASC->HandleGameplayEvent(MMGameplayTags::GameplayEvent_SwapWeapon, &NewEventData);
		}
	}
}

void UMMCheroInputComponent::Input_Crouch()
{
	if (AMMPlayerCharacter* Pawn = GetPawn<AMMPlayerCharacter>())
	{
		Pawn->ToogleCrouch();
	}
}

void UMMCheroInputComponent::NotivyPlayerEndMowing()
{
	if (AMMPlayerCharacter* Pawn = GetPawn<AMMPlayerCharacter>())
	{
		Pawn->OnPlayerMove();
		if (FMath::IsNearlyZero(Pawn->GetVelocity().Size2D()))
		{
			if (UWorld* World = GetWorld())
			{
				World->GetTimerManager().PauseTimer(TimerHandleForMove);
			}
		}
	}
}
