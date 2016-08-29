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

		
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UDataItemButton* ItemButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		ItemCategory allowedType = ItemCategory::ALL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		SlotInformation slotEnum = SlotInformation::NON;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UImage* image;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UInventoryComponent* inventoryParent;

	UFUNCTION(BlueprintCallable, Category = "Event")
		void OnItemDrop(UDragDropOperation* operation);

	UFUNCTION(BlueprintCallable, Category = "Event")
		void loadVisuals();

	UFUNCTION(BlueprintImplementableEvent)
		void forceConstruct();

	UFUNCTION(BlueprintCallable, Category = "Event")
		UEquippedItemWidget*  isDragDetected();
};

UCLASS()
class STALKERGHOSTS_API UWeaponAttachmentWidget : public UUserWidget
{
	GENERATED_BODY()

		
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UDataItemButton* ItemButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		ItemCategory allowedType = ItemCategory::WEAPON;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		WeaponAttachmentSlot weaponAttachmentSlot = WeaponAttachmentSlot::GRIP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UImage* attachmentImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UInventoryComponent* inventoryParent;

	UFUNCTION(BlueprintCallable, Category = "Event")
		void OnItemDrop(UDragDropOperation* operation);

	UFUNCTION(BlueprintCallable, Category = "Event")
		void loadVisuals();

	UFUNCTION(BlueprintImplementableEvent)
		void forceConstruct();
	UFUNCTION(BlueprintCallable, Category = "Event")
		UWeaponAttachmentWidget*  isDragDetected();

};
