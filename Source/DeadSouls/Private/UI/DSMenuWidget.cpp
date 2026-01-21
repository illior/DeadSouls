// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/DSMenuWidget.h"
#include "UI/DSButtonWidget.h"
#include "UI/Interfaces/DSSlideButton.h"

void UDSMenuWidget::AddButton(UDSButtonWidget* InButton)
{
	if (IsValid(InButton) && !Buttons.Contains(InButton))
	{
		if(InButton->OnHovered.IsBound())
		{
			InButton->OnHovered.Unbind();
		}

		InButton->OnHovered.BindUObject(this, &UDSMenuWidget::SetButtonSelected);
		Buttons.Add(InButton);
	}
}

void UDSMenuWidget::AddButtons(TArray<UDSButtonWidget*> InButtons)
{
	for (int32 i = 0; i < InButtons.Num(); i++)
	{
		if (IsValid(InButtons[i]) && !Buttons.Contains(InButtons[i]))
		{
			if (InButtons[i]->OnHovered.IsBound())
			{
				InButtons[i]->OnHovered.Unbind();
			}

			InButtons[i]->OnHovered.BindUObject(this, &UDSMenuWidget::SetButtonSelected);
			Buttons.Add(InButtons[i]);
		}
	}
}

void UDSMenuWidget::SetButtons(TArray<UDSButtonWidget*> InButtons)
{
	Buttons.Empty(InButtons.Num());

	for (int32 i = 0; i < InButtons.Num(); i++)
	{
		if (IsValid(InButtons[i]) && !Buttons.Contains(InButtons[i]))
		{
			if (InButtons[i]->OnHovered.IsBound())
			{
				InButtons[i]->OnHovered.Unbind();
			}

			InButtons[i]->OnHovered.BindUObject(this, &UDSMenuWidget::SetButtonSelected);
			Buttons.Add(InButtons[i]);
		}
	}
}

void UDSMenuWidget::CloseMenu()
{
	OnCloseMenu.Broadcast(this);
}

void UDSMenuWidget::InFocus_Implementation()
{
	SetButtonState(CurrentIndex, EDSButtonState::Hovered);
}

void UDSMenuWidget::OutFocus_Implementation()
{
	SetButtonState(CurrentIndex, EDSButtonState::Base);
	CurrentIndex = 0;
}

void UDSMenuWidget::Confirm_Implementation()
{
	if (Buttons.IsValidIndex(CurrentIndex))
	{
		Buttons[CurrentIndex]->Click();
	}
}

void UDSMenuWidget::MoveVertical_Implementation(int32 Offset)
{
	int32 NextIndex = FMath::Clamp(CurrentIndex + Offset, 0, Buttons.Num() - 1);
	if (NextIndex == CurrentIndex)
	{
		return;
	}

	if (Buttons[NextIndex]->GetState() == EDSButtonState::Disabled)
	{
		if (NextIndex != Buttons.Num() - 1 && NextIndex != 0)
		{
			MoveVertical(Offset + 1 * FMath::Sign(Offset));
		}

		return;
	}

	Buttons[CurrentIndex]->SetState(EDSButtonState::Base);
	Buttons[NextIndex]->SetState(EDSButtonState::Hovered);

	CurrentIndex = NextIndex;
}

void UDSMenuWidget::MoveHorizontal_Implementation(int32 Offset)
{
	if (Buttons.IsValidIndex(CurrentIndex) && Buttons[CurrentIndex]->Implements<UDSSlideButton>())
	{
		IDSSlideButton::Execute_HorizontalMove(Buttons[CurrentIndex], Offset);
	}
}

void UDSMenuWidget::SetButtonSelected(UDSButtonWidget* InButton)
{
	int32 ButtonIndex = Buttons.Find(InButton);
	if (ButtonIndex != INDEX_NONE && ButtonIndex != CurrentIndex)
	{
		SetButtonState(CurrentIndex, EDSButtonState::Base);
		SetButtonState(ButtonIndex, EDSButtonState::Hovered);

		CurrentIndex = ButtonIndex;
	}
}

void UDSMenuWidget::SetButtonState(int32 Index, EDSButtonState InState)
{
	if (Buttons.IsValidIndex(Index))
	{
		Buttons[Index]->SetState(InState);
	}
}
