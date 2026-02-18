// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/DSInventoryComponent.h"
#include "Settings/DSInventorySettings.h"
#include "Items/DSBaseItem.h"
#include "DSItemData.h"
#include "DSWeaponData.h"

DEFINE_LOG_CATEGORY(LogInventoryComponent);

UDSInventoryComponent::UDSInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

EDSSlotState UDSInventoryComponent::GetSlotState(FIntPoint InPosition) const
{
	if (SlotsMatrix == nullptr ||
		InPosition.X < 0 || InPosition.X >= MaxSize.X ||
		InPosition.Y < 0 || InPosition.Y >= MaxSize.Y)
	{
		return EDSSlotState::Block;
	}

	return SlotsMatrix[InPosition.Y][InPosition.X];
}

UDSItemData* UDSInventoryComponent::GetItemByPosition(FIntPoint InPosition) const
{
	for (int32 i = 0; i < Items.Num(); i++)
	{
		if (Items[i]->ContainsPoint(InPosition))
		{
			return Items[i];
		}
	}

	return nullptr;
}

void UDSInventoryComponent::TryAddDocument(const FDataTableRowHandle InHandle)
{
	FName RowName = InHandle.RowName;
	FDSDocument* Document = InHandle.GetRow<FDSDocument>(TEXT(""));
	if (Document != nullptr && !Documents.Contains(RowName))
	{
		Documents.Add(RowName);
		OnDocumentAdded.Broadcast(RowName, *Document);
	}
}

void UDSInventoryComponent::TryAddNewItem(UDSItemData* InItemData)
{
	if (IsValid(InItemData) && IsValid(InItemData->GetItem()) && !Items.Contains(InItemData))
	{
		OnTryingAddItem.Broadcast(InItemData);
		
		UE_LOG(LogInventoryComponent, Display, TEXT("Try add item %s"), *InItemData->ToString());
	}
}

bool UDSInventoryComponent::TryAddItem(UDSItemData* InItemData)
{
	if (!IsValid(InItemData) || Items.Contains(InItemData))
	{
		return false;
	}
	
	if (InItemData->GetItemSize() == 1 && GetSlotState(InItemData->GetPosition()) == EDSSlotState::Void)
	{
		AddItem(InItemData);
		InItemData->OnItemUpdated.Broadcast(InItemData);
		
		return true;
	}
	
	if (InItemData->GetItemSize() == 2 && GetSlotState(InItemData->GetPosition()) == EDSSlotState::Void && GetSlotState(InItemData->GetPosition() + FIntPoint(1, 0)) == EDSSlotState::Void)
	{
		AddItem(InItemData);
		InItemData->OnItemUpdated.Broadcast(InItemData);
		
		return true;
	}
	
	UDSItemData* ItemInPosition = GetItemByPosition(InItemData->GetPosition());
	if (IsValid(ItemInPosition))
	{
		if (CanCombineItems(ItemInPosition->GetItem(), InItemData->GetItem()))
		{
			CombineItems(ItemInPosition, InItemData);
			
			return InItemData->Count == 0;
		}
		
		if (CanCraftItems(ItemInPosition->GetItem(), InItemData->GetItem()))
		{
			CraftItems(ItemInPosition, InItemData);
			
			InItemData->Count = 0;
			InItemData->OnItemUpdated.Broadcast(InItemData);
			
			return true;
		}
	}
	
	return false;
}

bool UDSInventoryComponent::TryRemoveItem(UDSItemData* InItemData, bool bIsGarbage)
{
	if (Items.Contains(InItemData))
	{
		RemoveItem(InItemData);

		if (bIsGarbage)
		{
			InItemData->MarkAsGarbage();
		}

		return true;
	}

	return false;
}

