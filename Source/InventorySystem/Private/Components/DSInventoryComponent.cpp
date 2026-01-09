// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/DSInventoryComponent.h"
#include "DSItemData.h"

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
	
	OnInventoryInitialized.Broadcast();
	bInitialized = true;

	UE_LOG(LogInventoryComponent, Display, TEXT("Inventory component initialized"));
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

