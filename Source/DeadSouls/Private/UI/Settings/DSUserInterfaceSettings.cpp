// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Settings/DSUserInterfaceSettings.h"

UDSUserInterfaceSettings::UDSUserInterfaceSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CategoryName = TEXT("Project");
}

TSoftObjectPtr<UTexture2D> UDSUserInterfaceSettings::GetTextureForKey(const FKey InKey) const
{
	const TSoftObjectPtr<UTexture2D>* Texture = KeyToTextureMap.Find(InKey);
	if (Texture != nullptr)
	{
		return *Texture;
	}
	
	return nullptr;
}
