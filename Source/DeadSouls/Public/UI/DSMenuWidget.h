// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeadSoulsTypes.h"
#include "Blueprint/UserWidget.h"
#include "DSMenuWidget.generated.h"

UCLASS()
class DEADSOULS_API UDSMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Menu|Event")
	FDSOnCloseMenuSignature OnCloseMenu;
	
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void AddButton(UDSButtonWidget* InButton);
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void AddButtons(TArray<UDSButtonWidget*> InButtons);
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void SetButtons(TArray<UDSButtonWidget*> InButtons);

	UFUNCTION(BlueprintCallable, Category = "Menu")
	virtual void CloseMenu();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Menu", meta = (ForceAsFunction))
	void InFocus();
	virtual void InFocus_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Menu", meta = (ForceAsFunction))
	void OutFocus();
	virtual void OutFocus_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Menu", meta = (ForceAsFunction))
	void Confirm();
	virtual void Confirm_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Menu", meta = (ForceAsFunction))
	void MoveVertical(int32 Offset);
	virtual void MoveVertical_Implementation(int32 Offset);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Menu", meta = (ForceAsFunction))
	void MoveHorizontal(int32 Offset);
	virtual void MoveHorizontal_Implementation(int32 Offset);

	virtual void SetButtonSelected(UDSButtonWidget* InButton);

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UDSButtonWidget>> Buttons;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentIndex = 0;

	void SetButtonState(int32 Index, EDSButtonState InState);
};
