// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/Public/DSItemData.h"
#include "Components/DSInventoryComponent.h"
#include "Items/DSBaseItem.h"

FText UDSItemData::GetItemName() const
{
	return IsValid(Item) ? Item->GetDisplayName() : FText::GetEmpty();
}

FText UDSItemData::GetItemDescription() const
{
	return IsValid(Item) ? Item->GetDisplayDescription() : FText::GetEmpty();
}

bool UDSItemData::GetItemCanStack() const
{
	return IsValid(Item) ? Item->GetCanStack() : false;
}

int32 UDSItemData::GetItemMaxStackAmount() const
{
	return IsValid(Item) ? Item->GetMaxStackAmount() : 1;
}

int32 UDSItemData::GetItemSize() const
{
	return IsValid(Item) ? Item->GetSize() : 1;
}

UTexture2D* UDSItemData::GetItemIcon() const
{
	return IsValid(Item) ? Item->GetIcon() : nullptr;
}

TOptional<FInstancedStruct> UDSItemData::GetItemProperty(const UScriptStruct* InScriptStruct) const
{
	return IsValid(Item) ? Item->GetProperty(InScriptStruct) : TOptional<FInstancedStruct>();
}

bool UDSItemData::ItemHasProperty(const UScriptStruct* InScriptStruct) const
{
	return IsValid(Item) ? Item->HasProperty(InScriptStruct) : false;
}

bool UDSItemData::SetPosition(const FIntPoint InPosition)
{
	UDSInventoryComponent* InventoryComponent = Cast<UDSInventoryComponent>(GetOuter());
	if (IsValid(InventoryComponent))
	{
		return InventoryComponent->TrySetItemPosition(this, InPosition);
	}
	else
	{
		Position = InPosition;
		OnItemUpdated.Broadcast(this);
		
		return true;
	}
}

void UDSItemData::Initialize(UDSBaseItem* InItem, int32 InCount, FIntPoint InPosition)
{
	if (IsValid(InItem))
	{
		Item = InItem;
		Count = FMath::Clamp(InCount, 1, Item->GetMaxStackAmount());
		Position = InPosition;
	}
}

bool UDSItemData::ContainsPoint(FIntPoint InPosition) const
{
	return InPosition == Position || (GetItemSize() == 2 && FIntPoint(Position.X + 1, Position.Y) == InPosition);
}

bool UDSItemData::IsFull() const
{
	return IsValid(Item) ? Count == Item->GetMaxStackAmount() : false;
}

bool UDSItemData::CanDrop() const
{
	return true;
}

FString UDSItemData::ToString() const
{
	return FString::Printf(TEXT("Name: %s, Position: %s, Count: %i"),
		*Item->GetName(),
		*Position.ToString(),
		Count);
}

#if WITH_EDITOR
void UDSItemData::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UObject::PostEditChangeProperty(PropertyChangedEvent);
	
	if (PropertyChangedEvent.GetMemberPropertyName().IsEqual(TEXT("Count")) && IsValid(Item))
	{
		Count = FMath::Min(Count, Item->GetMaxStackAmount());
	}
}
#endif
