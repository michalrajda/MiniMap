// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MMHUD.generated.h"

class UUserWidget;
class UMiniMapWidget;
class UInventoryWidget;
class UWeaponManagerComponent;
class AMMPlayerCharacter;

UCLASS()
class MINIMAP_API AMMHUD : public AHUD
{
	GENERATED_BODY()

public:

	void SetMiniMapWidget(UMiniMapWidget* InMiniMapWidget);

	void InitializeMiniMapWidget(AMMPlayerCharacter* InPawn);

	void SetInventoryWidget(UInventoryWidget* InInventoryWidget);

	void InitializeInventoryWidget(UWeaponManagerComponent* InWeaponManager);

	void CreateMenu();

protected:

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, noClear)
	TSubclassOf<UUserWidget> UserInterfaceClass = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UUserWidget> UserInterface = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UMiniMapWidget> MiniMapWidget = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient)
	TObjectPtr<UInventoryWidget> InventoryWidget = nullptr;

	bool bNeedInitializeMiniMapWidget = false;
};
