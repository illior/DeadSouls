// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSGameplayAbility.h"
#include "DSGameplayAbility_ChangeWeapon.generated.h"

class UDSWeaponData;

UCLASS(Abstract)
class DEADSOULS_API UDSGameplayAbility_ChangeWeapon : public UDSGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDSGameplayAbility_ChangeWeapon(const FObjectInitializer& ObjectInitializer);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> ChangeWeaponAction;
	
	UPROPERTY()
	UDSWeaponData* EquippedWeapon;
	UPROPERTY()
	UDSWeaponData* WeaponToEquip;
	
	void EquipWeapon();
	void UnequipWeapon();
	
	UFUNCTION()
	void CompleteEquip();
	UFUNCTION()
	void CompleteUnequip();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
