// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DSAbilitySystemComponent.generated.h"

UCLASS()
class DEADSOULS_API UDSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "DSAbilitySystemComponent")
	void PressInputTag(const FGameplayTag InputTag);
	UFUNCTION(BlueprintCallable, Category = "DSAbilitySystemComponent")
	void ReleaseInputTag(const FGameplayTag InputTag);
};
