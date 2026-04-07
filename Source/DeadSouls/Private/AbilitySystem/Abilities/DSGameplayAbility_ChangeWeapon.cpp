// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/DSGameplayAbility_ChangeWeapon.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/DSGameplayTags.h"

#include "Player/DSPlayerController.h"
#include "EnhancedPlayerInput.h"

#include "Character/DSCharacter.h"
#include "Player/DSPlayerState.h"
#include "Components/DSInventoryComponent.h"
#include "DSWeaponData.h"

UDSGameplayAbility_ChangeWeapon::UDSGameplayAbility_ChangeWeapon(const FObjectInitializer& ObjectInitializer)
{
	SetAssetTags(FGameplayTagContainer(DSGameplayTags::GameplayAbility_ChangeWeapon));
	
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = DSGameplayTags::GameplayEvent_ChangeWeapon;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	
	AbilityTriggers.Add(TriggerData);
}

void UDSGameplayAbility_ChangeWeapon::EquipWeapon()
{
	TOptional<FInstancedStruct> InstancedStruct = WeaponToEquip->GetItemProperty(FDSWeaponGameplayProperty::StaticStruct());
	
	if (InstancedStruct.IsSet())
	{
		const FDSWeaponGameplayProperty* EquippedWeaponProperty = InstancedStruct->GetPtr<FDSWeaponGameplayProperty>();
		
		if (ADSCharacter* Character = GetDSCharacterFromActorInfo())
		{
			Character->SetAnimLayer(EquippedWeaponProperty->AnimLayer);
			 
			if (ADSPlayerState* PlayerState = Cast<ADSPlayerState>(Character->GetPlayerState()))
			{
				PlayerState->AddAbilitySet(EquippedWeaponProperty->AbilitySet);
			}
		}
		
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
	TOptional<FInstancedStruct> InstancedStruct = EquippedWeapon->GetItemProperty(FDSWeaponGameplayProperty::StaticStruct());
	if (InstancedStruct.IsSet())
	{
		const FDSWeaponGameplayProperty* EquippedWeaponProperty = InstancedStruct->GetPtr<FDSWeaponGameplayProperty>();
		
		if (ADSCharacter* Character = GetDSCharacterFromActorInfo())
		{
			Character->ResetAnimLayer(EquippedWeaponProperty->AnimLayer);
			
			if (ADSPlayerState* PlayerState = Cast<ADSPlayerState>(Character->GetPlayerState()))
			{
				PlayerState->RemoveAbilitySet(EquippedWeaponProperty->AbilitySet);
			}
		}
	}
	
	if (IsValid(WeaponToEquip))
	{
		EquipWeapon();
	}
	else
	{
		if (ADSCharacter* Character = GetDSCharacterFromActorInfo())
		{
			Character->SetAnimLayer(Character->UnarmedAnimInstance);
		}
		
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
	
	ADSCharacter* Character = GetDSCharacterFromActorInfo();
	if (!IsValid(Character))
	{
		K2_CancelAbility();
		
		return;
	}
	
	UDSInventoryComponent* InventoryComponent = Character->GetInventoryComponent();
	if (!IsValid(InventoryComponent))
	{
		K2_CancelAbility();
		
		return;
	}
	
	if (TriggerEventData)
	{
		WeaponToEquip = Cast<UDSWeaponData>(static_cast<UDSWeaponData*>(TriggerEventData->OptionalObject));
	}
	else
	{
		ADSPlayerController* PlayerController = GetDSPlayerControllerFromActorInfo();
		if (IsValid(PlayerController))
		{
			UEnhancedPlayerInput* EnhancedPlayerInput = Cast<UEnhancedPlayerInput>(PlayerController->PlayerInput);
			if (IsValid(EnhancedPlayerInput))
			{
				int32 Value = FMath::RoundToInt(EnhancedPlayerInput->GetActionValue(ChangeWeaponAction).Get<float>());
			
				WeaponToEquip = InventoryComponent->GetFastAccessWeapon(static_cast<EDSWeaponFastAccessIndex>(Value));
			}
		}
	}
	
	EquippedWeapon = InventoryComponent->GetEquippedWeapon();
	if (EquippedWeapon == WeaponToEquip)
	{
		K2_CancelAbility();

		return;
	}
	
	InventoryComponent->SetEquippedWeapon(WeaponToEquip);
			
	if (IsValid(EquippedWeapon))
	{
		UnequipWeapon();
	}
	else
	{
		Character->ResetAnimLayer(Character->UnarmedAnimInstance);
				
		EquipWeapon();
	}
}
