// Fill out your copyright notice in the Description page of Project Settings.


#include "MMPlayerController.h"
#include "AbilitySystem/MMAbilitySystemComponent.h"
#include "MMPlayerState.h"
#include "UI/MMHUD.h"

void AMMPlayerController::ClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass)
{
	if (MyHUD != nullptr)
	{
		MyHUD->Destroy();
		MyHUD = nullptr;
	}
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	MyHUD = GetWorld()->SpawnActor<AHUD>(NewHUDClass, SpawnInfo);
	if (AMMHUD* NewHUD = Cast<AMMHUD>(MyHUD))
	{
		NewHUD->CreateMenu();
	}
}

void AMMPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (AMMPlayerState* PS = Cast<AMMPlayerState>(PlayerState))
	{
		if (UMMAbilitySystemComponent* MMASC = PS->GetMMAbilitySystemComponent())
		{
			MMASC->ProcessAbilityInput(DeltaTime, bGamePaused);
		}
	}
}
