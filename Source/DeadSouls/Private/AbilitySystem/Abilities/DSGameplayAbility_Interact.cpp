// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/Abilities/DSGameplayAbility_Interact.h"

#include "AbilitySystem/DSGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Character/DSCharacter.h"
#include "Interaction/DSInteractionActor.h"

#include "Camera/CameraComponent.h"

FDSInteractData::FDSInteractData()
{
	InteractActor = TObjectPtr<ADSInteractionActor>(nullptr);
	Distance = UE_MAX_FLT;
}

FDSInteractData::FDSInteractData(ADSInteractionActor* InInteractActor, const float InDistance)
{
	InteractActor = TObjectPtr<ADSInteractionActor>(InInteractActor);
	Distance = InDistance;
}

UDSGameplayAbility_Interact::UDSGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer)
{
	SetAssetTags(FGameplayTagContainer(DSGameplayTags::GameplayAbility_Interact));
	
	ActivationOwnedTags.AddTag(DSGameplayTags::Character_State_DisableMovement);
}

bool UDSGameplayAbility_Interact::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return ClosestActorsBestIndex != INDEX_NONE && Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UDSGameplayAbility_Interact::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	RegisterTick();
}

void UDSGameplayAbility_Interact::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnRemoveAbility(ActorInfo, Spec);
	
	UnregisterTick();
}

void UDSGameplayAbility_Interact::OnDelayComplete()
{
	InteractionActor->StopInteract(InteractionActor->GetHoldTime());
	
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCanceled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCanceled);
}

void UDSGameplayAbility_Interact::OnInputRelease(float TimeHeld)
{
	InteractionActor->StopInteract(TimeHeld);
	
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCanceled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCanceled);
}

void UDSGameplayAbility_Interact::AbilityTick(float DeltaTime)
{
	Super::AbilityTick(DeltaTime);
	
	if (!bIsActive)
	{
		SearchInteractionObjects();
	}
}

void UDSGameplayAbility_Interact::SearchInteractionObjects()
{
	TArray<FDSInteractData> InteractObjects = GetAllInteractObjects();
	
	if (InteractObjects.Num() <= MaxShowWidgets)
	{
		UpdateInteractionData(InteractObjects);
		return;
	}
	
	TArray<FDSInteractData> NewClosestActors = GetClosestInteractObjects(InteractObjects);
	UpdateInteractionData(NewClosestActors);
}

TArray<FDSInteractData> UDSGameplayAbility_Interact::GetAllInteractObjects() const
{
	const ADSCharacter* Character = GetDSCharacterFromActorInfo();
	const UWorld* World = GetWorld();
	
	if (!IsValid(Character) || !IsValid(World))
	{
		return TArray<FDSInteractData>();
	}
	
	UCameraComponent* CharacterCamera = Character->GetCameraComponent();
	if (!IsValid(CharacterCamera))
	{
		return TArray<FDSInteractData>();
	}
	
	const FVector CameraLocation = Character->GetCameraComponent()->GetComponentLocation();
	const FVector CameraForward = Character->GetCameraComponent()->GetForwardVector();
	
	TSet<AActor*> OverlappingActors;
	Character->GetOverlappingActors(OverlappingActors, ADSInteractionActor::StaticClass());
	
	TArray<FDSInteractData> ValidActors;
	ValidActors.Reserve(OverlappingActors.Num());
	
	for (AActor* Actor : OverlappingActors)
	{
		ADSInteractionActor* InteractableActor = Cast<ADSInteractionActor>(Actor);
		if (!IsValid(InteractableActor) || !InteractableActor->GetEnabled())
		{
			continue;
		}

		const float Distance = FVector::Dist(Character->GetActorLocation(), InteractableActor->GetTargetLocation());
		if (Distance > InteractSearchDistance)
		{
			continue;
		}

		const float Alpha = (FMath::Max(Distance, InteractDistance) - InteractDistance) / (InteractSearchDistance - InteractDistance);
		const float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(
			FRotationMatrix::MakeFromX(InteractableActor->GetTargetLocation() - CameraLocation).Rotator().Vector().GetSafeNormal(),
			CameraForward.GetSafeNormal()
		)));
		if (Angle > FMath::Lerp(CloseInteractAngle, FarInteractAngle, Alpha))
		{
			continue;
		}

		FHitResult HitResult;
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActors(TArray<const AActor*>({Character, InteractableActor}));
		
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			InteractableActor->GetTargetLocation(),
			CameraLocation,
			ECC_Visibility,
			CollisionQueryParams
		);
		if (HitResult.bBlockingHit)
		{
			continue;
		}

		InteractableActor->SetDistanceAlpha(Alpha);
		ValidActors.Add(FDSInteractData(InteractableActor, Distance));
	}
	
	return ValidActors;
}

