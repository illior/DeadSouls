// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSInventoryTypes.h"
#include "Engine/DeveloperSettings.h"
#include "DSInventorySettings.generated.h"

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Inventory System"))
class INVENTORYSYSTEM_API UDSInventorySettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UDSInventorySettings(const FObjectInitializer& ObjectInitializer);
	
	TOptional<FDSCraftRecipe> GetCraftRecipe(UDSBaseItem* FirstItem, UDSBaseItem* SecondItem) const;
protected:
	UPROPERTY(Config, BlueprintReadOnly, EditDefaultsOnly, Category = "General")
	TArray<FDSCraftRecipe> DefaultCraftRecipes;
	
	UPROPERTY(Config, BlueprintReadOnly, EditDefaultsOnly, Category = "General", meta = (RowType = "DSDocument"))
	TSoftObjectPtr<UDataTable> Documents;
};
 