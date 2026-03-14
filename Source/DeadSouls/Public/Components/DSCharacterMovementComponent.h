// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DSCharacterMovementComponent.generated.h"

class UAbilitySystemComponent;
struct FOnAttributeChangeData;

UCLASS()
class DEADSOULS_API UDSCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UDSCharacterMovementComponent();
	
	virtual void InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystem);
protected:
	void OnMoveSpeedChanged(const FOnAttributeChangeData& Data);
};
