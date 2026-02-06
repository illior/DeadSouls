// Fill out your copyright notice in the Description page of Project Settings.

#include "DSWeaponData.h"
#include "Items/DSBaseItem.h"

int32 UDSWeaponData::GetItemMaxAmmoInClip() const
{
	return GetCachedWeaponItem().MaxAmmoInClip;
}

EDSAmmoType UDSWeaponData::GetItemAmmoType() const
{
	return GetCachedWeaponItem().AmmoType;
}

void UDSWeaponData::Initialize(UDSBaseItem* InItem, int32 InCount, FIntPoint InPosition)
{
	if (IsValid(InItem) && InItem->HasProperty(FDSWeaponItemData::StaticStruct()))
	{
		Item = InItem;
		Count = 1;
		CurrentAmmoInClip = FMath::Clamp(InCount, 1, GetItemMaxAmmoInClip());
		Position = InPosition;
	}
}

bool UDSWeaponData::IsFull() const
{
	return GetCachedWeaponItem().MaxAmmoInClip == CurrentAmmoInClip;
}

bool UDSWeaponData::CanDrop() const
{
	return false;
}

#if WITH_EDITOR
void UDSWeaponData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.GetMemberPropertyName().IsEqual(TEXT("Item")) && IsValid(Item) && !Item->HasProperty(FDSWeaponItemData::StaticStruct()))
	{
		Item = nullptr;
		return;
	}
	
	if (PropertyChangedEvent.GetMemberPropertyName().IsEqual(TEXT("CurrentAmmoInClip")) && IsValid(Item))
	{
		TOptional<FInstancedStruct> WeaponItem = Item->GetProperty(FDSWeaponItemData::StaticStruct());
		if (WeaponItem.IsSet())
		{
			const FDSWeaponItemData* WeaponItemData = WeaponItem->GetPtr<FDSWeaponItemData>();
		
			CurrentAmmoInClip = FMath::Clamp(CurrentAmmoInClip, 0, WeaponItemData->MaxAmmoInClip);
		}
	}
}
#endif

FDSWeaponItemData UDSWeaponData::GetCachedWeaponItem() const
{
	if (CachedWeaponItem.MaxAmmoInClip != 0)
	{
		return CachedWeaponItem;
	}
	
	if (IsValid(Item))
	{
		TOptional<FInstancedStruct> WeaponItem = Item->GetProperty(FDSWeaponItemData::StaticStruct());
		
		if (WeaponItem.IsSet())
		{
			CachedWeaponItem = WeaponItem->GetMutable<FDSWeaponItemData>();
		}
		
		return CachedWeaponItem;
	}
	
	return FDSWeaponItemData();
}
