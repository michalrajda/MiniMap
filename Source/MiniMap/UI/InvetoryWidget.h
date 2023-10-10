// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InvetoryWidget.generated.h"

class UUniformGridPanel;
class UInventorySlotWidget;
class UTexture2D;
class UWeaponManagerComponent;

UCLASS()
class MINIMAP_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void InitializeInvenotrySlotsFromWeaponManager(UWeaponManagerComponent* InWeaponManager);

	void ResetSlots();

	void ChangeColorInActiveSlot(bool bIsWeaponEquipped);

	void ChangeActiveSlot(int32 PrevActiveSlot, int32 NewActiveSlot);

	void SetSlotsImageFromSotfTexture(TArray<TSoftObjectPtr<UTexture2D>>& InTextures);

protected:

	virtual void NativePreConstruct() override;

	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor EmptySlotColor = FLinearColor::Gray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor EquippedSlotColor = FLinearColor::Green;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor ActiveSlotColor = FLinearColor::Blue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor InActiveSlotColor = FLinearColor::Yellow;

	UPROPERTY(VisibleAnywhere, Transient)
	TArray<TObjectPtr<UInventorySlotWidget>> InventorySlots;

	UPROPERTY(VisibleAnywhere, Transient)
	TObjectPtr<UInventorySlotWidget> ActiveInventorySlot = nullptr;
};
