// Fill out your copyright notice in the Description page of Project Settings.

#include "DSWeaponData.h"
#include "Components/DSInventoryComponent.h"
#include "Items/DSBaseItem.h"

int32 UDSWeaponData::GetItemMaxAmmoInClip() const
{
	if (IsValid(Item))
	{
		TOptional<FInstancedStruct> WeaponItem = Item->GetProperty(FDSWeaponProperty::StaticStruct());
		
		if (WeaponItem.IsSet())
		{
			return WeaponItem->Get<FDSWeaponProperty>().MaxAmmoInClip;
		}
	}
	
	return 1;
}

EDSAmmoType UDSWeaponData::GetItemAmmoType() const
{
	if (IsValid(Item))
	{
		TOptional<FInstancedStruct> WeaponItem = Item->GetProperty(FDSWeaponProperty::StaticStruct());
		
		if (WeaponItem.IsSet())
		{
			return WeaponItem->Get<FDSWeaponProperty>().AmmoType;
		}
	}
	
	return EDSAmmoType::Pistol;
}

bool UDSWeaponData::SetCount(const int32 InCount)
{
	UDSInventoryComponent* InventoryComponent = Cast<UDSInventoryComponent>(GetOuter());
	if (IsValid(InventoryComponent))
	{
		return InventoryComponent->TrySetItemCount(this, Count);
	}
	else
	{
		CurrentAmmoInClip = FMath::Clamp(InCount, 0, GetItemMaxAmmoInClip());
	
		return true;
	}
}

void UDSWeaponData::Initialize(UDSBaseItem* InItem, int32 InCount, FIntPoint InPosition)
{
	if (IsValid(InItem) && InItem->HasProperty(FDSWeaponProperty::StaticStruct(), true))
	{
		Item = InItem;
		Count = 1;
		CurrentAmmoInClip = FMath::Clamp(InCount, 1, GetItemMaxAmmoInClip());
		Position = InPosition;
	}
}

bool UDSWeaponData::IsFull() const
{
	return GetItemMaxAmmoInClip() == CurrentAmmoInClip;
}

bool UDSWeaponData::CanDrop() const
{
	return false;
}

#if WITH_EDITOR
void UDSWeaponData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.GetMemberPropertyName().IsEqual(TEXT("Item")) && IsValid(Item) && !Item->HasProperty(FDSWeaponProperty::StaticStruct(), true))
	{
		Item = nullptr;
		return;
	}
	
	if (PropertyChangedEvent.GetMemberPropertyName().IsEqual(TEXT("CurrentAmmoInClip")) && IsValid(Item))
	{
		TOptional<FInstancedStruct> WeaponItem = Item->GetProperty(FDSWeaponProperty::StaticStruct());
		if (WeaponItem.IsSet())
		{
			const FDSWeaponProperty* WeaponItemData = WeaponItem->GetPtr<FDSWeaponProperty>();
		
			CurrentAmmoInClip = FMath::Clamp(CurrentAmmoInClip, 0, WeaponItemData->MaxAmmoInClip);
		}
	}
}
#endif
