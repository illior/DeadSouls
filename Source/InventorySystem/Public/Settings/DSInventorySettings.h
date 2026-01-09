// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DSInventorySettings.generated.h"

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Inventory System"))
class INVENTORYSYSTEM_API UDSInventorySettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UDSInventorySettings(const FObjectInitializer& ObjectInitializer);
};
