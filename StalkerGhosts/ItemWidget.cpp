// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "DataItemButton.h"
#include "ItemBase.h"
#include "ItemWidget.h"
#include "MainGameInstance.h"


UTexture2D*  UItemWidget::getTexture()
{
	if (ItemButton)
	{
		if (ItemButton->UserPointer)
		{
			if (ItemButton->UserPointer->picture)
			{
				return ItemButton->UserPointer->picture;
			}
		}
	}
	return NULL;
}


int32  UItemWidget::getAmmount()
{
	if (ItemButton)
	{
		if (ItemButton->UserPointer)
		{
			if (ItemButton->UserPointer->picture)
			{
				return ItemButton->UserPointer->ammount;
			}
		}
	}
	return 0;
}


FString UItemWidget::getName()
{
	if (ItemButton)
	{
		if (ItemButton->UserPointer)
		{
			if (ItemButton->UserPointer->picture)
			{
				return ItemButton->UserPointer->name;
			}
		}
	}
	return "ERROR";
}

void UItemWidget::loadVisuals()
{
	
	
	if (!ItemButton)
	{
		AddToViewport();
		RemoveFromViewport();
	}
	if (!ItemButton) return;
	if (!ItemButton->UserPointer) return;
	name->SetText(FText::FromString(ItemButton->UserPointer->name));
	ammount->SetText(FText::FromString(FString::FromInt(ItemButton->UserPointer->ammount)));
	image->SetBrushFromTexture(ItemButton->UserPointer->picture);
	
	
}

UItemWidget*  UItemWidget::isDragDetected()
{
	APlayerController* player = GetOwningPlayer();
	if (!player) return NULL;
	UItemWidget* x = CreateWidget<UItemWidget>(GetWorld(), GetClass());

	x->AddToViewport(); 
	x->RemoveFromViewport();
	x->forceConstruct();
	x->ItemButton->UserPointer = ItemButton->UserPointer;
	x->loadVisuals();
	
	return x;
}