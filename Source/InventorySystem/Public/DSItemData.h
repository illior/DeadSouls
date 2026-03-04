// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DSInventoryTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "DSItemData.generated.h"

class UDSBaseItem;

UCLASS(BlueprintType, DefaultToInstanced, EditInlineNew)
class INVENTORYSYSTEM_API UDSItemData : public UObject
{
	GENERATED_BODY()
	
	friend class UDSInventoryComponent;
public:
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	FText GetItemName() const;
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	FText GetItemDescription() const;
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	bool GetItemCanStack() const;
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	int32 GetItemMaxStackAmount() const;
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	int32 GetItemSize() const;
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	UTexture2D* GetItemIcon() const;
	
	TOptional<FInstancedStruct> GetItemProperty(const UScriptStruct* InScriptStruct) const;
	bool ItemHasProperty(const UScriptStruct* InScriptStruct, const bool bCheckForChild = false) const;
	
	template<class T>
	T* GetItem() const { return Cast<T>(Item); };
	UDSBaseItem* GetItem() const { return Item; };
	int32 GetCount() const { return Count; };
	FIntPoint GetPosition() const { return Position; };
	
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	bool SetPosition(const FIntPoint InPosition);
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	bool SetCount(const int32 InCount);
	
	virtual void Initialize(UDSBaseItem* InItem, int32 InCount, FIntPoint InPosition);

	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	bool ContainsPoint(FIntPoint InPosition) const;
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	virtual bool IsFull() const;
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	virtual bool CanDrop() const;
	
	virtual FString ToString() const;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame, Category = "Item Data")
	TObjectPtr<UDSBaseItem> Item = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data", meta = (ClampMin = "1"))
	int32 Count = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	FIntPoint Position = FIntPoint(0, 0);
};
