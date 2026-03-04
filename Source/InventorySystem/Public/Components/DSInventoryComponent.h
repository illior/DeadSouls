// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSInventoryTypes.h"
#include "Components/ActorComponent.h"
#include "DSInventoryComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInventoryComponent, Log, All);

class UDSBaseItem;
class UDSItemData;
class UDSWeaponData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UDSInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDSInventoryComponent();
	
	UPROPERTY(BlueprintAssignable, Category = "InventorySystem")
	FDSOnInventoryInitializedSignature OnInventoryInitialized;
	
	UPROPERTY(BlueprintAssignable, Category = "InventorySystem")
	FDSOnInventoryDocumentAddedSignature OnDocumentAdded;
	
	UPROPERTY(BlueprintAssignable, Category = "InventorySystem")
	FDSOnInventorySlotsIncrementedSignature  OnSlotsIncremented;
	
	UPROPERTY(BlueprintAssignable, Category = "InventorySystem")
	FDSOnInventoryTryingAddItemSignature  OnTryingAddItem;
	
	UPROPERTY(BlueprintAssignable, Category = "InventorySystem")
	FDSOnInventoryItemCreatedSignature  OnItemCreated;
	UPROPERTY(BlueprintAssignable, Category = "InventorySystem")
	FDSOnInventoryItemRemovedSignature  OnItemRemoved;
	UPROPERTY(BlueprintAssignable, Category = "InventorySystem")
	FDSOnInventoryItemUpdatedSignature OnItemUpdated;
	
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	EDSSlotState GetSlotState(FIntPoint InPosition) const;
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	bool IsInitialized() const { return bInitialized; }
	FIntPoint GetMaxSize() const { return MaxSize; }
	int32 GetCurrentSlotsCount() const { return CurrentSlotsCount; }
	
	UDSItemData* GetItemByPosition(FIntPoint InPosition) const;
	UDSWeaponData* GetFastAccessWeapon(EDSWeaponFastAccessIndex FastAccessIndex) const;
	UDSWeaponData* GetEquippedWeapon() const;
	
	
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	void TryAddDocument(const FDataTableRowHandle InHandle);
	
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	void TryAddNewItem(UDSItemData* InItemData);
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	bool TryAddItem(UDSItemData* InItemData);
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	bool TryRemoveItem(UDSItemData* InItemData, bool bIsGarbage = true);
	
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	bool TrySetItemPosition(UDSItemData* InItemData, FIntPoint InPosition);
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	bool TrySetItemCount(UDSItemData* InItemData, int32 InCount);
	
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	bool SetFastAccessWeapon(UDSWeaponData* WeaponData, EDSWeaponFastAccessIndex FastAccessIndex);
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	bool SetEquippedWeapon(UDSWeaponData* WeaponData);
	
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	bool AddSlots(int32 InCount);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InventorySystem")
	FIntPoint MaxSize = FIntPoint(4, 5);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame, Category = "InventorySystem", meta = (ClampMin = "1"))
	int32 CurrentSlotsCount = 8;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "InventorySystem")
	TArray<TObjectPtr<UDSItemData>> Items;
	
	UPROPERTY(BlueprintReadOnly, Category = "InventorySystem")
	TObjectPtr<UDSWeaponData> EquippedWeapon;
	
	UPROPERTY(BlueprintReadOnly, Category = "InventorySystem")
	TMap<EDSWeaponFastAccessIndex, TObjectPtr<UDSWeaponData>> FastAccessWeapons;
	
	UPROPERTY(BlueprintReadOnly, Category = "InventorySystem")
	TArray<FName> Documents; 
	
	void SetSlotState(const FIntPoint& InPosition, const EDSSlotState& InState);
	
	void AddItem(UDSItemData* InItemData);
	void RemoveItem(UDSItemData* InItemData);
	
	bool CanCombineItems(UDSBaseItem* InItem1, UDSBaseItem* InItem2) const;
	bool CanCraftItems(UDSBaseItem* InItem1, UDSBaseItem* InItem2) const;

	void CombineItems(UDSItemData* ItemInPosition, UDSItemData* MovingItem);
	void CraftItems(UDSItemData* ItemInPosition, UDSItemData* MovingItem);
	
	virtual void Initialize();
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	bool bInitialized = false;
	EDSSlotState** SlotsMatrix;
	
	void CreateMatrix();
	void DestroyMatrix();
};
