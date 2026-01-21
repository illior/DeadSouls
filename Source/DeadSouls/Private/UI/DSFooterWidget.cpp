// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/DSFooterWidget.h"
#include "UI/DSKeyDescriptionWidget.h"

void UDSFooterWidget::SetDescriptionByName(FName InMappingName, FText InDescription)
{
	for (UDSKeyDescriptionWidget* KeyDescription : KeyDescriptions)
	{
		if (KeyDescription->GetMappingName() == InMappingName)
		{
			if (InDescription.IsEmpty())
			{
				KeyDescription->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				KeyDescription->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				KeyDescription->SetDescription(InDescription);
			}
			
			return;
		}
	}
}

void UDSFooterWidget::SetDescriptionByAction(UInputAction* InAction, FText InDescription)
{
	for (UDSKeyDescriptionWidget* KeyDescription : KeyDescriptions)
	{
		if (KeyDescription->GetAssociatedInputAction() == InAction)
		{
			if (InDescription.IsEmpty())
			{
				KeyDescription->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				KeyDescription->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				KeyDescription->SetDescription(InDescription);
			}
			
			return;
		}
	}
}

void UDSFooterWidget::ClearAllDescriptions()
{
	for (UDSKeyDescriptionWidget* KeyDescription : KeyDescriptions)
	{
		KeyDescription->SetVisibility(ESlateVisibility::Collapsed);
	}
}
