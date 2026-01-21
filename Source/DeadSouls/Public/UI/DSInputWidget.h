// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DSInputWidget.generated.h"

class UDSFooterWidget;

UCLASS()
class DEADSOULS_API UDSInputWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UDSFooterWidget* GetFooterWidget() const;
	
	virtual void Open();
	virtual void Close();
	
	virtual void DisableInput();
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UDSFooterWidget> Footer;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnOpen();
	UFUNCTION(BlueprintImplementableEvent)
	void OnClose();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDisableInput();
	
	virtual void NativeOnInitialized() override;
};
