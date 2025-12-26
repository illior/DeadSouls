// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DSTickManagerComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FDSTickDelegate, float);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEADSOULS_API UDSTickManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDSTickManagerComponent();

	FDSTickDelegate OnTick;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
