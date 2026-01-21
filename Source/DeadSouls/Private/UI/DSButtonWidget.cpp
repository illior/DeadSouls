// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/DSButtonWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UDSButtonWidget::SetState(EDSButtonState NewState)
{
	switch (NewState)
	{
	case EDSButtonState::Base:
		ButtonImage->SetBrush(BaseBrush);
		break;
	case EDSButtonState::Hovered:
		ButtonImage->SetBrush(HoveredBrush);
		break;
	case EDSButtonState::Pressed:
		ButtonImage->SetBrush(PressedBrush);
		break;
	case EDSButtonState::Disabled:
		ButtonImage->SetBrush(DisabledBrush);
		break;
	}

	ReceiveStateChanged(NewState);

	State = NewState;
}

EDSButtonState UDSButtonWidget::GetState() const
{
	return State;
}

void UDSButtonWidget::Click()
{
	OnClicked.Broadcast(this);
}

void UDSButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(ButtonText))
	{
		ButtonText->SetText(Text);
	}

	if (IsValid(ButtonImage))
	{
		ButtonImage->SetBrush(BaseBrush);
	}
}

void UDSButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	check(ButtonImage);
	check(ButtonText);
}

FReply UDSButtonWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (State != EDSButtonState::Disabled && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		SetState(EDSButtonState::Pressed);

		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UDSButtonWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (State == EDSButtonState::Pressed && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		SetState(EDSButtonState::Hovered);
		OnClicked.Broadcast(this);

		return FReply::Unhandled();
	}

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UDSButtonWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (State == EDSButtonState::Disabled)
	{
		return;
	}

	if (OnHovered.IsBound())
	{
		OnHovered.Execute(this);
	}
	else
	{
		SetState(EDSButtonState::Hovered);
	}

	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UDSButtonWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (State == EDSButtonState::Disabled)
	{
		return;
	}

	if (!OnHovered.IsBound())
	{
		SetState(EDSButtonState::Base);
	}
	else if (State == EDSButtonState::Pressed)
	{
		SetState(EDSButtonState::Hovered);
	}
	

	Super::NativeOnMouseLeave(InMouseEvent);
}
