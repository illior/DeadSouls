// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/DSTickManagerComponent.h"

UDSTickManagerComponent::UDSTickManagerComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UDSTickManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	OnTick.Broadcast(DeltaTime);
}