TArray<FDSInteractData> UDSGameplayAbility_Interact::GetClosestInteractObjects(const TArray<FDSInteractData>& InArray) const
{
	TArray<FDSInteractData> NewClosestActors;
	NewClosestActors.Reserve(MaxShowWidgets);
	
	int32 WorstIndex = INDEX_NONE;
	float CurrentWorstDist = InteractSearchDistance;

	for(FDSInteractData InteractData : InArray)
	{
		if (InteractData.Distance >= CurrentWorstDist)
		{
			continue;
		}

		if (NewClosestActors.Num() < MaxShowWidgets)
		{
			NewClosestActors.Add(InteractData);

			if (NewClosestActors.Num() == MaxShowWidgets)
			{
				WorstIndex = FindWorstIndex(NewClosestActors);
				CurrentWorstDist = NewClosestActors[FindWorstIndex(NewClosestActors)].Distance;
			}
		}
		else
		{
			NewClosestActors[WorstIndex] = InteractData;

			WorstIndex = FindWorstIndex(NewClosestActors);
			CurrentWorstDist = NewClosestActors[WorstIndex].Distance;
		}
	}
	
	return NewClosestActors;
}

void UDSGameplayAbility_Interact::UpdateInteractionData(const TArray<FDSInteractData>& NewClosestActors)
{
	for (FDSInteractData PrevInteractData : ClosestActors)
	{
		if (IsValid(PrevInteractData.InteractActor) && !NewClosestActors.Contains(PrevInteractData))
		{
			PrevInteractData.InteractActor->SetShowWidget(false);
		}
	}
	
	for (FDSInteractData NewInteractData : NewClosestActors)
	{
		if (IsValid(NewInteractData.InteractActor) && !ClosestActors.Contains(NewInteractData))
		{
			NewInteractData.InteractActor->SetShowWidget(true);
		}
	}
	
	const int32 NewBestIndex = FindBestIndex(NewClosestActors, InteractDistance);
	if (NewBestIndex != INDEX_NONE)
	{
		if (ClosestActorsBestIndex == INDEX_NONE)
		{
			NewClosestActors[NewBestIndex].InteractActor->SetShowWidgetKey(true);
		}
		else if (NewClosestActors[NewBestIndex] != ClosestActors[ClosestActorsBestIndex])
		{
			NewClosestActors[NewBestIndex].InteractActor->SetShowWidgetKey(true);
			ClosestActors[ClosestActorsBestIndex].InteractActor->SetShowWidgetKey(false);
		}
	}
	else
	{
		if (ClosestActorsBestIndex != INDEX_NONE)
		{
			ClosestActors[ClosestActorsBestIndex].InteractActor->SetShowWidgetKey(false);
		}
	}

	ClosestActorsBestIndex = NewBestIndex;
	ClosestActors = NewClosestActors;
}

int32 UDSGameplayAbility_Interact::FindWorstIndex(const TArray<FDSInteractData>& InArray)
{
	int32 WorstIndex = 0;
	for (int32 i = 1; i < InArray.Num(); i++)
	{
		if (InArray[i].Distance > InArray[WorstIndex].Distance)
		{
			WorstIndex = i;
		}
	}

	return WorstIndex;
}

int32 UDSGameplayAbility_Interact::FindBestIndex(const TArray<FDSInteractData>& InArray, const float& InInteractDistance)
{
	int32 BestIndex = INDEX_NONE;
	float ShortestDistance = InInteractDistance;

	for (int32 i = 0; i < InArray.Num(); i++)
	{
		if (ShortestDistance > InArray[i].Distance)
		{
			BestIndex = i;
			ShortestDistance = InArray[i].Distance;
		}
	}

	return BestIndex;
}

void UDSGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCanceled = true;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCanceled);

		return;
	}
	
	InteractionActor = ClosestActors[ClosestActorsBestIndex].InteractActor;
	if (!IsValid(InteractionActor))
	{
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCanceled = true;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCanceled);

		return;
	}

	InteractionActor->StartInteract(GetDSCharacterFromActorInfo());
	
	if (!InteractionActor->GetShouldHold())
	{
		ClosestActors.RemoveAt(ClosestActorsBestIndex);
		ClosestActorsBestIndex = INDEX_NONE;
		
		constexpr bool bReplicateEndAbility = true;
		constexpr bool bWasCanceled = false;
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCanceled);

		return;
	}
	
	bIsActive = true;
	
	UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, InteractionActor->GetHoldTime());
	DelayTask->OnFinish.AddDynamic(this, &UDSGameplayAbility_Interact::OnDelayComplete);
	DelayTask->ReadyForActivation();
	
	UAbilityTask_WaitInputRelease* InputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, true);
	InputReleaseTask->OnRelease.AddDynamic(this, &UDSGameplayAbility_Interact::OnInputRelease);
	InputReleaseTask->ReadyForActivation();
}

void UDSGameplayAbility_Interact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	InteractionActor = nullptr;
}
