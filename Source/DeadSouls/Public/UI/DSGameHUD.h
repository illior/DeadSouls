// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DSGameHUD.generated.h"

class UDSInputWidget;

UCLASS()
class DEADSOULS_API ADSGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void OpenInventoryWidget();
	void OpenPauseWidget();
	
	UFUNCTION(BlueprintCallable, Category = "GameHUD")
	void OpenWidget(UDSInputWidget* WidgetToShow);
	UFUNCTION(BlueprintCallable, Category = "GameHUD")
	void CloseCurrentWidgets();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeadSouls: UI")
	TSubclassOf<UDSInputWidget> InventoryWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DeadSouls: UI")
	TSubclassOf<UDSInputWidget> PauseWidgetClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDSInputWidget> CurrentInputWidget;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDSInputWidget> InventoryWidget;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDSInputWidget> PauseWidget;
	
	void BeginPlay() override;
};
