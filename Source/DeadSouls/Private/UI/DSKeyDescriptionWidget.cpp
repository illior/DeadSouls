// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/DSKeyDescriptionWidget.h"
#include "UI/Settings/DSUserInterfaceSettings.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "EnhancedInputSubsystems.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

FName UDSKeyDescriptionWidget::GetMappingName() const
{
	if (KeyMapping != nullptr)
	{
		return KeyMapping->GetMappingName();
	}
	
	return FName();
}

const UInputAction* UDSKeyDescriptionWidget::GetAssociatedInputAction() const
{
	if (KeyMapping != nullptr)
	{
		return KeyMapping->GetAssociatedInputAction();
	}
	
	return nullptr;
}

void UDSKeyDescriptionWidget::SetDescription(const FText& InText) const
{
	DescriptionText->SetText(InText);
}

void UDSKeyDescriptionWidget::OnMappingsRebuilt()
{
	if (KeyMapping != nullptr)
	{
		const UDSUserInterfaceSettings* UserInterfaceSettings = GetDefault<UDSUserInterfaceSettings>();
		if (IsValid(UserInterfaceSettings))
		{
			KeyImage->SetBrushFromSoftTexture(UserInterfaceSettings->GetTextureForKey(KeyMapping->GetCurrentKey()));
		}
		
		return;
	}
	
	const UEnhancedInputLocalPlayerSubsystem* EISubsystem = GetOwningLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (IsValid(EISubsystem))
	{
		const UEnhancedInputUserSettings* UserSettings = EISubsystem->GetUserSettings();
		if (IsValid(UserSettings))
		{
			UEnhancedPlayerMappableKeyProfile* KeyProfile = UserSettings->GetActiveKeyProfile();
			if (IsValid(KeyProfile))
			{
				FMapPlayerKeyArgs Args = {};
				Args.MappingName = MappingName;
				Args.Slot = EPlayerMappableKeySlot::First;

				KeyMapping = KeyProfile->FindKeyMapping(Args);
				if (KeyMapping != nullptr)
				{
					const UDSUserInterfaceSettings* UserInterfaceSettings = GetDefault<UDSUserInterfaceSettings>();
					if (IsValid(UserInterfaceSettings))
					{
						KeyImage->SetBrushFromSoftTexture(UserInterfaceSettings->GetTextureForKey(KeyMapping->GetCurrentKey()));
					}
				}
			}
		}
	}
}

void UDSKeyDescriptionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	check(KeyImage);
	check(DescriptionText);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningLocalPlayer());
	if (IsValid(Subsystem))
	{
		Subsystem->ControlMappingsRebuiltDelegate.AddDynamic(this, &UDSKeyDescriptionWidget::OnMappingsRebuilt);
	}
	
	OnMappingsRebuilt();
}
