// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMCharacter.h"
#include "MMPlayerCharacter.generated.h"

class UMMCheroInputComponent;
class UCameraComponent;
class USpringArmComponent;

DECLARE_DELEGATE_OneParam(FOnYawRotationChangedSignature, float)
DECLARE_DELEGATE_TwoParams(FOnCharacterMoveSignature, FVector2D, float)

UCLASS()
class MINIMAP_API AMMPlayerCharacter : public AMMCharacter
{
	GENERATED_BODY()

public:

	AMMPlayerCharacter();

	void OnPlayerMove();

	void OnPlayerControllerRotate();

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void UnPossessed() override;

	FORCEINLINE void UnbindDelegates()
	{
		OnControllerRotateDelegate.Unbind();
		OnCharacterMoveDelegate.Unbind();
	}

	FOnYawRotationChangedSignature OnControllerRotateDelegate;

	FOnCharacterMoveSignature OnCharacterMoveDelegate;

protected:

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = Input)
	TObjectPtr<UMMCheroInputComponent> CheroInputComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<USpringArmComponent> CameraBoom = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<UCameraComponent> FollowCamera = nullptr;
};
