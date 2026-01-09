// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "DSInputTriggerDynamicPulse.generated.h"

UCLASS(NotBlueprintable, meta = (DisplayName = "DynamicPulse"))
class DEADSOULS_API UDSInputTriggerDynamicPulse : public UInputTriggerTimedBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Trigger Settings", meta = (ClampMin = "0"))
	float InitialInterval = 0.3f;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Trigger Settings", meta = (ClampMin = "0"))
	float MinInterval = 0.1f;
	
	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Trigger Settings", meta = (ClampMin = "0"))
	float IntervalDecayRate = 0.5f;
	
protected:
	virtual ETriggerState UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime) override;
	
private:
	float NextTimeTrigger = 0.0f;
};
