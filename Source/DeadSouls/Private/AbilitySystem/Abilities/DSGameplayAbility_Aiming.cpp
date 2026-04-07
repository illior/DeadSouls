// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/DSGameplayAbility_Aiming.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "AbilitySystem/DSGameplayTags.h"

#include "Character/DSCharacter.h"
#include "Components/DSSpringArmComponent.h"

UDSGameplayAbility_Aiming::UDSGameplayAbility_Aiming()
{
	SetAssetTags(FGameplayTagContainer(DSGameplayTags::GameplayAbility_Aim));
	
	CancelAbilitiesWithTag = FGameplayTagContainer(DSGameplayTags::GameplayAbility_Sprint);
	BlockAbilitiesWithTag = FGameplayTagContainer(DSGameplayTags::GameplayAbility_Sprint);
	
	ActivationOwnedTags = FGameplayTagContainer(DSGameplayTags::Character_State_Aim);
}

void UDSGameplayAbility_Aiming::OnInputRelease(float TimeHeld)
{
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDSGameplayAbility_Aiming::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCanceled = true;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCanceled);

		return;
	}
	
	if (IsValid(AimEffect))
	{
		const UGameplayEffect* GameplayEffect = AimEffect->GetDefaultObject<UGameplayEffect>();
		ActiveAimEffect = ApplyGameplayEffectToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, GameplayEffect, 0, 1);
	}
	
	ADSCharacter* Character = GetDSCharacterFromActorInfo();
	if (IsValid(Character))
	{
		UDSSpringArmComponent* SpringArmComponent = Character->GetSpringArmComponent();
		if (IsValid(SpringArmComponent))
		{
			SpringArmComponent->StartAiming(ArmLength, TargetOffset, SocketOffset, InBlendType);
		}
	}
	
	UAbilityTask_WaitInputRelease* WaitInputRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	WaitInputRelease->OnRelease.AddDynamic(this, &UDSGameplayAbility_Aiming::OnInputRelease);
	WaitInputRelease->ReadyForActivation();
}

void UDSGameplayAbility_Aiming::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	if (ActiveAimEffect.IsValid())
	{
		BP_RemoveGameplayEffectFromOwnerWithHandle(ActiveAimEffect);
	}
	
	ADSCharacter* Character = GetDSCharacterFromActorInfo();
	if (IsValid(Character))
	{
		UDSSpringArmComponent* SpringArmComponent = Character->GetSpringArmComponent();
		if (IsValid(SpringArmComponent))
		{
			SpringArmComponent->StopAiming(OutBlendType);
		}
	}
}
