// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StalkerGhosts.h"
#include "ItemCategoryWidget.h"
#include "ItemDetailWidget.h"
#include "EquippedItemWidget.h"
#include "MainInventoryWidget.generated.h"


/**
 * 
 */
UCLASS()
class STALKERGHOSTS_API UItemScrollBoxWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
};
UCLASS()
class STALKERGHOSTS_API UMainInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UItemScrollBoxWidget* ItemBoxWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UScrollBox* ItemBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UHorizontalBox* CategoryBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UItemDetailWidget* details;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UEquippedItemWidget* helmetEquipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UEquippedItemWidget* armorEquipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UEquippedItemWidget* backBackEquipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UEquippedItemWidget* weapon1Equipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UEquippedItemWidget* artifact1Equipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UEquippedItemWidget* artifact2Equipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UEquippedItemWidget* artifact3Equipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UEquippedItemWidget* artifact4Equipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UEquippedItemWidget* quick1Equipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UEquippedItemWidget* quick2Equipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UEquippedItemWidget* quick3Equipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UEquippedItemWidget* quick4Equipped;

};
