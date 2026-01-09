// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StructUtils/InstancedStruct.h"
#include "DSBaseItem.generated.h"

UCLASS(BlueprintType)
class INVENTORYSYSTEM_API UDSBaseItem : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FText GetDisplayName() const { return DisplayName; }
	FText GetDisplayDescription() const { return DisplayDescription; }
	bool GetCanStack() const { return bCanStack; }
	int32 GetMaxStackAmount() const { return MaxStackAmount; }
	int32 GetSize() const { return Size; }
	UTexture2D* GetIcon() const { return Icon; }
	
	TOptional<FInstancedStruct> GetProperty(const UScriptStruct* InScriptStruct) const;
	bool HasProperty(const UScriptStruct* InScriptStruct) const;
	
	virtual FString ToString() const;
	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Item")
	FText DisplayName = FText::FromString("Item Name");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Item")
	FText DisplayDescription = FText::FromString("Item Description");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Item")
	bool bCanStack = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Item", meta = (ClampMin = "1", ClampMax = "400", EditCondition = "bCanStack"))
	int32 MaxStackAmount = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Item", meta = (ClampMin = "1", ClampMax = "2"))
	int32 Size = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Item")
	TObjectPtr<UTexture2D> Icon = nullptr;
	
	UPROPERTY(EditAnywhere, meta = (BaseStruct = "/Script/InventorySystem.DSItemInstancedProperty", ExcludeBaseStruct), Category = "Base Item")
	TArray<FInstancedStruct> ItemProperties;
};
