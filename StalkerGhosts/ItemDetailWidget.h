// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Weapon.h"
#include "ItemBase.h"
#include "ItemDetailWidget.generated.h"

/**
 * 
 */
UCLASS()
class STALKERGHOSTS_API UItemDetailWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
			UTextBlock* itemNameText;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
			UTextBlock* itemFlavourText;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
			UTextBlock* weightText;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
			UTexture2D* picture;
		void load(UItemBase* base)
		{
			itemNameText->SetText(FText::FromString(base->name));
			itemFlavourText->SetText(FText::FromString(base->name));
			weightText->SetText(FText::FromString(FString::SanitizeFloat(base->weight)));
			picture = base->picture;
		};
	
};

UCLASS()
class STALKERGHOSTS_API UWeaponDetailWidget : public UUserWidget
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
	void load(UWeaponItem* wep)
	{
		itemNameText->SetText(FText::FromString(wep->name));
		itemFlavourText->SetText(FText::FromString(wep->name));
		weightText->SetText(FText::FromString(FString::SanitizeFloat(wep->weight)));
		picture = wep->picture;
	};

};