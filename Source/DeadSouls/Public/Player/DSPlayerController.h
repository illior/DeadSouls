// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DSPlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class DEADSOULS_API ADSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADSPlayerController();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DeadSouls: Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	//~ Begin AActor Interface.
	virtual void BeginPlay() override;
	//~ End AActor Interface
};