bool UDSInventoryComponent::TrySetItemPosition(UDSItemData* InItemData, FIntPoint InPosition)
{
	if (!Items.Contains(InItemData))
	{
		return false;
	}
	
	if (InItemData->GetItemSize() == 1)
	{
		if (GetSlotState(InPosition) == EDSSlotState::Void)
		{
			SetSlotState(InItemData->Position, EDSSlotState::Void);
			SetSlotState(InPosition, EDSSlotState::Item);
			InItemData->Position = InPosition;
			
			InItemData->OnItemUpdated.Broadcast(InItemData);
			UE_LOG(LogInventoryComponent, Display, TEXT("Item moved %s"), *InItemData->ToString());
			
			return true;
		}

		UDSItemData* ItemInPosition = GetItemByPosition(InPosition);
		if (IsValid(ItemInPosition) && ItemInPosition != InItemData)
		{
			if (CanCombineItems(InItemData->GetItem(), ItemInPosition->GetItem()))
			{
				CombineItems(ItemInPosition, InItemData);

				return true;
			}

			if (CanCraftItems(InItemData->GetItem(), ItemInPosition->GetItem()))
			{
				CraftItems(ItemInPosition, InItemData);

				return true;
			}

			if (ItemInPosition->GetItemSize() == 1)
			{
				ItemInPosition->Position = InItemData->Position;
				InItemData->Position = InPosition;

				ItemInPosition->OnItemUpdated.Broadcast(ItemInPosition);
				UE_LOG(LogInventoryComponent, Display, TEXT("Item moved %s"), *ItemInPosition->ToString());
				
				InItemData->OnItemUpdated.Broadcast(InItemData);
				UE_LOG(LogInventoryComponent, Display, TEXT("Item moved %s"), *InItemData->ToString());
				
				return true;
			}
		}
	}
	else
	{
		if (GetSlotState(InPosition) == EDSSlotState::Void && GetSlotState(InPosition + FIntPoint(1, 0)) == EDSSlotState::Void)
		{
			SetSlotState(InItemData->Position, EDSSlotState::Void);
			SetSlotState(InItemData->Position + FIntPoint(1, 0), EDSSlotState::Void);
			SetSlotState(InPosition, EDSSlotState::Item);
			SetSlotState(InPosition + FIntPoint(1, 0), EDSSlotState::Item);
			InItemData->Position = InPosition;
			
			InItemData->OnItemUpdated.Broadcast(InItemData);
			UE_LOG(LogInventoryComponent, Display, TEXT("Item moved %s"), *InItemData->ToString());

			return true;
		}

		if (InItemData->Position + FIntPoint(1, 0) == InPosition && GetSlotState(InPosition + FIntPoint(1, 0)) == EDSSlotState::Void)
		{
			SetSlotState(InItemData->Position, EDSSlotState::Void);
			SetSlotState(InPosition + FIntPoint(1, 0), EDSSlotState::Item);
			InItemData->Position = InPosition;
			
			InItemData->OnItemUpdated.Broadcast(InItemData);
			UE_LOG(LogInventoryComponent, Display, TEXT("Item moved %s"), *InItemData->ToString());

			return true;
		}

		if (InItemData->Position - FIntPoint(1, 0) == InPosition && GetSlotState(InPosition) == EDSSlotState::Void)
		{
			SetSlotState(InItemData->Position + FIntPoint(1, 0), EDSSlotState::Void);
			SetSlotState(InPosition, EDSSlotState::Item);
			InItemData->Position = InPosition;
			
			InItemData->OnItemUpdated.Broadcast(InItemData);
			UE_LOG(LogInventoryComponent, Display, TEXT("Item moved %s"), *InItemData->ToString());

			return true;
		}

		UDSItemData* ItemInPosition = GetItemByPosition(InPosition);
		if (IsValid(ItemInPosition) && ItemInPosition->GetItemSize() == 2 && ItemInPosition != InItemData)
		{
			ItemInPosition->Position = InItemData->Position;
			InItemData->Position = InPosition;

			ItemInPosition->OnItemUpdated.Broadcast(ItemInPosition);
			UE_LOG(LogInventoryComponent, Display, TEXT("Item moved %s"), *ItemInPosition->ToString());
			
			InItemData->OnItemUpdated.Broadcast(InItemData);
			UE_LOG(LogInventoryComponent, Display, TEXT("Item moved %s"), *InItemData->ToString());
			
			return true;
		}
	}
	
	return false;
}

bool UDSInventoryComponent::AddSlots(int32 InCount)
{
	InCount = FMath::Clamp(InCount, 0, MaxSize.X * MaxSize.Y - CurrentSlotsCount);
	if (InCount == 0)
	{
		return false;
	}
	
	for (int32 i = CurrentSlotsCount; i < CurrentSlotsCount + InCount; i++)
	{
		SetSlotState(FIntPoint(i % MaxSize.X, i / MaxSize.X), EDSSlotState::Void);
	}
	
	CurrentSlotsCount += InCount;
	OnSlotsIncremented.Broadcast(InCount);
	
	UE_LOG(LogInventoryComponent, Display, TEXT("Slots incremented. Current slots count %i"), CurrentSlotsCount);
	
	return true;
}

void UDSInventoryComponent::SetSlotState(const FIntPoint& InPosition, const EDSSlotState& InState)
{
	if (SlotsMatrix == nullptr ||
		InPosition.X < 0 || InPosition.X >= MaxSize.X ||
		InPosition.Y < 0 || InPosition.Y >= MaxSize.Y)
	{
		return;
	}

	SlotsMatrix[InPosition.Y][InPosition.X] = InState;
}

void UDSInventoryComponent::AddItem(UDSItemData* InItemData)
{
	Items.Add(InItemData);
	InItemData->Rename(nullptr, this);

	SetSlotState(InItemData->GetPosition(), EDSSlotState::Item);
	if (InItemData->GetItemSize() == 2)
	{
		SetSlotState(InItemData->GetPosition() + FIntPoint(1, 0), EDSSlotState::Item);
	}

	UE_LOG(LogTemp, Display, TEXT("Add item: %s"), *InItemData->ToString());
	
	OnItemCreated.Broadcast(InItemData);
}

