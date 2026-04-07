// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/DSSpringArmComponent.h"

FDSCameraTransitionLatentAction::FDSCameraTransitionLatentAction(UDSSpringArmComponent* InSpringArm,
	float InStartArmLength,
	float InFinishArmLength,
	FVector InStartSocketOffset,
	FVector InFinishSocketOffset,
	FVector InStartTargetOffset,
	FVector InFinishTargetOffset,
	const FAlphaBlendArgs& BlendArgs,
	const FLatentActionInfo& LatentInfo
)
	: SpringArm(InSpringArm)
	, StartArmLength(InStartArmLength)
	, FinishArmLength(InFinishArmLength)
	, StartSocketOffset(InStartSocketOffset)
	, FinishSocketOffset(InFinishSocketOffset)
	, StartTargetOffset(InStartTargetOffset)
	, FinishTargetOffset(InFinishTargetOffset)
	, ExecutionFunction(LatentInfo.ExecutionFunction)
	, OutputLink(LatentInfo.Linkage)
	, CallbackTarget(LatentInfo.CallbackTarget)
{
	Blend = FAlphaBlend(BlendArgs);
	if (InSpringArm->TargetArmLength != InStartArmLength)
	{
		float Alpha = (InSpringArm->TargetArmLength - InStartArmLength) / (InFinishArmLength - InStartArmLength);
		
		Blend.Update(Alpha * BlendArgs.BlendTime);
	}
}

void FDSCameraTransitionLatentAction::UpdateOperation(FLatentResponse& Response)
{
	if (!IsValid(SpringArm))
	{
		Response.DoneIf(true);
		return;
	}
	
	Blend.Update(Response.ElapsedTime());
	float Alpha = Blend.GetBlendedValue();
	
	SpringArm->TargetArmLength = FMath::Lerp(StartArmLength, FinishArmLength, Alpha);
	
	SpringArm->SocketOffset = FMath::Lerp(StartSocketOffset, FinishSocketOffset, Alpha);
	SpringArm->TargetOffset = FMath::Lerp(StartTargetOffset, FinishTargetOffset, Alpha);
	
	bool bFinished = Blend.IsComplete();
	Response.DoneIf(bFinished);
}

UDSSpringArmComponent::UDSSpringArmComponent()
{
	
}

void UDSSpringArmComponent::StartAiming(float InTargetArmLength, FVector InTargetOffset, FVector InSocketOffset, const FAlphaBlendArgs& BlendArgs)
{
	AimingTargetArmLength = InTargetArmLength;
	AimingSocketOffset = InSocketOffset;
	AimingTargetOffset = InTargetOffset;
	
	if (UWorld* World = GetWorld())
	{
		FLatentActionManager& Manager = World->GetLatentActionManager();
		
		Manager.RemoveActionsForObject(this);
		
		FLatentActionInfo LatentInfo;
		LatentInfo.UUID = 3302;
		
		Manager.AddNewAction(
			this,
			LatentInfo.UUID,
			new FDSCameraTransitionLatentAction(
				this,
				InitialTargetArmLength,
				AimingTargetArmLength,
				InitialSocketOffset,
				AimingSocketOffset,
				InitialTargetOffset,
				AimingTargetOffset,
				BlendArgs,
				LatentInfo
			)
		);
	}
}

void UDSSpringArmComponent::StopAiming(const FAlphaBlendArgs& BlendArgs)
{
	if (UWorld* World = GetWorld())
	{
		FLatentActionManager& Manager = World->GetLatentActionManager();
		
		Manager.RemoveActionsForObject(this);
		
		FLatentActionInfo LatentInfo;
		LatentInfo.UUID = 3302;
		
		Manager.AddNewAction(
			this,
			LatentInfo.UUID,
			new FDSCameraTransitionLatentAction(
				this,
				AimingTargetArmLength,
				InitialTargetArmLength,
				AimingSocketOffset,
				InitialSocketOffset,
				AimingTargetOffset,
				InitialTargetOffset,
				BlendArgs,
				LatentInfo
			)
		);
	}
}

void UDSSpringArmComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//GEngine->AddOnScreenDebugMessage(1488, 0.1f, FColor::Red, FString::Printf(TEXT("InitialTargetArmLength: %f, InitialSocketOffset: %s, InitialTargetOffset: %s"), InitialTargetArmLength, *InitialSocketOffset.ToString(), *InitialTargetOffset.ToString()));
}

void UDSSpringArmComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitialTargetArmLength = TargetArmLength;
	InitialSocketOffset = SocketOffset;
	InitialTargetOffset = TargetOffset;
}
