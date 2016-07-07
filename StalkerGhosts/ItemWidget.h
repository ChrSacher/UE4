// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "ItemCategoryWidget.h"
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
		FString ItemText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		FString ItemAmmount;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UTexture2D* ItemPicture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UDataItemButton* ItemButton;
		

	
};
