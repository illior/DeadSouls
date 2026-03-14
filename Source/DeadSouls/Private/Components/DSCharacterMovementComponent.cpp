// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/DSCharacterMovementComponent.h"
#include "AbilitySystem/Attributes/DSMovementAttributeSet.h"
#include "AbilitySystem/DSGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

UDSCharacterMovementComponent::UDSCharacterMovementComponent()
{
	MaxWalkSpeed = 300.0f;
	GroundFriction = 0.9f;
	MaxAcceleration = 1024.0f;
	BrakingFriction = 0.5f;
	bUseSeparateBrakingFriction = true;
	Mass = 60.0f;
}

void UDSCharacterMovementComponent::InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystem)
{
	check(AbilitySystem);
	
	AbilitySystem->GetGameplayAttributeValueChangeDelegate(UDSMovementAttributeSet::GetMoveSpeedAttribute()).AddUObject(this, &UDSCharacterMovementComponent::OnMoveSpeedChanged);
}

void UDSCharacterMovementComponent::OnMoveSpeedChanged(const FOnAttributeChangeData& Data)
{
	MaxWalkSpeed = Data.NewValue;
}
