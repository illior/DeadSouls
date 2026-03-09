// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSInventoryTypes.generated.h"

class UDSBaseItem;
class UDSItemData;
class UDSWeaponData;
struct FDSDocument;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDSOnInventoryInitializedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDSOnInventorySlotsIncrementedSignature, int32, DeltaSlots);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDSOnInventoryTryingAddItemSignature, UDSItemData*, InItemData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDSOnInventoryItemCreatedSignature, UDSItemData*, InItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDSOnInventoryItemRemovedSignature, UDSItemData*, InItemData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDSOnInventoryItemUpdatedSignature, UDSItemData*, InItemData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDSOnInventoryWeaponEquippedSignature, UDSWeaponData*, InWeaponData);

USTRUCT(BlueprintType)
struct FDSDocument : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Document")
	FText Title;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Document", meta = (MultiLine = "true"))
	TArray<FText> Pages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory Document")
	TSoftObjectPtr<UTexture2D> Item;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDSOnInventoryDocumentAddedSignature, FName, RowName, FDSDocument, Document);

UENUM(BlueprintType)
enum class EDSSlotState : uint8
{
	Void,
	Item,
	Block,
};

UENUM(BlueprintType)
enum class EDSAmmoType : uint8
{
	Pistol,
	Shotgun,
	SubmachineGun,
	Revolver,
	Rifle,
	AssaultRifle,
	Bolts,
	Custom1,
	Custom2,
	Custom3
};

UENUM(BlueprintType)
enum class EDSWeaponFastAccessIndex : uint8
{
	None = 0,
	First = 1,
	Second = 2,
	Third = 3,
	Fourth = 4
};

ENUM_RANGE_BY_FIRST_AND_LAST(EDSWeaponFastAccessIndex, EDSWeaponFastAccessIndex::First, EDSWeaponFastAccessIndex::Fourth);

USTRUCT()
struct FDSItemInstancedProperty
{
	GENERATED_BODY()
	
};

USTRUCT(Blueprintable)
struct FDSAmmoProperty : public FDSItemInstancedProperty
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EDSAmmoType AmmoType = EDSAmmoType::Pistol;
};

USTRUCT(Blueprintable)
struct FDSWeaponProperty : public FDSItemInstancedProperty
{
	GENERATED_BODY()
	
	FDSWeaponProperty()
	{
		MaxAmmoInClip = 0;
		AmmoType = EDSAmmoType::Pistol;
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1"))
	int32 MaxAmmoInClip = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EDSAmmoType AmmoType = EDSAmmoType::Pistol;
};

USTRUCT(Blueprintable)
struct FDSCraftRecipe
{
	GENERATED_BODY()


public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDSBaseItem> FirstItem;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDSBaseItem> SecondItem;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDSBaseItem> Result;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1"))
	int32 Count = 1;
};
