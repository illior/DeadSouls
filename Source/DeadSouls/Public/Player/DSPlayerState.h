// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/DSAbilitySet.h"
#include "DSPlayerState.generated.h"

class UDSAbilitySystemComponent;

UCLASS()
class DEADSOULS_API ADSPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ADSPlayerState();

	UFUNCTION(BlueprintCallable, Category = "DSPlayerState")
	void AddAbilitySet(UDSAbilitySet* InAbilitySet);
	UFUNCTION(BlueprintCallable, Category = "DSPlayerState")
	void RemoveAbilitySet(UDSAbilitySet* InAbilitySet);
	UFUNCTION(BlueprintCallable, Category = "DSPlayerState")
	void ClearAllAbilitySets();
	
	//~ Begin AbilitySystem Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End AbilitySystem Interface
	
protected:
	UPROPERTY()
	TMap<TObjectPtr<UDSAbilitySet>, FDSAbilitySetGrantedHandles> AbilitySetHandles;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDSAbilitySystemComponent> AbilitySystemComponent;
};
