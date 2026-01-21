// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/DSGameHUD.h"
#include "UI/DSInputWidget.h"

void ADSGameHUD::OpenInventoryWidget()
{
	OpenWidget(InventoryWidget);
}

void ADSGameHUD::OpenPauseWidget()
{
	OpenWidget(PauseWidget);
}

void ADSGameHUD::OpenWidget(UDSInputWidget* WidgetToShow)
{
	if (IsValid(CurrentInputWidget))
	{
		CurrentInputWidget->Close();
	}
	
	if (IsValid(WidgetToShow))
	{
		CurrentInputWidget = WidgetToShow;
		CurrentInputWidget->Open();
	}
}

void ADSGameHUD::CloseCurrentWidgets()
{
	if (IsValid(CurrentInputWidget))
	{
		CurrentInputWidget->Close();
		CurrentInputWidget = nullptr;
	}
}

void ADSGameHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(InventoryWidgetClass))
	{
		InventoryWidget = CreateWidget<UDSInputWidget>(PlayerOwner, InventoryWidgetClass, FName(TEXT("InventoryWidget")));
		InventoryWidget->AddToViewport();
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		InventoryWidget->DisableInput();
	}
	
	if (IsValid(PauseWidgetClass))
	{
		PauseWidget = CreateWidget<UDSInputWidget>(PlayerOwner, PauseWidgetClass, FName(TEXT("PauseWidget")));
		PauseWidget->AddToViewport();
		PauseWidget->SetVisibility(ESlateVisibility::Collapsed);
		PauseWidget->DisableInput();
	}
}
