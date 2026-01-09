// Fill out your copyright notice in the Description page of Project Settings.

#include "Input/DSInputTriggerDynamicPulse.h"

ETriggerState UDSInputTriggerDynamicPulse::UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime)
{
	ETriggerState State = Super::UpdateState_Implementation(PlayerInput, ModifiedValue, DeltaTime);
	if (State == ETriggerState::Ongoing)
	{
		if (HeldDuration > NextTimeTrigger)
		{
			State = ETriggerState::Triggered;
			NextTimeTrigger = HeldDuration + FMath::Max(MinInterval, InitialInterval - HeldDuration * IntervalDecayRate);
		}
	}
	else
	{
		NextTimeTrigger = 0.0f;
	}
	
	return State;
}
