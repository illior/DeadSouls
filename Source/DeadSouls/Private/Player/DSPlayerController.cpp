// Fill out your copyright notice in the Description page of Project Settings.

#include "DeadSouls/Public/Player/DSPlayerController.h"

#include "EnhancedInputSubsystems.h"

ADSPlayerController::ADSPlayerController()
{
	
}

void ADSPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem != nullptr)
	{
		FModifyContextOptions ModifyContextOptions;
		ModifyContextOptions.bNotifyUserSettings = true;

		Subsystem->AddMappingContext(DefaultMappingContext, 0, ModifyContextOptions);
	}
}
