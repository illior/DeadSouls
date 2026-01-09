// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSInventoryTypes.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDSOnInventoryInitializedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDSOnInventorySlotsIncrementedSignature, int32, DeltaSlots);

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

USTRUCT()
struct FDSItemInstancedProperty
{
	GENERATED_BODY()
	
};

USTRUCT(Blueprintable)
struct FDSAmmoItemData : public FDSItemInstancedProperty
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EDSAmmoType AmmoType = EDSAmmoType::Pistol;
};

USTRUCT(Blueprintable)
struct FDSWeaponItemData : public FDSItemInstancedProperty
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1"))
	int32 MaxAmmoInClip = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EDSAmmoType AmmoType = EDSAmmoType::Pistol;
};
