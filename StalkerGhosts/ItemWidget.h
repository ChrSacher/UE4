// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


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
		

	
};
