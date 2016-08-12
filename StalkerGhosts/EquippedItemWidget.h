// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "DataItemButton.h"
#include "ItemEnums.h"
#include "EquippedItemWidget.generated.h"

class UItemBase;
class UInventoryComponent;
/**
 * 
 */
UCLASS()
class STALKERGHOSTS_API UEquippedItemWidget : public UUserWidget
{
	GENERATED_BODY()

		DECLARE_DELEGATE_TwoParams(UEquippedItemDelegate, UEquippedItemWidget*, UItemBase*)
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UDataItemButton* ItemButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		ItemCategory allowedType = ItemCategory::ALL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		SlotInformation slotEnum = SlotInformation::NON;
	UFUNCTION(BlueprintCallable, Category = "Event")
		UTexture2D* getTexture();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UInventoryComponent* inventoryParent;

	UFUNCTION(BlueprintCallable, Category = "Event")
		void OnItemDrop(UDragDropOperation* operation);
	
};