void UDSInventoryComponent::RemoveItem(UDSItemData* InItemData)
{
	Items.Remove(InItemData);
	InItemData->Rename(nullptr, nullptr);
	
	SetSlotState(InItemData->GetPosition(), EDSSlotState::Void);
	if (InItemData->GetItemSize() == 2)
	{
		SetSlotState(InItemData->GetPosition() + FIntPoint(1, 0), EDSSlotState::Void);
	}
	
	UE_LOG(LogTemp, Display, TEXT("Remove item: %s"), *InItemData->ToString());
	
	OnItemRemoved.Broadcast(InItemData);
}

bool UDSInventoryComponent::CanCombineItems(UDSBaseItem* InItem1, UDSBaseItem* InItem2) const
{
	return IsValid(InItem1) && InItem1->GetCanStack() && InItem1 == InItem2;
}

bool UDSInventoryComponent::CanCraftItems(UDSBaseItem* InItem1, UDSBaseItem* InItem2) const
{
	const UDSInventorySettings* CraftingList = GetDefault<UDSInventorySettings>();

	return IsValid(CraftingList) ? CraftingList->GetCraftRecipe(InItem1, InItem2).IsSet() : false;
}

void UDSInventoryComponent::CombineItems(UDSItemData* ItemInPosition, UDSItemData* MovingItem)
{
	if ((ItemInPosition->GetCount() + MovingItem->GetCount()) > ItemInPosition->GetItemMaxStackAmount())
	{
		MovingItem->Count = ItemInPosition->GetCount() + MovingItem->GetCount() - ItemInPosition->GetItemMaxStackAmount();
		ItemInPosition->Count = ItemInPosition->GetItemMaxStackAmount();

		ItemInPosition->OnItemUpdated.Broadcast(ItemInPosition);
		MovingItem->OnItemUpdated.Broadcast(MovingItem);
	}
	else
	{
		ItemInPosition->Count = ItemInPosition->GetCount() + MovingItem->GetCount();
		MovingItem->Count = 0;

		ItemInPosition->OnItemUpdated.Broadcast(ItemInPosition);
		if (MovingItem->GetOuter() == this)
		{
			TryRemoveItem(MovingItem);
		}
		else
		{
			MovingItem->OnItemUpdated.Broadcast(MovingItem);
		}
	}
}

void UDSInventoryComponent::CraftItems(UDSItemData* ItemInPosition, UDSItemData* MovingItem)
{
	const UDSInventorySettings* CraftingList = GetDefault<UDSInventorySettings>();

	if (IsValid(CraftingList) && IsValid(ItemInPosition) && IsValid(MovingItem))
	{
		TOptional<FDSCraftRecipe> CraftRecipe = CraftingList->GetCraftRecipe(ItemInPosition->GetItem(), MovingItem->GetItem());
		
		if (CraftRecipe.IsSet())
		{
			UDSItemData* NewItemData = CraftRecipe->Result->HasProperty(FDSWeaponItemData::StaticStruct()) ? NewObject<UDSWeaponData>(this) : NewObject<UDSItemData>(this);
			NewItemData->Initialize(CraftRecipe->Result, CraftRecipe->Count, ItemInPosition->GetPosition());

			TryRemoveItem(ItemInPosition);
			TryRemoveItem(MovingItem);

			TryAddItem(NewItemData);
		}
	}
}

void UDSInventoryComponent::Initialize()
{
	for (int32 i = 0; i < Items.Num(); i++)
	{
		SetSlotState(Items[i]->GetPosition(), EDSSlotState::Item);

		if (Items[i]->GetItemSize() == 2)
		{
			SetSlotState(Items[i]->GetPosition() + FIntPoint(1, 0), EDSSlotState::Item);
		}
	}
	
	UE_LOG(LogInventoryComponent, Display, TEXT("Inventory component initialized"));
	
	OnInventoryInitialized.Broadcast();
	bInitialized = true;
}

void UDSInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CreateMatrix();
	Initialize();
}

void UDSInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	DestroyMatrix();
}

void UDSInventoryComponent::CreateMatrix()
{
	CurrentSlotsCount = FMath::Clamp(CurrentSlotsCount, 1, MaxSize.X * MaxSize.Y);

	SlotsMatrix = new EDSSlotState * [MaxSize.Y];

	for (int32 i = 0; i < MaxSize.Y; i++)
	{
		SlotsMatrix[i] = new EDSSlotState[MaxSize.X];

		for (int32 j = 0; j < MaxSize.X; j++)
		{
			if ((i * MaxSize.X + j) < CurrentSlotsCount)
			{
				SlotsMatrix[i][j] = EDSSlotState::Void;
				continue;
			}

			SlotsMatrix[i][j] = EDSSlotState::Block;
		}
	}
}

void UDSInventoryComponent::DestroyMatrix()
{
	for (int32 i = 0; i < MaxSize.Y; i++)
	{
		delete[] SlotsMatrix[i];
	}
	delete[] SlotsMatrix;

	SlotsMatrix = nullptr;
}
