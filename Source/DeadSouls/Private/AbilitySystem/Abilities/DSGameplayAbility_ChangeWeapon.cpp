// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/DSGameplayAbility_ChangeWeapon.h"
#include "AbilitySystem/DSGameplayTags.h"

#include "Player/DSPlayerController.h"
#include "EnhancedPlayerInput.h"

#include "Character/DSCharacter.h"
#include "Components/DSInventoryComponent.h"

UDSGameplayAbility_ChangeWeapon::UDSGameplayAbility_ChangeWeapon(const FObjectInitializer& ObjectInitializer)
{
	SetAssetTags(FGameplayTagContainer(DSGameplayTags::GameplayAbility_ChangeWeapon));
}

void UDSGameplayAbility_ChangeWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCanceled = true;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCanceled);

		return;
	}
	
	int32 Value = 0;
	ADSPlayerController* PlayerController = GetDSPlayerControllerFromActorInfo();
	if (IsValid(PlayerController))
	{
		UEnhancedPlayerInput* EnhancedPlayerInput = Cast<UEnhancedPlayerInput>(PlayerController->PlayerInput);
		if (IsValid(EnhancedPlayerInput))
		{
			Value = FMath::RoundToInt(EnhancedPlayerInput->GetActionValue(ChangeWeaponAction).Get<float>());
		}
	}
	
	ADSCharacter* Character = GetDSCharacterFromActorInfo();
	if (IsValid(Character))
	{
		UDSInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
		if (IsValid(InventoryComponent))
		{
			InventoryComponent->SetEquippedWeapon(InventoryComponent->GetFastAccessWeapon(static_cast<EDSWeaponFastAccessIndex>(Value)));
		}
	}
	
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCanceled);
}
