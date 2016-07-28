// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "DataItemButton.h"
#include "ItemEnums.h"
#include "EquippedItemWidget.generated.h"

class UItemBase;
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
	UEquippedItemDelegate onEq;
	UEquippedItemDelegate offEq;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		ItemCategory allowedType = ItemCategory::ALL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		SlotInformation slotEnum = SlotInformation::NON;
	UFUNCTION(BlueprintCallable, Category = "Event")
		UTexture2D* getTexture();
	//void called when itemWidget gets dropped on this
	UFUNCTION(BlueprintCallable, Category = "Event")
	void onEquip(UItemBase* base)
	{
		
		onEq.ExecuteIfBound(this, base);
	}
	//called when this is droppedon an itembox
	UFUNCTION(BlueprintCallable, Category = "Event")
	void onUnequiped(UItemBase* base)
	{
		
		offEq.ExecuteIfBound(this, base);
		
	}
	
};
