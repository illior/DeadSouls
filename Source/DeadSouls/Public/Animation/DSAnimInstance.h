// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimInstance.h"
#include "DSAnimInstance.generated.h"

UCLASS()
class DEADSOULS_API UDSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void InitializeWithAbilitySystem(UAbilitySystemComponent* AbilitySystem);
	
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
};
