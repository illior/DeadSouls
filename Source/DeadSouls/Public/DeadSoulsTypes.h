// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DeadSoulsTypes.generated.h"

class UDSMenuWidget;
class UDSButtonWidget;

DECLARE_DELEGATE_OneParam(FDSOnButtonHovered, UDSButtonWidget*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDSOnButtonClickSignature, UDSButtonWidget*, Button);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDSOnCloseMenuSignature, UDSMenuWidget*, Menu);

UENUM(BlueprintType)
enum class EDSButtonState : uint8
{
	Base,
	Hovered,
	Pressed,
	Disabled
};