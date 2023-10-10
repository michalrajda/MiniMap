// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MMPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MINIMAP_API AMMPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void ClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass) override;

protected:

	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
};
