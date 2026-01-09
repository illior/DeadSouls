// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DSPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionInstance;

UCLASS()
class DEADSOULS_API ADSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADSPlayerController();
	
	UFUNCTION(BlueprintCallable)
	void PauseGame();
	UFUNCTION(BlueprintCallable)
	void ResumeGame();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DeadSouls: Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeadSouls: Input")
	TObjectPtr<UInputMappingContext> PauseMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DeadSouls: Input")
	TObjectPtr<UInputAction> OpenPauseMenuAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DeadSouls: Input")
	TObjectPtr<UInputAction> OpenInventoryAction;
	
	//~ Begin APlayerController Interface.
	virtual void SetupInputComponent() override;
	//~ End APlayerController Interface
	
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	//~ End AActor Interface
	
private:
	void InputPause(const FInputActionInstance& Value);
	void InputInventory(const FInputActionInstance& Value);
};
