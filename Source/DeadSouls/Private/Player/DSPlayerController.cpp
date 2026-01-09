// Fill out your copyright notice in the Description page of Project Settings.

#include "DeadSouls/Public/Player/DSPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ADSPlayerController::ADSPlayerController()
{
	
}

void ADSPlayerController::PauseGame()
{
	if (IsPaused())
	{
		return;
	}
	
	SetPause(true);

	FInputModeGameAndUI InputMode = FInputModeGameAndUI();
	InputMode.SetHideCursorDuringCapture(false);

	SetInputMode(InputMode);
	bShowMouseCursor = true;
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem != nullptr)
	{
		FModifyContextOptions ModifyContextOptions;
		ModifyContextOptions.bNotifyUserSettings = true;

		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(PauseMappingContext, 0, ModifyContextOptions);
	}
}

void ADSPlayerController::ResumeGame()
{
	if (!IsPaused())
	{
		return;
	}
	
	SetPause(false);
	
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem != nullptr)
	{
		FModifyContextOptions ModifyContextOptions;
		ModifyContextOptions.bNotifyUserSettings = true;

		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(DefaultMappingContext, 0, ModifyContextOptions);
	}
	
	//
}

void ADSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(OpenPauseMenuAction, ETriggerEvent::Started, this, &ADSPlayerController::InputPause);
		EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &ADSPlayerController::InputInventory);
	}
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

void ADSPlayerController::InputPause(const FInputActionInstance& Value)
{
	
}

void ADSPlayerController::InputInventory(const FInputActionInstance& Value)
{
	
}
