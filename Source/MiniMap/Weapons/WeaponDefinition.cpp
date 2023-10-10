// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDefinition.h"
#include "WeaponInstance.h"

UWeaponDefinition::UWeaponDefinition()
{
	InstanceType = UWeaponInstance::StaticClass();
}
