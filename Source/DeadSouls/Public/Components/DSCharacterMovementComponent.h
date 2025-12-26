// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DSCharacterMovementComponent.generated.h"

class UAbilitySystemComponent;

UCLASS()
class DEADSOULS_API UDSCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UDSCharacterMovementComponent();
	
	virtual void InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystem);
	
	virtual float GetMaxSpeed() const override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking", meta = (ClampMin = "0", UIMin = "0", ForceUnits = "cm/s", DisplayAfter = "MaxWalkSpeedCrouched"))
	float MaxSprintSpeed = 450.0f;

	bool bShouldSprint = false;
	
	void CharacterStateHandle(FGameplayTag InGameplayTag, int32 InInt);
};
