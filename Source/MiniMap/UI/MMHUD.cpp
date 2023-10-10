// Fill out your copyright notice in the Description page of Project Settings.


#include "MMHUD.h"
#include "MiniMapWidget.h"
#include "Components/PanelWidget.h"
#include "InvetoryWidget.h"

void AMMHUD::CreateMenu()
{
	if (IsValid(UserInterfaceClass) && IsValid(PlayerOwner))
	{
		if (UserInterface = CreateWidget<UUserWidget>(PlayerOwner, UserInterfaceClass))
		{
			UserInterface->AddToViewport();
			PlayerOwner->SetInputMode(FInputModeGameOnly());
			if (UWorld* const World = GetWorld())
			{
				if (UGameViewportClient* const Viewport = World->GetGameViewport())
				{
					Viewport->SetMouseCaptureMode(EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
					Viewport->SetMouseLockMode(EMouseLockMode::LockAlways);
				}
			}
			PlayerOwner->bShowMouseCursor = false;
		}
	}
}

void AMMHUD::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(UserInterface))
	{
		UserInterface->RemoveFromParent();
		UserInterface = nullptr;
		MiniMapWidget = nullptr;
		InventoryWidget = nullptr;
	}
	Super::EndPlay(EndPlayReason);
}

void AMMHUD::SetMiniMapWidget(UMiniMapWidget* InMiniMapWidget)
{
	if (MiniMapWidget != InMiniMapWidget && IsValid(InMiniMapWidget))
	{
		MiniMapWidget = InMiniMapWidget;
	}
}

void AMMHUD::InitializeMiniMapWidget(AMMPlayerCharacter* InPawn)
{
	if (IsValid(MiniMapWidget))
	{
		MiniMapWidget->InitializeMiniMapWidgetFromPawn(InPawn);
	}
}

void AMMHUD::SetInventoryWidget(UInventoryWidget* InInventoryWidget)
{
	if (InventoryWidget != InInventoryWidget && IsValid(InInventoryWidget))
	{
		InventoryWidget = InInventoryWidget;
	}
}

void AMMHUD::InitializeInventoryWidget(UWeaponManagerComponent* InWeaponManager)
{
	if (IsValid(InventoryWidget))
	{
		InventoryWidget->InitializeInvenotrySlotsFromWeaponManager(InWeaponManager);
	}
}
