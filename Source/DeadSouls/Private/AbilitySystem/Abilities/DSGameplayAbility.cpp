// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/DSGameplayAbility.h"
#include "AbilitySystem/DSAbilitySystemComponent.h"

#include "Components/DSTickManagerComponent.h"
#include "Player/DSPlayerController.h"
#include "Character/DSCharacter.h"

UDSGameplayAbility::UDSGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

UDSAbilitySystemComponent* UDSGameplayAbility::GetDSAbilitySystemComponentFromActorInfo() const
{
	return CurrentActorInfo ? Cast<UDSAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent.Get()) : nullptr;
}

ADSPlayerController* UDSGameplayAbility::GetDSPlayerControllerFromActorInfo() const
{
	return CurrentActorInfo ? Cast<ADSPlayerController>(CurrentActorInfo->PlayerController.Get()) : nullptr;
}

ADSCharacter* UDSGameplayAbility::GetDSCharacterFromActorInfo() const
{
	return CurrentActorInfo ? Cast<ADSCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}

void UDSGameplayAbility::RegisterTick()
{
	AActor* Owner = GetOwningActorFromActorInfo();

	TickComponent = Owner->FindComponentByClass<UDSTickManagerComponent>();
	if (!IsValid(TickComponent.Get()))
	{
		TickComponent = NewObject<UDSTickManagerComponent>(Owner);
		TickComponent->RegisterComponent();
	}

	TickDelegateHandle = TickComponent->OnTick.AddUObject(this, &UDSGameplayAbility::AbilityTick);
}

void UDSGameplayAbility::UnregisterTick()
{
	if (IsValid(TickComponent))
	{
		TickComponent->OnTick.Remove(TickDelegateHandle);
	}
}

void UDSGameplayAbility::AbilityTick(float DeltaTime)
{
	NativeTick(DeltaTime);
}
