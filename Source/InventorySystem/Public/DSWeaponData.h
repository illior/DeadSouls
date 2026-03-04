// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSInventoryTypes.h"
#include "DSItemData.h"
#include "DSWeaponData.generated.h"

UCLASS()
class INVENTORYSYSTEM_API UDSWeaponData : public UDSItemData
{
	GENERATED_BODY()
	
	friend class UDSInventoryComponent;
public:
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	int32 GetItemMaxAmmoInClip() const;
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	EDSAmmoType GetItemAmmoType() const;

	int32 GetCurrentAmmoInClip() const { return CurrentAmmoInClip; };
	EDSWeaponFastAccessIndex GetFastAccessIndex() const { return FastAccessIndex; };
	bool GetIsEquip() const { return bIsEquip; };
	
	bool SetCount(const int32 InCount);
	
	virtual void Initialize(UDSBaseItem* InItem, int32 InCount, FIntPoint InPosition) override;

	virtual bool IsFull() const override;
	virtual bool CanDrop() const override;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame, Category = "Weapon Data", meta = (ClampMin = "0"))
	int32 CurrentAmmoInClip = 1;

	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Weapon Data")
	EDSWeaponFastAccessIndex FastAccessIndex = EDSWeaponFastAccessIndex::None;
	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Weapon Data")
	bool bIsEquip = false;
	
	FDSWeaponItemData GetCachedWeaponItem() const;
private:
	mutable FDSWeaponItemData CachedWeaponItem = FDSWeaponItemData();
};
