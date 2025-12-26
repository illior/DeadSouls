// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSGameplayAbility.h"
#include "DSGameplayAbility_Interact.generated.h"

class ADSInteractionActor;

USTRUCT()
struct FDSInteractData
{
	GENERATED_BODY()
	
public:
	FDSInteractData();
	FDSInteractData(ADSInteractionActor* InInteractActor, const float InDistance);
	
	UPROPERTY()
	TObjectPtr<ADSInteractionActor> InteractActor;
	
	UPROPERTY()
	float Distance;
	
	bool operator==(const FDSInteractData& Other) const
	{
		return InteractActor == Other.InteractActor;
	}

	bool operator!=(const FDSInteractData& other) const
	{
		return !(*this == other);
	}
};

FORCEINLINE uint32 GetTypeHash(const FDSInteractData& Thing)
{
	return GetTypeHash(Thing.InteractActor);
}

UCLASS()
class DEADSOULS_API UDSGameplayAbility_Interact : public UDSGameplayAbility
{
	GENERATED_BODY()
	
public:
	UDSGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer);
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DeadSouls: Interaction")
	float InteractDistance = 150.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DeadSouls: Interaction")
	float InteractSearchDistance = 600.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeadSouls: Interact")
	float CloseInteractAngle = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeadSouls: Interact")
	float FarInteractAngle = 15.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeadSouls: Interact")
	int32 MaxShowWidgets = 3;
	
	UPROPERTY()
	TArray<FDSInteractData> ClosestActors;
	int32 ClosestActorsBestIndex = INDEX_NONE;
	
	UPROPERTY()
	TObjectPtr<ADSInteractionActor> InteractionActor;
	
	UFUNCTION()
	void OnDelayComplete();
	UFUNCTION()
	void OnInputRelease(float TimeHeld);
	
	virtual void AbilityTick(float DeltaTime) override;
	
	void SearchInteractionObjects();
	
	TArray<FDSInteractData> GetAllInteractObjects() const;
	TArray<FDSInteractData> GetClosestInteractObjects(const TArray<FDSInteractData>& InArray) const;
	
	void UpdateInteractionData(const TArray<FDSInteractData>& NewClosestActors);

	static int32 FindWorstIndex(const TArray<FDSInteractData>& InArray);
	static int32 FindBestIndex(const TArray<FDSInteractData>& InArray, const float& InInteractDistance);
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
