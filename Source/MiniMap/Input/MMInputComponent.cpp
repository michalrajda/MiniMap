// Fill out your copyright notice in the Description page of Project Settings.


#include "MMInputComponent.h"

void UMMInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveActionEventBinding(Handle);
	}
	BindHandles.Reset();
}
