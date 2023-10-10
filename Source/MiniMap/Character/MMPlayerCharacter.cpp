// Fill out your copyright notice in the Description page of Project Settings.


#include "MMPlayerCharacter.h"
#include "MMCheroInputComponent.h"
#include "Camera/CameraComponent.h"
#include "UI/MMHUD.h"
#include "GameFramework/SpringArmComponent.h"

AMMPlayerCharacter::AMMPlayerCharacter()
{
	CheroInputComponent = CreateDefaultSubobject<UMMCheroInputComponent>(TEXT("CheroInputComponent"));
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;	
	CameraBoom->bUsePawnControlRotation = true;
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AMMPlayerCharacter::OnPlayerMove()
{
	if (OnCharacterMoveDelegate.IsBound())
	{
		FVector PlayerLocation = GetActorLocation();
		OnCharacterMoveDelegate.Execute(FVector2D(PlayerLocation.X, PlayerLocation.Y), GetActorRotation().Yaw);
	}
}

void AMMPlayerCharacter::OnPlayerControllerRotate()
{
	if (OnControllerRotateDelegate.IsBound())
	{
		OnControllerRotateDelegate.Execute(GetControlRotation().Yaw);
	}
}

void AMMPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (IsValid(CheroInputComponent))
	{
		CheroInputComponent->InitializePlayerInput(PlayerInputComponent);
	}
}

void AMMPlayerCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(CheroInputComponent))
	{
		CheroInputComponent->ClearTimerForMove();
	}
	OnControllerRotateDelegate.Unbind();
	OnCharacterMoveDelegate.Unbind();
	Super::EndPlay(EndPlayReason);
}

void AMMPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UnbindDelegates();
	if (APlayerController* PC = Cast<APlayerController>(NewController))
	{
		if (AMMHUD* HUD = PC->GetHUD<AMMHUD>())
		{
			if (IsValid(CheroInputComponent))
			{
				CheroInputComponent->SetTimerForMove();
			}
			HUD->InitializeInventoryWidget(WeaponManager);
			HUD->InitializeMiniMapWidget(this);
		}
	}
}

void AMMPlayerCharacter::UnPossessed()
{
	UnbindDelegates();
	if (IsValid(CheroInputComponent))
	{
		CheroInputComponent->ClearTimerForMove();
	}
	Super::UnPossessed();
}
