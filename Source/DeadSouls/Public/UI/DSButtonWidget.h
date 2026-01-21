// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeadSoulsTypes.h"
#include "Blueprint/UserWidget.h"
#include "DSButtonWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class DEADSOULS_API UDSButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FDSOnButtonClickSignature OnClicked;

	FDSOnButtonHovered OnHovered;

	UFUNCTION(BlueprintImplementableEvent, Category = "Button", meta = (DisplayName = "StateChanged"))
	void ReceiveStateChanged(EDSButtonState NewState);

	UFUNCTION(BlueprintCallable, Category = "Button")
	virtual void SetState(EDSButtonState NewState);
	EDSButtonState GetState() const;

	UFUNCTION(BlueprintCallable, Category = "Button")
	virtual void Click();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> ButtonImage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button: Style")
	FSlateBrush BaseBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button: Style")
	FSlateBrush HoveredBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button: Style")
	FSlateBrush PressedBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button: Style")
	FSlateBrush DisabledBrush;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button: Content")
	FText Text = FText();

	UPROPERTY(BlueprintReadOnly)
	EDSButtonState State = EDSButtonState::Base;

	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
