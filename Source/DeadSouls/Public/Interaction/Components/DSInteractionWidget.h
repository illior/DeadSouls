// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DSInteractionWidget.generated.h"

UINTERFACE(MinimalAPI)
class UDSInteractionWidget : public UInterface
{
	GENERATED_BODY()
};

class DEADSOULS_API IDSInteractionWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void StartHold(float InHoldTime);
	virtual void StartHold_Implementation(float InHoldTime);
	
	UFUNCTION(BlueprintNativeEvent)
	void StopHold();
	virtual void StopHold_Implementation();
	
	UFUNCTION(BlueprintNativeEvent)
	void StartShow();
	virtual void StartShow_Implementation();
	
	UFUNCTION(BlueprintNativeEvent)
	void StartHide();
	virtual void StartHide_Implementation();
	
	UFUNCTION(BlueprintNativeEvent)
	void StartShowKey();
	virtual void StartShowKey_Implementation();
	
	UFUNCTION(BlueprintNativeEvent)
	void StartHideKey();
	virtual void StartHideKey_Implementation();
};
