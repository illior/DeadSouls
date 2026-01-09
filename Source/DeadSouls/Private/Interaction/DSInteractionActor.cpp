// Fill out your copyright notice in the Description page of Project Settings.

#include "Interaction/DSInteractionActor.h"
#include "Interaction/Components/DSInteractionWidgetComponent.h"
#include "Character/DSCharacter.h"

ADSInteractionActor::ADSInteractionActor()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(SceneComponent);
	
	WidgetComponent = CreateDefaultSubobject<UDSInteractionWidgetComponent>("Widget");
	WidgetComponent->SetupAttachment(GetRootComponent());
}

FVector ADSInteractionActor::GetTargetLocation(AActor* RequestedBy) const
{
	if (IsValid(WidgetComponent))
	{
		return WidgetComponent->GetComponentLocation();
	}
	
	return Super::GetTargetLocation(RequestedBy);
}

void ADSInteractionActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (!bEnabled)
	{
		SetEnabled(false);
	}
}

void ADSInteractionActor::SetEnabled(const bool bInEnabled)
{
	bEnabled = bInEnabled;
	
	if (bEnabled)
	{
		GetWorldTimerManager().ClearTimer(CooldownTimer);
	}
	else
	{
		SetShowWidget(false);
		SetShowWidgetKey(false);
	}
	
	BP_SetEnabled(bEnabled);
}

bool ADSInteractionActor::GetEnabled() const
{
	return bEnabled;
}

void ADSInteractionActor::SetDistanceAlpha(const float& InAlpha)
{
	if (IsValid(WidgetComponent))
	{
		WidgetComponent->SetDistanceAlpha(InAlpha);
	}
}

void ADSInteractionActor::SetShowWidget(const bool& bInShow)
{
	if (IsValid(WidgetComponent))
	{
		bInShow ? WidgetComponent->ShowWidget() : WidgetComponent->HideWidget();
	}
}

void ADSInteractionActor::SetShowWidgetKey(const bool& bInShowKey)
{
	if (IsValid(WidgetComponent))
	{
		bInShowKey ? WidgetComponent->ShowWidgetKey() : WidgetComponent->HideWidgetKey();
	}
}

void ADSInteractionActor::StartInteract(ADSCharacter* InCharacter)
{
	if (!IsValid(InCharacter))
	{
		return;
	}
	
	Character = InCharacter;
	
	BP_Interact();
	
	if (!bShouldHold)
	{
		Character = nullptr;
		SetEnabled(false);
		
		if (bReusable)
		{
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "SetEnabled", true);

			GetWorldTimerManager().SetTimer(CooldownTimer, Delegate, CooldownTime, false);
		}
	}
	else
	{
		if (IsValid(WidgetComponent))
		{
			WidgetComponent->StartHold(HoldTime);
		}
	}
}

void ADSInteractionActor::StopInteract(const float& InHoldTime)
{
	if (InHoldTime < HoldTime)
	{
		BP_CancelHold();
		
		if (IsValid(WidgetComponent))
		{
			WidgetComponent->StopHold();
		}
		
		Character = nullptr;
		if (CooldownTimeIfCanceled > 0.0f)
		{
			SetEnabled(false);
			
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "SetEnabled", true);

			GetWorldTimerManager().SetTimer(CooldownTimer, Delegate, CooldownTimeIfCanceled, false);
		}
	}
	else
	{
		BP_CompleteHold();
	
		if (IsValid(WidgetComponent))
		{
			WidgetComponent->StopHold();
		}
		
		Character = nullptr;
		SetEnabled(false);
	
		if (bReusable)
		{
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "SetEnabled", true);

			GetWorldTimerManager().SetTimer(CooldownTimer, Delegate, CooldownTime, false);
		}
	}
}
