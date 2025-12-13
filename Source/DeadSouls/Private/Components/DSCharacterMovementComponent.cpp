// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/DSCharacterMovementComponent.h"
#include "AbilitySystem/DSGameplayTags.h"
#include "AbilitySystemComponent.h"

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
	
	AbilitySystem->RegisterGameplayTagEvent(DSGameplayTags::Character_State_Sprint, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDSCharacterMovementComponent::SprintTagHandle);
}

float UDSCharacterMovementComponent::GetMaxSpeed() const
{
	if (bShouldSprint)
	{
		return MaxSprintSpeed;
	}

	return Super::GetMaxSpeed();
}

void UDSCharacterMovementComponent::SprintTagHandle(FGameplayTag InGameplayTag, int32 InInt)
{
	bShouldSprint = InInt != 0;
}
