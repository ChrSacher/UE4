// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "ItemDetailWidget.h"




void UItemDetailWidget::load(UItemBase* base)
{
	forceConstruct();
	itemNameText->SetText(FText::FromString(base->name));
	itemFlavourText->SetText(FText::FromString(base->name));
	weightText->SetText(FText::FromString(FString::SanitizeFloat(base->weight)));
	picture->SetBrushFromTexture(base->picture);
};

void UWeaponDetailWidget::load(UItemBase* wep)
{
	forceConstruct();
	itemNameText->SetText(FText::FromString(wep->name));
	itemFlavourText->SetText(FText::FromString(wep->name));
	weightText->SetText(FText::FromString(FString::SanitizeFloat(wep->weight)));
	picture = wep->picture;
};