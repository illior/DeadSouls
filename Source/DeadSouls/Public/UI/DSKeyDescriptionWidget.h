// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DSKeyDescriptionWidget.generated.h"

class UImage;
class UTextBlock;
class UInputAction;
struct FPlayerKeyMapping;

UCLASS()
class DEADSOULS_API UDSKeyDescriptionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FName GetMappingName() const;
	const UInputAction* GetAssociatedInputAction() const;
	
	void SetDescription(const FText& InText) const;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> KeyImage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> DescriptionText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DeadSouls: Input", meta = (GetOptions = "EnhancedInput.PlayerMappableKeySettings.GetKnownMappingNames"))
	FName MappingName;
	
	FPlayerKeyMapping* KeyMapping;
	
	UFUNCTION()
	virtual void OnMappingsRebuilt();
	
	virtual void NativeOnInitialized() override;
};
