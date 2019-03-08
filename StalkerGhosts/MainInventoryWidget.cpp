// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "MainInventoryWidget.h"
#include "ItemWidget.h"
#include "InventoryComponent.h"

void UMainInventoryWidget::setEquippedWidget(SlotInformation sslot, UEquippedItemWidget* widget)
{
	widgets[(uint8)sslot] = widget;
}
UEquippedItemWidget* UMainInventoryWidget::getEquippedWidget(SlotInformation sslot)
{
	return widgets[(uint8)sslot];
}

void UMainInventoryWidget::setup()
{
	for (int i = 0; i < (uint8)SlotInformation::NUM; i++)
	{
		widgets.Add(NULL);
	}
}

void UItemScrollBoxWidget::OnItemDrop(UDragDropOperation* operation)
{
	UItemWidget* x = Cast<UItemWidget>(operation->Payload);
	if (!inventoryParent)
	{
		UE_LOG(LogTemp, Warning, TEXT("SCROLLBOX MAJOR ERROR"));
	}
	if (x)
	{
		inventoryParent->moveItem(x, x->ItemButton->UserPointer, this);
	}
	UEquippedItemWidget* y = Cast<UEquippedItemWidget>(operation->Payload);
	if (y)
	{
		x->ItemButton->UserPointer->itemParent->unEquip(y, y->ItemButton->UserPointer,this);
	}
	//
}