// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Weapon.h"

#include "ItemDetailWidget.generated.h"

class UItemBase;
class UWeaponItem;
/**
 * 
 */
UCLASS()
class STALKERGHOSTS_API UItemDetailBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
		virtual void load(UItemBase* base) {}

		UFUNCTION(BlueprintImplementableEvent)
			void forceConstruct();
};


UCLASS()
class STALKERGHOSTS_API UItemDetailWidget : public UItemDetailBaseWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
			UEditableTextBox* itemNameText;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
			UMultiLineEditableTextBox* itemFlavourText;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
			UEditableTextBox* weightText;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
			UEditableTextBox* ammountText;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
			UImage* picture;
		virtual void load(UItemBase* base);
	
};

UCLASS()
class STALKERGHOSTS_API UWeaponDetailWidget : public UItemDetailBaseWidget
{
	GENERATED_BODY()

public:	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UTextBlock* itemNameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UTextBlock* itemFlavourText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UTextBlock* weightText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UTexture2D* picture;
	void load(UItemBase* wep);

};