// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapWidget.h"
#include "MMHUD.h"
#include "Engine/AssetManager.h"
#include "Character/MMPlayerCharacter.h"

void UMiniMapWidget::SetMiniMapSoftTexture(TSoftObjectPtr<UTexture2D> NewMiniMapTexture)
{
	if (UTexture2D* NewText = NewMiniMapTexture.Get())
	{
		SetMiniMapTexture(NewText);
		MiniMapTexture = NewMiniMapTexture;
		return;
	}
	if (NewMiniMapTexture.IsPending())
	{
		CancelTextureStreaming();
		TextureStreamingPath = NewMiniMapTexture.ToSoftObjectPath();
		TWeakObjectPtr<UMiniMapWidget> WeakThis(this);
		TextureStreamingHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
			TextureStreamingPath,
			[WeakThis, NewMiniMapTexture]() {
				if (UMiniMapWidget* StrongThis = WeakThis.Get())
				{
					if (StrongThis->TextureStreamingPath != NewMiniMapTexture.ToSoftObjectPath())
					{
						return;
					}
					StrongThis->SetMiniMapTexture(NewMiniMapTexture.Get());
					StrongThis->MiniMapTexture = NewMiniMapTexture;
				}
			},
			FStreamableManager::AsyncLoadHighPriority);
	}
}

void UMiniMapWidget::OnPlayerMove(FVector2D PlayerLocation, float Angle)
{
	SetPlayerIconAngle(Angle);
	SetPlayerPosition(PlayerLocation);
}

void UMiniMapWidget::InitializeMiniMapWidgetFromPawn(AMMPlayerCharacter* InPawn)
{
	if (IsValid(InPawn))
	{
		InPawn->OnControllerRotateDelegate.BindUObject(this, &UMiniMapWidget::SetCameraIconAngle);
		SetCameraIconAngle(InPawn->GetControlRotation().Yaw);
		InPawn->OnCharacterMoveDelegate.BindUObject(this, &UMiniMapWidget::OnPlayerMove);
		FVector CharLocation = InPawn->GetActorLocation();
		OnPlayerMove(FVector2D(CharLocation.X, CharLocation.Y), InPawn->GetActorRotation().Yaw);
	}
}

void UMiniMapWidget::CancelMaterialStreaming()
{
	if (MaterialStreamingHandle.IsValid())
	{
		MaterialStreamingHandle->CancelHandle();
		MaterialStreamingHandle.Reset();
	}
	MaterialStreamingPath.Reset();
}

void UMiniMapWidget::CancelTextureStreaming()
{
	if (TextureStreamingHandle.IsValid())
	{
		TextureStreamingHandle->CancelHandle();
		TextureStreamingHandle.Reset();
	}
	TextureStreamingPath.Reset();
}

void UMiniMapWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	InitMiniMapImage();
	InitMiniMapRam();
	InitPlayerIcon();
	InitCameraIcon();
}

void UMiniMapWidget::SetTextureInImage(TSoftObjectPtr<UTexture2D>& OldTexture, TSoftObjectPtr<UTexture2D>& NewTexture, TObjectPtr<UImage>& InImage)
{
	if (IsValid(InImage))
	{
		if (UTexture2D* NewText = NewTexture.Get())
		{
			OldTexture = NewTexture;
			InImage->SetBrushFromTexture(NewText);
			return;
		}
		if (NewTexture.IsPending())
		{
			OldTexture = NewTexture;
			InImage->SetBrushFromSoftTexture(NewTexture);
		}
	}
}

void UMiniMapWidget::InitMiniMapImage()
{
	if (UMaterialInstance* MaterialInstane = MiniMapMaterial.Get())
	{
		InitAllPropertiesInMiniMapMaterial(MaterialInstane);
		return;
	}
	if (MiniMapMaterial.IsPending())
	{
		CancelMaterialStreaming();
		TWeakObjectPtr<UMiniMapWidget> WeakThis(this);
		TextureStreamingHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
			MiniMapMaterial.ToSoftObjectPath(),
			[WeakThis]() {
				if (UMiniMapWidget* StrongThis = WeakThis.Get())
				{
					StrongThis->InitAllPropertiesInMiniMapMaterial(StrongThis->MiniMapMaterial.Get());
				}
			},
			FStreamableManager::AsyncLoadHighPriority);
	}
}

void UMiniMapWidget::InitMiniMapRam()
{
	if (IsValid(MiniMapRam))
	{
		MiniMapRam->SetBrushFromSoftTexture(MiniMapRamTexture);
	}
}

void UMiniMapWidget::InitPlayerIcon()
{
	if (IsValid(PlayerIcon))
	{
		PlayerIcon->SetDesiredSizeOverride(PlayerIconSize);
		PlayerIcon->SetBrushFromSoftTexture(PlayerIconTexture);
	}
}

void UMiniMapWidget::InitCameraIcon()
{
	if (IsValid(CameraIcon))
	{
		CameraIcon->SetDesiredSizeOverride(CameraIconSize);
		CameraIcon->SetRenderTranslation(FVector2D(0.0f, -CameraIconSize.Y * 0.5f));
		CameraIcon->SetBrushFromSoftTexture(CameraIconTexture);
	}
}

void UMiniMapWidget::InitAllPropertiesInMiniMapMaterial(UMaterialInstance* NewMaterial)
{
	if (IsValid(MiniMapImage) && IsValid(NewMaterial))
	{
		CancelMaterialStreaming();
		MiniMapMaterialInstance = UMaterialInstanceDynamic::Create(NewMaterial, this);
		if (IsValid(MiniMapMaterialInstance))
		{
			MiniMapImage->SetBrushFromMaterial(MiniMapMaterialInstance);
			Zoom = FMath::Clamp(Zoom, MinZoom, MaxZoom);
			Dimension = (Dimension > 0.0f) ? Dimension : 1.0f;
			MiniMapMaterialInstance->SetScalarParameterValue(TEXT("Dimension"), Dimension);
			MiniMapMaterialInstance->SetScalarParameterValue(TEXT("Zoom"), Zoom);
			MiniMapMaterialInstance->SetScalarParameterValue(TEXT("X Offset"), MiniMapOffset.X);
			MiniMapMaterialInstance->SetScalarParameterValue(TEXT("Y Offset"), MiniMapOffset.Y);
			if (APlayerController* PC = GetOwningPlayer())
			{
				if (AMMHUD* MyHud = PC->GetHUD<AMMHUD>())
				{
					MyHud->SetMiniMapWidget(this);
				}
				if (AMMPlayerCharacter* Char = PC->GetPawn<AMMPlayerCharacter>())
				{
					InitializeMiniMapWidgetFromPawn(Char);
				}
			}
			SetMiniMapSoftTexture(MiniMapTexture);
		}
	}
}
