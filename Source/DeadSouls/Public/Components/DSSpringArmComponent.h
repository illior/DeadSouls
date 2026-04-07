// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LatentActions.h"
#include "GameFramework/SpringArmComponent.h"
#include "DSSpringArmComponent.generated.h"

class FDSCameraTransitionLatentAction : public FPendingLatentAction
{
    
public:
	class UDSSpringArmComponent* SpringArm;

	float StartArmLength;
	float FinishArmLength;

	FVector StartSocketOffset;
	FVector FinishSocketOffset;
	
	FVector StartTargetOffset;
	FVector FinishTargetOffset;
	
	FAlphaBlend Blend;

	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;

	FDSCameraTransitionLatentAction(UDSSpringArmComponent* InSpringArm, float InStartArmLength, float InFinishArmLength, FVector InStartSocketOffset, FVector InFinishSocketOffset, FVector InStartTargetOffset, FVector InFinishTargetOffset, const FAlphaBlendArgs& BlendArgs, const FLatentActionInfo& LatentInfo);

	virtual void UpdateOperation(FLatentResponse& Response) override;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEADSOULS_API UDSSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:
	UDSSpringArmComponent();

	void StartAiming(float InTargetArmLength, FVector InTargetOffset, FVector InSocketOffset, const FAlphaBlendArgs& BlendArgs);
	void StopAiming(const FAlphaBlendArgs& BlendArgs);
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	
protected:
	float InitialTargetArmLength;
	FVector InitialSocketOffset;
	FVector InitialTargetOffset;
	
	float AimingTargetArmLength;
	FVector AimingSocketOffset;
	FVector AimingTargetOffset;
};
