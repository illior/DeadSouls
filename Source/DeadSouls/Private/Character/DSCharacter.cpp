// Fill out your copyright notice in the Description page of Project Settings.

#include "DeadSouls/Public/Character/DSCharacter.h"
#include "Player/DSPlayerState.h"

#include "AbilitySystem/DSAbilitySystemComponent.h"
#include "Components/DSCharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Animation/DSAnimInstance.h"

#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

ADSCharacter::ADSCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<UDSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// SpringArmComponent
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
	SpringArmComponent->TargetArmLength = 130.0f;
	SpringArmComponent->SocketOffset = FVector(0.0f, 30.0f, 30.0f);

	SpringArmComponent->bUsePawnControlRotation = true;

	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 40.0f;
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->CameraRotationLagSpeed = 30.0f;

	// CameraComponent
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
}

USpringArmComponent* ADSCharacter::GetSpringArmComponent() const
{
	return SpringArmComponent;
}

UCameraComponent* ADSCharacter::GetCameraComponent() const
{
	return CameraComponent;
}

UAbilitySystemComponent* ADSCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ADSCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	ADSPlayerState* PS = GetPlayerState<ADSPlayerState>();
	if (IsValid(PS))
	{
		AbilitySystemComponent = CastChecked<UDSAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		if (UDSCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement<UDSCharacterMovementComponent>())
		{
			CharacterMovementComponent->InitializeWithAbilitySystem(AbilitySystemComponent);
		}

		if (USkeletalMeshComponent* SkeletalMeshComponent = GetMesh())
		{
			if (UDSAnimInstance* AnimInstance = Cast<UDSAnimInstance>(SkeletalMeshComponent->GetAnimInstance()))
			{
				AnimInstance->InitializeWithAbilitySystem(AbilitySystemComponent);
			}
		}
	}
}

void ADSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADSCharacter::InputLook);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADSCharacter::InputMove);
		
		for (const TPair<TObjectPtr<UInputAction>, FGameplayTag>& Elem : InputBindings)
		{
			EnhancedInputComponent->BindAction(Elem.Key, ETriggerEvent::Started, this, &ADSCharacter::InputAbilityPress);
			EnhancedInputComponent->BindAction(Elem.Key, ETriggerEvent::Completed, this, &ADSCharacter::InputAbilityRelease);
		}
	}
}

void ADSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADSCharacter::InputLook(const FInputActionInstance& Value)
{
	FVector2D LookAxisVector = Value.GetValue().Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ADSCharacter::InputMove(const FInputActionInstance& Value)
{
	if(!IsValid(Controller))
	{
		return;
	}
	
	FVector2D MovementVector = Value.GetValue().Get<FVector2D>();
	FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);

	FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
	FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ADSCharacter::InputAbilityPress(const FInputActionInstance& Value)
{
	if (IsValid(AbilitySystemComponent))
	{
		FGameplayTag* InputTag = InputBindings.Find(Value.GetSourceAction());
		if (InputTag != nullptr)
		{
			AbilitySystemComponent->PressInputTag(*InputTag);
		}
	}
}

void ADSCharacter::InputAbilityRelease(const FInputActionInstance& Value)
{
	if (IsValid(AbilitySystemComponent))
	{
		FGameplayTag* InputTag = InputBindings.Find(Value.GetSourceAction());
		if (InputTag != nullptr)
		{
			AbilitySystemComponent->ReleaseInputTag(*InputTag);
		}
	}
}
