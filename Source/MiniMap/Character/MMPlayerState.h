// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "MMPlayerState.generated.h"

class UMMAbilitySystemComponent;

UCLASS()
class MINIMAP_API AMMPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AMMPlayerState();

	FORCEINLINE UMMAbilitySystemComponent* GetMMAbilitySystemComponent() const { return AbilitySystemComponent; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PostInitializeComponents() override;

protected:

	UPROPERTY(VisibleAnywhere, Category = "MiniMap|PlayerState")
	TObjectPtr<UMMAbilitySystemComponent> AbilitySystemComponent = nullptr;
};
