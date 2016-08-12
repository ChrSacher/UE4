// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "EquippedItemWidget.h"
#include "ItemWidget.h"
#include "InventoryComponent.h"


UTexture2D* UEquippedItemWidget::getTexture()
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

void UEquippedItemWidget::OnItemDrop(UDragDropOperation* operation)
{

	UItemWidget* x = Cast<UItemWidget>(operation->Payload);
	if (!x) return;
	if (x->ItemButton->UserPointer)
	{
		inventoryParent->equip(this, x->ItemButton->UserPointer);
	}
	//
}