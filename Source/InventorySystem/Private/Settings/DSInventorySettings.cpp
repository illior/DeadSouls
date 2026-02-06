// Fill out your copyright notice in the Description page of Project Settings.

#include "Settings/DSInventorySettings.h"

UDSInventorySettings::UDSInventorySettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CategoryName = TEXT("Project");
}

TOptional<FDSCraftRecipe> UDSInventorySettings::GetCraftRecipe(UDSBaseItem* FirstItem, UDSBaseItem* SecondItem) const
{
	TOptional<FDSCraftRecipe> Result;
	
	for (const FDSCraftRecipe& Recipe : Crafts)
	{
		if ((Recipe.FirstItem == FirstItem && Recipe.SecondItem == SecondItem) ||
			(Recipe.FirstItem == SecondItem && Recipe.SecondItem == FirstItem))
		{
			Result = TOptional(Recipe);

			return Result;
		}
	}
	
	return Result;
}
