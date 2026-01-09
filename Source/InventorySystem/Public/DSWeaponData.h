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
	
public:
	int32 GetCurrentAmmoInClip() const { return CurrentAmmoInClip; };
	int32 GetFastAccessIndex() const { return FastAccessIndex; };
	bool GetIsEquip() const { return bIsEquip; };

	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	int32 GetItemMaxAmmoInClip() const;
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	EDSAmmoType GetItemAmmoType() const;

	virtual void Initialize(UDSBaseItem* InItem, int32 InCount, FIntPoint InPosition) override;

	virtual bool IsFull() const override;
	virtual bool CanDrop() const override;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame, Category = "InventorySystem", meta = (ClampMin = "0"))
	int32 CurrentAmmoInClip = 1;

	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "InventorySystem")
	int32 FastAccessIndex = -1;
	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "InventorySystem")
	bool bIsEquip = false;
	
	FDSWeaponItemData* GetCachedWeaponItem() const;
private:
	mutable FDSWeaponItemData* CachedWeaponItem;
};
