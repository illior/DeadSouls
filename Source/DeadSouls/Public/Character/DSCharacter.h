// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "DSCharacter.generated.h"

class UDSInventoryComponent;
class UDSAbilitySystemComponent;
class USpringArmComponent;
class UCameraComponent;

class UInputAction;
struct FInputActionInstance;

UCLASS()
class DEADSOULS_API ADSCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ADSCharacter(const FObjectInitializer& ObjInit);

	USpringArmComponent* GetSpringArmComponent() const;
	UCameraComponent* GetCameraComponent() const;
	UDSInventoryComponent* GetInventoryComponent() const;
	//~ Begin AbilitySystem Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End AbilitySystem Interface
	
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//~ End APawn Interface
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DeadSouls: Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DeadSouls: Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DeadSouls: Input", Meta = (Categories = "InputTag"))
	TMap<TObjectPtr<UInputAction>, FGameplayTag> InputBindings;
	
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	//~ End AActor Interface

	void InputLook(const FInputActionInstance& Value);
	void InputMove(const FInputActionInstance& Value);

	void InputAbilityPress(const FInputActionInstance& Value);
	void InputAbilityRelease(const FInputActionInstance& Value);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDSInventoryComponent> InventoryComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDSAbilitySystemComponent> AbilitySystemComponent;
};
