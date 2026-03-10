// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/DSGameplayAbility_Aiming.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "AbilitySystem/DSGameplayTags.h"

#include "Character/DSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

FDSCameraTransitionLatentAction::FDSCameraTransitionLatentAction(
		USpringArmComponent* InSpringArm,
		UCameraComponent* InCamera,
		float InTargetArmLength,
		float InTargetFOV,
		FVector InTargetOffset,
		float InDuration,
		const FAlphaBlendArgs& BlendArgs,
		const FLatentActionInfo& LatentInfo)
		: SpringArm(InSpringArm)
		, Camera(InCamera)
		, TargetArmLength(InTargetArmLength)
		, TargetFOV(InTargetFOV)
		, TargetSocketOffset(InTargetOffset)
		, Duration(InDuration)
		, Elapsed(0.f)
		, ExecutionFunction(LatentInfo.ExecutionFunction)
		, OutputLink(LatentInfo.Linkage)
		, CallbackTarget(LatentInfo.CallbackTarget)
{
	StartArmLength = SpringArm ? SpringArm->TargetArmLength : 0.f;
	StartFOV = Camera ? Camera->FieldOfView : 0.f;
	StartSocketOffset = SpringArm ? SpringArm->SocketOffset : FVector::ZeroVector;

	Blend = FAlphaBlend(BlendArgs);
	Blend.SetBlendTime(Duration);
	Blend.Reset();
}

void FDSCameraTransitionLatentAction::UpdateOperation(FLatentResponse& Response)
{
	if (!IsValid(SpringArm) || !IsValid(Camera))
	{
		Response.DoneIf(true);
		return;
	}

	float DeltaTime = Response.ElapsedTime();
	Blend.Update(DeltaTime);
	float Alpha = Blend.GetBlendedValue();

	SpringArm->TargetArmLength = FMath::Lerp(StartArmLength, TargetArmLength, Alpha);
	SpringArm->SocketOffset = FMath::Lerp(StartSocketOffset, TargetSocketOffset, Alpha);
	Camera->FieldOfView = FMath::Lerp(StartFOV, TargetFOV, Alpha);

	bool bFinished = Blend.IsComplete();
	Response.DoneIf(bFinished);
}

UDSGameplayAbility_Aiming::UDSGameplayAbility_Aiming()
{
	SetAssetTags(FGameplayTagContainer(DSGameplayTags::GameplayAbility_Aim));
	
	CancelAbilitiesWithTag = FGameplayTagContainer(DSGameplayTags::GameplayAbility_Sprint);
	BlockAbilitiesWithTag = FGameplayTagContainer(DSGameplayTags::GameplayAbility_Sprint);
	
	ActivationOwnedTags = FGameplayTagContainer(DSGameplayTags::Character_State_Aim);
}

void UDSGameplayAbility_Aiming::OnInputRelease(float TimeHeld)
{
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UDSGameplayAbility_Aiming::StartTransition(const float& InTargetArmLength, const FVector& InTargetSocketOffset, const float& InTargetFOV)
{
	ADSCharacter* Character = GetDSCharacterFromActorInfo();
	UWorld* World = GetWorld();
	
	if (IsValid(Character) && IsValid(World))
	{
		FLatentActionManager& Manager = World->GetLatentActionManager();
		
		Manager.RemoveActionsForObject(Character->GetSpringArmComponent());
		
		FLatentActionInfo LatentInfo;
		LatentInfo.UUID = 3302;
		
		Manager.AddNewAction(
			Character->GetSpringArmComponent(),
			LatentInfo.UUID,
			new FDSCameraTransitionLatentAction(
				Character->GetSpringArmComponent(),
				Character->GetCameraComponent(),
				InTargetArmLength,
				InTargetFOV,
				InTargetSocketOffset,
				Duration,
				BlendType,
				LatentInfo
			)
		);
	}
}

void UDSGameplayAbility_Aiming::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCanceled = true;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCanceled);

		return;
	}
	
	StartTransition(TargetArmLength, TargetSocketOffset, TargetFOV);
	
	UAbilityTask_WaitInputRelease* WaitInputRelease = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	WaitInputRelease->OnRelease.AddDynamic(this, &UDSGameplayAbility_Aiming::OnInputRelease);
	WaitInputRelease->ReadyForActivation();
}

void UDSGameplayAbility_Aiming::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	ADSCharacter* Character = GetDSCharacterFromActorInfo();
	if (IsValid(Character))
	{
		StartTransition(Character->GetInitialArmLength(), Character->GetInitialSocketOffset(), Character->GetInitialFOV());
	}
}
