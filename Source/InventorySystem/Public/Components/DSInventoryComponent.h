// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DSInventoryTypes.h"
#include "Components/ActorComponent.h"
#include "DSInventoryComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInventoryComponent, Log, All);

class UDSItemData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEM_API UDSInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDSInventoryComponent();
	
	UPROPERTY(BlueprintAssignable, Category = "InventorySystem")
	FDSOnInventoryInitializedSignature OnInventoryInitialized;
	
	UPROPERTY(BlueprintAssignable, Category = "InventorySystem")
	FDSOnInventorySlotsIncrementedSignature  OnSlotsIncremented;
	
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	EDSSlotState GetSlotState(FIntPoint InPosition) const;
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	bool IsInitialized() const { return bInitialized; }
	FIntPoint GetMaxSize() const { return MaxSize; }
	int32 GetCurrentSlotsCount() const { return CurrentSlotsCount; }
	
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	bool AddSlots(int32 InCount);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InventorySystem")
	FIntPoint MaxSize = FIntPoint(4, 5);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame, Category = "InventorySystem", meta = (ClampMin = "1"))
	int32 CurrentSlotsCount = 8;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "InventorySystem")
	TArray<TObjectPtr<UDSItemData>> Items;
	
	void SetSlotState(const FIntPoint& InPosition, const EDSSlotState& InState);
	
	virtual void Initialize();
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	bool bInitialized = false;
	EDSSlotState** SlotsMatrix;
	
	void CreateMatrix();
	void DestroyMatrix();
};
