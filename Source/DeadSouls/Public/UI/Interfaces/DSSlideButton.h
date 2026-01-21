// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DSSlideButton.generated.h"

UINTERFACE(MinimalAPI)
class UDSSlideButton : public UInterface
{
	GENERATED_BODY()
};

class DEADSOULS_API IDSSlideButton
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Button", meta = (ForceAsFunction))
	void HorizontalMove(int32 Offset);
	virtual void HorizontalMove_Implementation(int32 Offset);
};
