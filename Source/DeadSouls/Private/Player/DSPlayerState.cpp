// Fill out your copyright notice in the Description page of Project Settings.

#include "DeadSouls/Public/Player/DSPlayerState.h"

#include "AbilitySystem/DSAbilitySystemComponent.h"

ADSPlayerState::ADSPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UDSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

UAbilitySystemComponent* ADSPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
