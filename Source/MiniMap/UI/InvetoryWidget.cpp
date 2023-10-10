// Fill out your copyright notice in the Description page of Project Settings.


#include "InvetoryWidget.h"
#include "InventorySlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "MMHUD.h"
#include "Character/WeaponManagerComponent.h"
#include "Weapons/WeaponInstance.h"

void UInventoryWidget::InitializeInvenotrySlotsFromWeaponManager(UWeaponManagerComponent* InWeaponManager)
{
	ResetSlots();
	if (IsValid(InWeaponManager))
	{
		TArray<TSoftObjectPtr<UTexture2D>> WeaponTextures;
		for (const FWeaponEntry& WeaponEntry : InWeaponManager->GetSpawnedWeapons())
		{
			const UWeaponInstance* WI = WeaponEntry.WeaponInstance;
			if (IsValid(WI))
			{
				WeaponTextures.Add(WI->GetWeaponHUDTexture());
			}
			else
			{
				WeaponTextures.Add(TSoftObjectPtr<UTexture2D>());
			}
		}
		SetSlotsImageFromSotfTexture(WeaponTextures);
		ChangeActiveSlot(-1, InWeaponManager->GetActiveWeaponIndex());
		ChangeColorInActiveSlot(InWeaponManager->IsWeaponEuipped());
		InWeaponManager->OnActiveWeaponChanged.BindUObject(this, &UInventoryWidget::ChangeActiveSlot);
		InWeaponManager->OnChangedEquippingState.BindUObject(this, &UInventoryWidget::ChangeColorInActiveSlot);
	}
}

void UInventoryWidget::ResetSlots()
{
	for (UInventorySlotWidget* InvSlot : InventorySlots)
	{
		if (IsValid(InvSlot))
		{
			InvSlot->SetSlotImageFromTexture(nullptr);
			InvSlot->SetSlotBorderColor(EmptySlotColor);
			InvSlot->SetSlotImageColor(EmptySlotColor);
		}
	}
}

void UInventoryWidget::ChangeColorInActiveSlot(bool bIsWeaponEquipped)
{
	FLinearColor& NewColor = bIsWeaponEquipped ? EquippedSlotColor : ActiveSlotColor;
	if (IsValid(ActiveInventorySlot) && ActiveInventorySlot->IsNotEmpty())
	{
		ActiveInventorySlot->SetSlotBorderColor(NewColor);
	}
}

void UInventoryWidget::ChangeActiveSlot(int32 PrevActiveSlot, int32 NewActiveSlot)
{
	if (InventorySlots.IsValidIndex(PrevActiveSlot))
	{
		UInventorySlotWidget* PrevSlot = InventorySlots[PrevActiveSlot];
		if (IsValid(PrevSlot) && PrevSlot->IsNotEmpty())
		{
			PrevSlot->SetSlotBorderColor(InActiveSlotColor);
		}
	}
	if (InventorySlots.IsValidIndex(NewActiveSlot))
	{
		UInventorySlotWidget* NewSlot = InventorySlots[NewActiveSlot];
		if (IsValid(NewSlot) && NewSlot->IsNotEmpty())
		{
			ActiveInventorySlot = NewSlot;
			NewSlot->SetSlotBorderColor(ActiveSlotColor);
		}
	}
}

void UInventoryWidget::SetSlotsImageFromSotfTexture(TArray<TSoftObjectPtr<UTexture2D>>& InTextures)
{
	int32 MaxIndex = FMath::Min(InTextures.Num(), InventorySlots.Num());
	for (int32 Index = 0; Index < MaxIndex; ++Index)
	{
		UInventorySlotWidget* ISW = InventorySlots[Index];
		if (IsValid(ISW))
		{
			ISW->SetIsNotEmpty(true);
			ISW->SetSlotBorderColor(InActiveSlotColor);
			ISW->SetSlotImageFromSotfTexture(InTextures[Index]);
		}
	}
}

void UInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (IsValid(InventoryPanel))
	{
		InventorySlots.Empty();
		TArray<UWidget*> InventoryChildrens = InventoryPanel->GetAllChildren();
		for (UWidget* InventoryChildren : InventoryChildrens)
		{
			if (UInventorySlotWidget* InventorySlot = Cast<UInventorySlotWidget>(InventoryChildren))
			{
				InventorySlot->SetSlotImageFromTexture(nullptr);
				InventorySlot->SetSlotBorderColor(EmptySlotColor);
				InventorySlot->SetSlotImageColor(EmptySlotColor);
				InventorySlots.AddUnique(InventorySlot);
			}
		}
	}
}

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AMMHUD* MyHud = PC->GetHUD<AMMHUD>())
		{
			MyHud->SetInventoryWidget(this);
		}
	}
}
