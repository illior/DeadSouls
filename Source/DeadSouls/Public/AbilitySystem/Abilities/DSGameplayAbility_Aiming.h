// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LatentActions.h"
#include "DSGameplayAbility.h"
#include "DSGameplayAbility_Aiming.generated.h"

class FDSCameraTransitionLatentAction : public FPendingLatentAction
{
    
public:
    class USpringArmComponent* SpringArm;
    class UCameraComponent* Camera;

    float StartArmLength;
    float TargetArmLength;

    float StartFOV;
    float TargetFOV;

    FVector StartSocketOffset;
    FVector TargetSocketOffset;

    float Duration;
    float Elapsed;

    FAlphaBlend Blend;

    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

    FDSCameraTransitionLatentAction(USpringArmComponent* InSpringArm, UCameraComponent* InCamera, float InTargetArmLength, float InTargetFOV, FVector InTargetOffset, float InDuration, const FAlphaBlendArgs& BlendArgs, const FLatentActionInfo& LatentInfo);

    virtual void UpdateOperation(FLatentResponse& Response) override;
};

UCLASS(Abstract)
class DEADSOULS_API UDSGameplayAbility_Aiming : public UDSGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDSGameplayAbility_Aiming();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Transition Settings")
	float TargetArmLength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Transition Settings")
	float TargetFOV;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Transition Settings")
	FVector TargetSocketOffset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Transition Settings")
	float Duration;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Transition Settings")
	FAlphaBlendArgs BlendType;
	
	UFUNCTION()
	void OnInputRelease(float TimeHeld);
	
	void StartTransition(const float& InTargetArmLength, const FVector& InTargetSocketOffset, const float& InTargetFOV);
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
