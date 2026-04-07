// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSGameplayAbility.h"
#include "DSGameplayAbility_Aiming.generated.h"

UCLASS(Abstract)
class DEADSOULS_API UDSGameplayAbility_Aiming : public UDSGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDSGameplayAbility_Aiming();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Arm Settings")
	float ArmLength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Arm Settings")
	FVector SocketOffset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Arm Settings")
	FVector TargetOffset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Transition Settings")
	FAlphaBlendArgs InBlendType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Transition Settings")
	FAlphaBlendArgs OutBlendType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect Settings")
	TSubclassOf<UGameplayEffect> AimEffect;
	
	FActiveGameplayEffectHandle ActiveAimEffect;
	
	UFUNCTION()
	void OnInputRelease(float TimeHeld);
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
