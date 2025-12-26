// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DSInteractionWidgetComponent.generated.h"

UCLASS()
class DEADSOULS_API UDSInteractionWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UDSInteractionWidgetComponent();
	
	void StartHold(const float& InHoldTime);
	void StopHold();
	
	void SetDistanceAlpha(const float& InAlpha);
	
	void ShowWidget();
	void ShowWidgetKey();
	void HideWidget();
	void HideWidgetKey();
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnUserWidgetVisibilityChanged(ESlateVisibility InVisibility);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeadSouls: Interaction")
	FVector2D CloseWidgetSize = FVector2D(60, 120);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeadSouls: Interaction")
	FVector2D FarWidgetSize = FVector2D(30, 60);
	
	float DistanceAlpha;

	bool bShouldShow = false;
	bool bShouldShowKey = false;
};
