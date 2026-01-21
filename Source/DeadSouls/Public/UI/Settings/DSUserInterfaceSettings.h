// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DSUserInterfaceSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "User Interface"))
class DEADSOULS_API UDSUserInterfaceSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UDSUserInterfaceSettings(const FObjectInitializer& ObjectInitializer);
	
	TSoftObjectPtr<UTexture2D> GetTextureForKey(const FKey InKey) const;
protected:
	UPROPERTY(EditDefaultsOnly, Config)
	TMap<FKey, TSoftObjectPtr<UTexture2D>> KeyToTextureMap;
};
