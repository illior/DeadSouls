// Fill out your copyright notice in the Description page of Project Settings.

#include "DSWeaponData.h"

#include "VectorUtil.h"
#include "Items/DSBaseItem.h"

int32 UDSWeaponData::GetItemMaxAmmoInClip() const
{
	return GetCachedWeaponItem() != nullptr ? GetCachedWeaponItem()->MaxAmmoInClip : 0;
}

EDSAmmoType UDSWeaponData::GetItemAmmoType() const
{
	return GetCachedWeaponItem() != nullptr ? GetCachedWeaponItem()->AmmoType : EDSAmmoType::Pistol;
}

void UDSWeaponData::Initialize(UDSBaseItem* InItem, int32 InCount, FIntPoint InPosition)
{
	if (IsValid(InItem) && InItem->HasProperty(FDSWeaponItemData::StaticStruct()))
	{
		Item = InItem;
		Count = 1;
		CurrentAmmoInClip = FMath::Clamp(InCount, 1, Item->GetMaxStackAmount());
		Position = InPosition;
	}
}

bool UDSWeaponData::IsFull() const
{
	return GetCachedWeaponItem() != nullptr ? GetCachedWeaponItem()->MaxAmmoInClip == CurrentAmmoInClip : true;
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

FDSWeaponItemData* UDSWeaponData::GetCachedWeaponItem() const
{
	if (CachedWeaponItem != nullptr)
	{
		return CachedWeaponItem;
	}
	
	if (IsValid(Item))
	{
		TOptional<FInstancedStruct> WeaponItem = Item->GetProperty(FDSWeaponItemData::StaticStruct());
		if (WeaponItem.IsSet())
		{
			CachedWeaponItem = const_cast<FDSWeaponItemData*>(WeaponItem->GetPtr<FDSWeaponItemData>());
		}
		
		return CachedWeaponItem;
	}
	
	return nullptr;
}
