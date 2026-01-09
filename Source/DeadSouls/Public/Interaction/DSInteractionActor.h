// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DSInteractionActor.generated.h"

class ADSCharacter;
class UDSInteractionWidgetComponent;

UCLASS()
class DEADSOULS_API ADSInteractionActor : public AActor
{
	GENERATED_BODY()

public:
	ADSInteractionActor();

	virtual FVector GetTargetLocation(AActor* RequestedBy = nullptr) const override;
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void SetEnabled(const bool bInEnabled);
	bool GetEnabled() const;
	
	bool GetShouldHold() const { return bShouldHold; };
	float GetHoldTime() const { return HoldTime; };
	
	void SetDistanceAlpha(const float& InAlpha);
	void SetShowWidget(const bool& bInShow);
	void SetShowWidgetKey(const bool& bInShowKey);
	
	void StartInteract(ADSCharacter* InCharacter);
	void StopInteract(const float& InHoldTime);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "DeadSouls: Interaction")
	bool bEnabled = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeadSouls: Interaction")
	bool bReusable = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeadSouls: Interaction", meta = (EditCondition = "bReusable", ClampMin = "0", UIMin = "0"))
	float CooldownTime = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeadSouls: Interaction")
	bool bShouldHold = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeadSouls: Interaction", meta = (EditCondition = "bShouldHold", ClampMin = "0", UIMin = "0"))
	float HoldTime = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeadSouls: Interaction", meta = (EditCondition = "bShouldHold", ClampMin = "0", UIMin = "0"))
	float CooldownTimeIfCanceled = 0.5f;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ADSCharacter> Character;
	
	FTimerHandle CooldownTimer;
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="Interact", meta = (ScriptName = "Interact"))
	void BP_Interact();
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnSetEnabled", meta = (ScriptName = "OnSetEnabled"))
	void BP_SetEnabled(bool bInEnabled);
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="CancelHold", meta = (ScriptName = "CancelHold"))
	void BP_CancelHold();
	UFUNCTION(BlueprintImplementableEvent, DisplayName="CompleteHold", meta = (ScriptName = "CompleteHold"))
	void BP_CompleteHold();
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDSInteractionWidgetComponent> WidgetComponent;
};
