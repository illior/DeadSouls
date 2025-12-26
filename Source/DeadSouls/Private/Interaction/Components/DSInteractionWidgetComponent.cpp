// Fill out your copyright notice in the Description page of Project Settings.

#include "Interaction/Components/DSInteractionWidgetComponent.h"
#include "Interaction/Components/DSInteractionWidget.h"

UDSInteractionWidgetComponent::UDSInteractionWidgetComponent()
{
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PostUpdateWork;
	
	SetVisibleFlag(false);
	
	Space = EWidgetSpace::Screen;
	bDrawAtDesiredSize = false;
	Pivot = FVector2D(0.5f, 1.0f);
	DrawSize = FIntPoint(FarWidgetSize.X, FarWidgetSize.Y);
}

void UDSInteractionWidgetComponent::StartHold(const float& InHoldTime)
{
	UUserWidget* InteractWidget = GetUserWidgetObject();
	if (IsValid(InteractWidget) && InteractWidget->Implements<UDSInteractionWidget>())
	{
		IDSInteractionWidget::Execute_StartHold(InteractWidget, InHoldTime);
	}
}

void UDSInteractionWidgetComponent::StopHold()
{
	UUserWidget* InteractWidget = GetUserWidgetObject();
	if (IsValid(InteractWidget) && InteractWidget->Implements<UDSInteractionWidget>())
	{
		IDSInteractionWidget::Execute_StopHold(InteractWidget);
	}
}

void UDSInteractionWidgetComponent::SetDistanceAlpha(const float& InAlpha)
{
	if (DistanceAlpha != InAlpha)
	{
		DistanceAlpha = InAlpha;
		
		SetDrawSize(FMath::Lerp(CloseWidgetSize, FarWidgetSize, DistanceAlpha));
	}
}

void UDSInteractionWidgetComponent::ShowWidget()
{
	if (!bShouldShow)
	{
		bShouldShow = true;
		
		UUserWidget* InteractWidget = GetUserWidgetObject();
		if (IsValid(InteractWidget) && InteractWidget->Implements<UDSInteractionWidget>())
		{
			IDSInteractionWidget::Execute_StartShow(InteractWidget);
		}
	}
}

void UDSInteractionWidgetComponent::ShowWidgetKey()
{
	if (!bShouldShowKey)
	{
		bShouldShowKey = true;
		
		UUserWidget* InteractWidget = GetUserWidgetObject();
		if (IsValid(InteractWidget) && InteractWidget->Implements<UDSInteractionWidget>())
		{
			IDSInteractionWidget::Execute_StartShowKey(InteractWidget);
		}
	}
}

void UDSInteractionWidgetComponent::HideWidget()
{
	if (bShouldShow)
	{
		bShouldShow = false;
		
		UUserWidget* InteractWidget = GetUserWidgetObject();
		if (IsValid(InteractWidget) && InteractWidget->Implements<UDSInteractionWidget>())
		{
			IDSInteractionWidget::Execute_StartHide(InteractWidget);
		}
	}
}

void UDSInteractionWidgetComponent::HideWidgetKey()
{
	if (bShouldShowKey)
	{
		bShouldShowKey = false;
		
		UUserWidget* InteractWidget = GetUserWidgetObject();
		if (IsValid(InteractWidget) && InteractWidget->Implements<UDSInteractionWidget>())
		{
			IDSInteractionWidget::Execute_StartHideKey(InteractWidget);
		}
	}
}

void UDSInteractionWidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	
	UUserWidget* InteractWidget = GetUserWidgetObject();
	if (IsValid(InteractWidget))
	{
		InteractWidget->OnVisibilityChanged.AddDynamic(this, &UDSInteractionWidgetComponent::OnUserWidgetVisibilityChanged);
	}
}

void UDSInteractionWidgetComponent::OnUserWidgetVisibilityChanged(ESlateVisibility InVisibility)
{
	switch (InVisibility)
	{
		case ESlateVisibility::Hidden:
			SetVisibility(false);
			break;
		case ESlateVisibility::Collapsed:
			SetVisibility(false);
			break;
		default:
			SetVisibility(true);
			break;
	}
}
