// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StalkerGhosts.h"
#include "ItemCategoryWidget.h"
#include "ItemDetailWidget.h"
#include "EquippedItemWidget.h"
#include "MainInventoryWidget.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS()
class STALKERGHOSTS_API UItemScrollBoxWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UInventoryComponent* inventoryParent;
	UFUNCTION(BlueprintCallable, Category = "Event")
		void OnItemDrop(UDragDropOperation* operation);
};
UCLASS()
class STALKERGHOSTS_API UMainInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		TArray<UItemCategoryWidget*> categories;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		TArray<UItemCategoryWidget*> otherCategories;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UItemScrollBoxWidget* ItemBoxWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UScrollBox* ItemBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UHorizontalBox* CategoryBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UItemDetailWidget* details;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UItemScrollBoxWidget* otherItemBoxWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UScrollBox* otherItemBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UHorizontalBox* otherCategoryBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UBorder* otherBorder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		TArray<UEquippedItemWidget*> widgets;

	UFUNCTION(BlueprintCallable, Category = "Event")
		void setEquippedWidget(SlotInformation sslot, UEquippedItemWidget* widget);
	UFUNCTION(BlueprintCallable, Category = "Event")
		UEquippedItemWidget* getEquippedWidget(SlotInformation sslot);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void setup();


};
