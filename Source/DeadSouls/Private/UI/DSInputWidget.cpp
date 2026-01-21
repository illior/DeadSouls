// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/DSInputWidget.h"
#include "UI/DSFooterWidget.h"

UDSFooterWidget* UDSInputWidget::GetFooterWidget() const
{
	return Footer;
}

void UDSInputWidget::Open()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	InputComponent ? RegisterInputComponent() : InitializeInputComponent();
	
	OnOpen();
}

void UDSInputWidget::Close()
{
	SetVisibility(ESlateVisibility::Collapsed);
	DisableInput();
	
	OnClose();
}

void UDSInputWidget::DisableInput()
{
	UnregisterInputComponent();
	
	OnDisableInput();
}

void UDSInputWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	check(Footer);
}
