// Fill out your copyright notice in the Description page of Project Settings.

#include "DeadSouls/Public/Player/DSPlayerState.h"

#include "AbilitySystem/DSAbilitySystemComponent.h"

ADSPlayerState::ADSPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UDSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void ADSPlayerState::AddAbilitySet(UDSAbilitySet* InAbilitySet)
{
	if (!IsValid(InAbilitySet) || !IsValid(AbilitySystemComponent) || AbilitySetHandles.Contains(InAbilitySet))
	{
		return;
	}
	
	const FDSAbilitySetGrantedHandles Handle = InAbilitySet->GiveToAbilitySystem(AbilitySystemComponent);
	AbilitySetHandles.Add(InAbilitySet, Handle);
}

void ADSPlayerState::RemoveAbilitySet(UDSAbilitySet* InAbilitySet)
{
	if (!IsValid(InAbilitySet) || !IsValid(AbilitySystemComponent))
	{
		return;
	}
	
	if (FDSAbilitySetGrantedHandles* Handles = AbilitySetHandles.Find(InAbilitySet))
	{
		Handles->TakeFromAbilitySystem(AbilitySystemComponent);
		AbilitySetHandles.Remove(InAbilitySet);
	}
}

void ADSPlayerState::ClearAllAbilitySets()
{
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	for (TPair<UDSAbilitySet*, FDSAbilitySetGrantedHandles>& Elem : AbilitySetHandles)
	{
		Elem.Value.TakeFromAbilitySystem(AbilitySystemComponent);
	}

	AbilitySetHandles.Empty();
}

UAbilitySystemComponent* ADSPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
