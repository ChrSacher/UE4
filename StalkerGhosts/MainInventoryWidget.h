// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StalkerGhosts.h"
#include "ItemWidget.h"
#include "MainInventoryWidget.generated.h"
/**
 * 
 */
UCLASS()
class STALKERGHOSTS_API UMainInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UScrollBox* ItemBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UHorizontalBox* CategoryBox;
	
	
};
