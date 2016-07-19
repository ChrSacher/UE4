// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "DataItemButton.h"
#include "ItemEnums.h"
#include "ItemCategoryWidget.generated.h"

//enum ItemCategory;
/**
 * 
 */
UCLASS()
class STALKERGHOSTS_API UItemCategoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UTexture2D* CategoryPicture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UDataItemButton* CategoryButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	ItemCategory cat;
};
