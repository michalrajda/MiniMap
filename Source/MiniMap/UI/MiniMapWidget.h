// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "MiniMapWidget.generated.h"

class UOverlay;
class UTexture2D;
class AMMPlayerCharacter;

UCLASS()
class MINIMAP_API UMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	FORCEINLINE void SetPlayerIconTexture(TSoftObjectPtr<UTexture2D> IconTexture)
	{
		SetTextureInImage(PlayerIconTexture, IconTexture, PlayerIcon);
	}

	FORCEINLINE void SetCameraIconTexture(TSoftObjectPtr<UTexture2D> IconTexture)
	{
		SetTextureInImage(CameraIconTexture, IconTexture, CameraIcon);
	}

	void SetMiniMapSoftTexture(TSoftObjectPtr<UTexture2D> NewMiniMapTexture);

	FORCEINLINE void SetMiniMapTexture(UTexture2D* NewMiniMapTexture)
	{
		if (IsValid(NewMiniMapTexture) && IsValid(MiniMapMaterialInstance))
		{
			CancelTextureStreaming();
			MiniMapMaterialInstance->SetTextureParameterValue(TEXT("Texture"), NewMiniMapTexture);
		}
	}

	FORCEINLINE void SetMiniMapRamTexture(TSoftObjectPtr<UTexture2D> RamTexture)
	{
		SetTextureInImage(MiniMapRamTexture, RamTexture, MiniMapRam);
	}

	FORCEINLINE void SetPlayerIconSize(FVector2D NewSize)
	{
		PlayerIconSize = NewSize;
		if (IsValid(PlayerIcon))
		{
			PlayerIcon->SetDesiredSizeOverride(PlayerIconSize);
		}
	}

	FORCEINLINE void SetCameraIconSize(FVector2D NewSize)
	{
		CameraIconSize = NewSize;
		if (IsValid(CameraIcon))
		{
			CameraIcon->SetDesiredSizeOverride(CameraIconSize);
			CameraIcon->SetRenderTranslation(FVector2D(0.0f, -CameraIconSize.Y * 0.5f));
		}
	}

	FORCEINLINE void SetCameraIconAngle(float NewAngle)
	{
		if (IsValid(CameraIcon))
		{
			CameraIcon->SetRenderTransformAngle(NewAngle);
		}
	}

	FORCEINLINE void SetPlayerIconAngle(float NewAngle)
	{
		if (IsValid(PlayerIcon))
		{
			PlayerIcon->SetRenderTransformAngle(NewAngle);
		}
	}

	FORCEINLINE void SetPlayerPosition(FVector2D NewPosition)
	{
		if (IsValid(MiniMapMaterialInstance))
		{
			MiniMapMaterialInstance->SetScalarParameterValue(TEXT("X"), NewPosition.X);
			MiniMapMaterialInstance->SetScalarParameterValue(TEXT("Y"), NewPosition.Y);
		}
	}


	FORCEINLINE void SetMiniMapDimension(float NewDimension)
	{
		Dimension = (NewDimension > 0.0f) ? NewDimension : 1.0f;
		if (IsValid(MiniMapMaterialInstance))
		{
			MiniMapMaterialInstance->SetScalarParameterValue(TEXT("Dimension"), Dimension);
		}
	}

	FORCEINLINE void SetMiniMapZoom(float NewZoom)
	{
		Zoom = FMath::Clamp(NewZoom, MinZoom, MaxZoom);
		if (IsValid(MiniMapMaterialInstance))
		{
			MiniMapMaterialInstance->SetScalarParameterValue(TEXT("Zoom"), Zoom);
		}
	}

	FORCEINLINE void SetMiniMapOffset(FVector2D NewOffset)
	{
		MiniMapOffset = NewOffset;
		if (IsValid(MiniMapMaterialInstance))
		{
			MiniMapMaterialInstance->SetScalarParameterValue(TEXT("X Offset"), MiniMapOffset.X);
			MiniMapMaterialInstance->SetScalarParameterValue(TEXT("Y Offset"), MiniMapOffset.Y);
		}
	}

	void OnPlayerMove(FVector2D PlayerLocation, float Angle);

	void InitializeMiniMapWidgetFromPawn(AMMPlayerCharacter* InPawn);

protected:

	void CancelMaterialStreaming();

	void CancelTextureStreaming();

	virtual void NativePreConstruct() override;

	void SetTextureInImage(TSoftObjectPtr<UTexture2D>& OldTexture, TSoftObjectPtr<UTexture2D>& NewTexture, TObjectPtr<UImage>& InImage);

	void InitMiniMapImage();

	void InitMiniMapRam();

	void InitPlayerIcon();

	void InitCameraIcon();

	void InitAllPropertiesInMiniMapMaterial(UMaterialInstance* NewMaterial);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UOverlay> OverlayMiniMap = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> MiniMapImage = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> MiniMapRam = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> PlayerIcon = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> CameraIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, noClear)
	TSoftObjectPtr<UTexture2D> PlayerIconTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, noClear)
	TSoftObjectPtr<UTexture2D> CameraIconTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, noClear)
	TSoftObjectPtr<UTexture2D> MiniMapTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, noClear)
	TSoftObjectPtr<UTexture2D> MiniMapRamTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, noClear)
	TSoftObjectPtr<UMaterialInstance> MiniMapMaterial = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	TObjectPtr<UMaterialInstanceDynamic> MiniMapMaterialInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Dimension = 5000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Zoom = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinZoom = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxZoom = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D MiniMapOffset = FVector2D(1500.0f, 1750.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D CameraIconSize = FVector2D(32.0f, 64.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D PlayerIconSize = FVector2D(32.0f, 32.0f);

protected:

	TSharedPtr<FStreamableHandle> MaterialStreamingHandle;

	FSoftObjectPath MaterialStreamingPath;

	TSharedPtr<FStreamableHandle> TextureStreamingHandle;

	FSoftObjectPath TextureStreamingPath;
};
