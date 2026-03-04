// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSGameplayAbility.h"
#include "DSGameplayAbility_ChangeWeapon.generated.h"

UCLASS(Abstract)
class DEADSOULS_API UDSGameplayAbility_ChangeWeapon : public UDSGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDSGameplayAbility_ChangeWeapon(const FObjectInitializer& ObjectInitializer);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UInputAction> ChangeWeaponAction;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
