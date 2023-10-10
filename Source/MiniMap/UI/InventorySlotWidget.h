// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "InventorySlotWidget.generated.h"

class UTexture2D;

UCLASS()
class MINIMAP_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	FORCEINLINE void SetSlotBorderColor(FLinearColor NewColor)
	{
		if (IsValid(SlotBorder))
		{
			SlotBorder->SetBrushColor(NewColor);
		}
	}

	FORCEINLINE void SetSlotImageColor(FLinearColor NewColor)
	{
		if (IsValid(SlotImage))
		{
			SlotImage->SetColorAndOpacity(NewColor);
		}
	}

	FORCEINLINE void SetSlotImageFromTexture(UTexture2D* NewTexture)
	{
		if (IsValid(SlotImage))
		{
			SlotImage->SetBrushFromTexture(NewTexture);
		}
	}

	FORCEINLINE void SetSlotImageFromSotfTexture(TSoftObjectPtr<UTexture2D> NewTexture)
	{
		if (IsValid(SlotImage))
		{
			SlotImage->SetBrushFromSoftTexture(NewTexture);
		}
	}

	FORCEINLINE void SetIsNotEmpty(bool bNewIsNotEmpty)
	{
		bIsNotEmpty = bNewIsNotEmpty;
	}

	FORCEINLINE bool IsNotEmpty() const { return bIsNotEmpty; }

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBorder> SlotBorder = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> SlotImage = nullptr;

	bool bIsNotEmpty = false;
};
