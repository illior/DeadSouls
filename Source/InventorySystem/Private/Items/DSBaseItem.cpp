// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/DSBaseItem.h"

TOptional<FInstancedStruct> UDSBaseItem::GetProperty(const UScriptStruct* InScriptStruct) const
{
	TOptional<FInstancedStruct> Result;
	
	for (int32 i = 0; i < ItemProperties.Num(); i++)
	{
		if (ItemProperties[i].GetScriptStruct() == InScriptStruct)
		{
			Result = TOptional(ItemProperties[i]);
			
			return Result;
		}
	}
	
	return Result;
}

bool UDSBaseItem::HasProperty(const UScriptStruct* InScriptStruct) const
{
	for (int32 i = 0; i < ItemProperties.Num(); i++)
	{
		if (ItemProperties[i].GetScriptStruct() == InScriptStruct)
		{
			return true;
		}
	}
	
	return false;
}

FString UDSBaseItem::ToString() const
{
	return FString::Printf(TEXT("Name: %s, Description: %s, CanStack: %s, MaxStackAmount: %i, Size: %i"),
		*(DisplayName.ToString()),
		*(DisplayDescription.ToString()),
		bCanStack ? TEXT("True") : TEXT("False"),
		MaxStackAmount,
		Size);
}

#if WITH_EDITOR
void UDSBaseItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (!bCanStack)
	{
		MaxStackAmount = 1;
	}
}
#endif
