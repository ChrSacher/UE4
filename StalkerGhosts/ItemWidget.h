// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DataItemButton.h"
#include "ItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class STALKERGHOSTS_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()
		
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UDataItemButton* ItemButton;

	UFUNCTION(BlueprintCallable, Category = "Event")
		UTexture2D* getTexture();

	UFUNCTION(BlueprintCallable, Category = "Event")
		int32 getAmmount();

	UFUNCTION(BlueprintCallable, Category = "Event")
		FString getName();
	UFUNCTION(BlueprintCallable, Category = "Event")
		void loadVisuals();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UEditableTextBox* name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UEditableTextBox* ammount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UImage* image;

	UFUNCTION(BlueprintCallable, Category = "Event")
		UItemWidget* isDragDetected();

	UFUNCTION(BlueprintImplementableEvent)
		void forceConstruct();
};
