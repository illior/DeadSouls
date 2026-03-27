// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/DSGameplayAbility_ChangeWeapon.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/DSGameplayTags.h"

#include "Player/DSPlayerController.h"
#include "EnhancedPlayerInput.h"

#include "Character/DSCharacter.h"
#include "Components/DSInventoryComponent.h"
#include "DSWeaponData.h"

UDSGameplayAbility_ChangeWeapon::UDSGameplayAbility_ChangeWeapon(const FObjectInitializer& ObjectInitializer)
{
	SetAssetTags(FGameplayTagContainer(DSGameplayTags::GameplayAbility_ChangeWeapon));
}

void UDSGameplayAbility_ChangeWeapon::EquipWeapon()
{
	TOptional<FInstancedStruct> InstancedStruct = WeaponToEquip->GetItemProperty(FDSWeaponGameplayProperty::StaticStruct());
	
	if (InstancedStruct.IsSet())
	{
		const FDSWeaponGameplayProperty* EquippedWeaponProperty = InstancedStruct->GetPtr<FDSWeaponGameplayProperty>();
		
		UAbilityTask_PlayMontageAndWait* EquipTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			FName(TEXT("PlayMontageAndWait")),
			EquippedWeaponProperty->EquipMontage
		);
		
		EquipTask->OnCompleted.AddDynamic(this, &UDSGameplayAbility_ChangeWeapon::CompleteEquip);
		EquipTask->ReadyForActivation();
	}
}

void UDSGameplayAbility_ChangeWeapon::UnequipWeapon()
{
	TOptional<FInstancedStruct> InstancedStruct = EquippedWeapon->GetItemProperty(FDSWeaponGameplayProperty::StaticStruct());
	
	if (InstancedStruct.IsSet())
	{
		const FDSWeaponGameplayProperty* EquippedWeaponProperty = InstancedStruct->GetPtr<FDSWeaponGameplayProperty>();
		
		UAbilityTask_PlayMontageAndWait* UnequipTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			FName(TEXT("PlayMontageAndWait")),
			EquippedWeaponProperty->UnequipMontage
		);
		
		UnequipTask->OnCompleted.AddDynamic(this, &UDSGameplayAbility_ChangeWeapon::CompleteUnequip);
		UnequipTask->ReadyForActivation();
	}
}

void UDSGameplayAbility_ChangeWeapon::CompleteEquip()
{
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCanceled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCanceled);
}

void UDSGameplayAbility_ChangeWeapon::CompleteUnequip()
{
	if (IsValid(WeaponToEquip))
	{
		EquipWeapon();
	}
	else
	{
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCanceled = true;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCanceled);
	}
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
			EquippedWeapon = InventoryComponent->GetEquippedWeapon();
			WeaponToEquip = InventoryComponent->GetFastAccessWeapon(static_cast<EDSWeaponFastAccessIndex>(Value));
			
			if (EquippedWeapon == WeaponToEquip)
			{
				constexpr bool bReplicateEndAbility = true;
				constexpr bool bWasCanceled = true;
				EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCanceled);

				return;
			}
			
			InventoryComponent->SetEquippedWeapon(WeaponToEquip);
			
			if (IsValid(EquippedWeapon))
			{
				UnequipWeapon();
				
				return;
			}
			
			if (IsValid(WeaponToEquip))
			{
				InventoryComponent->SetEquippedWeapon(WeaponToEquip);
				EquipWeapon();
				
				return;
			}
		}
	}
	
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCanceled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCanceled);
}
