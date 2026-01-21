// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DSFooterWidget.generated.h"

class UInputAction;
class UDSKeyDescriptionWidget;

UCLASS()
class DEADSOULS_API UDSFooterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Footer")
	void SetDescriptionByName(FName InMappingName, FText InDescription);
	UFUNCTION(BlueprintCallable, Category = "Footer")
	void SetDescriptionByAction(UInputAction* InAction, FText InDescription);
	UFUNCTION(BlueprintCallable, Category = "Footer")
	void ClearAllDescriptions();
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Footer Data")
	TArray<UDSKeyDescriptionWidget*> KeyDescriptions;
};
