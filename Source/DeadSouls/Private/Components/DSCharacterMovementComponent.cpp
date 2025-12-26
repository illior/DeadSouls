// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/DSCharacterMovementComponent.h"
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
	
	AbilitySystem->RegisterGameplayTagEvent(
		FGameplayTag::RequestGameplayTag(FName(TEXT("Character.State"))),
		EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDSCharacterMovementComponent::CharacterStateHandle
	);
}

float UDSCharacterMovementComponent::GetMaxSpeed() const
{
	if (bShouldSprint)
	{
		return MaxSprintSpeed;
	}

	return Super::GetMaxSpeed();
}

void UDSCharacterMovementComponent::CharacterStateHandle(FGameplayTag InGameplayTag, int32 InInt)
{
	UAbilitySystemComponent* AbilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
	if (!IsValid(AbilitySystem))
	{
		return;
	}
	
	bShouldSprint = AbilitySystem->GetGameplayTagCount(DSGameplayTags::Character_State_Sprint) != 0;
	if (AbilitySystem->GetGameplayTagCount(DSGameplayTags::Character_State_DisableMovement) != 0)
	{
		SetMovementMode(EMovementMode::MOVE_None);
	}
	else
	{
		SetMovementMode(EMovementMode::MOVE_Walking);
	}
}
