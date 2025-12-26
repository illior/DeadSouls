// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DSGameplayAbility.generated.h"

class UDSAbilitySystemComponent;
class ADSPlayerController;
class ADSCharacter;

UCLASS(Abstract)
class DEADSOULS_API UDSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDSGameplayAbility();
	
	UFUNCTION(BlueprintCallable, Category = "DeadSouls|Ability")
	UDSAbilitySystemComponent* GetDSAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "DeadSouls|Ability")
	ADSPlayerController* GetDSPlayerControllerFromActorInfo() const;

	UFUNCTION(BlueprintCallable, Category = "DeadSouls|Ability")
	ADSCharacter* GetDSCharacterFromActorInfo() const;
	
protected:
	UPROPERTY()
	TObjectPtr<class UDSTickManagerComponent> TickComponent;

	FDelegateHandle TickDelegateHandle;

	UFUNCTION(BlueprintCallable, Category = "DeadSouls|Ability")
	void RegisterTick();
	UFUNCTION(BlueprintCallable, Category = "DeadSouls|Ability")
	void UnregisterTick();

	UFUNCTION(BlueprintImplementableEvent, Category = "DeadSouls|Ability", meta = (DisplayName = "AbilityTick"))
	void NativeTick(float DeltaTime);
	
	virtual void AbilityTick(float DeltaTime);
};
