// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetManager/MMAssetManager.h"
#include "AbilitySystemGlobals.h"

UMMAssetManager& UMMAssetManager::Get()
{
	check(GEngine);

	if (UMMAssetManager* Singleton = Cast<UMMAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to MMAssetManager!"));

	// Fatal error above prevents this from being called.
	return *NewObject<UMMAssetManager>();
}

void UMMAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
